/**
 * Description: This module runs processes sent to it from the command line and logs their results.
 *              It restarts any process that lasts longer than 2 seconds.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 04/20/2023
 * Creation date: 04/16/2023
 **/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_LENGTH 31

int executeCommand(char **commands, int index);

struct nlist {
    time_t starttime;   // Clock time when process started
    time_t finishtime;  // Clock time when process finished
    int index;          // this is the line index in the input text file 
    int pid;            // the process id. you can use the pid result of wait to lookup in the hashtable 
    char *command;      // command. This is good to store for when you decide to restart a command
    struct nlist *next; //next entry in chain 
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

unsigned hash(int pid)
{
    unsigned hashval = pid;
    return hashval % HASHSIZE;
}

/* This is traversing the linked list under a slot of the hash
table. The array position to look in is returned by the hash
function */
struct nlist *lookup(int pid)
{
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
            return np; /* found */
    return NULL; /* not found */
}


/**
 * @brief Create new execution node at the head of the appropriate list.
 * 
 * @param pid 
 * @return struct nlist* 
 */
struct nlist *insert(int pid)
{
    struct nlist *np;
    unsigned hashval;

    // Only insert non existent nodes
    if ((np = lookup(pid)) == NULL) { 
        /* case 1: the pid is not
        found, so you have to create it with malloc. Then you want to set
        the pid, command and index */
        np = (struct nlist *) malloc(sizeof(struct nlist));
        if (np == NULL)
            return NULL;
        np->pid = pid;
        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }

    return np;
}

int main( int argc, char *argv[] ) {
    
    memset(hashtab,0,HASHSIZE*sizeof(struct nlist*));

    int cpid;

    // Create 2D array to track all commands
    int count = 0;
    int size= 10;

    char** commands;
    commands = (char**) malloc(size * sizeof(char*));
    memset(commands, 0, size * sizeof(char*));

    // For managing files
    char fileName[MAX_LENGTH];

    // Get commands from stdin, store dynamic array
    while(1) {
        // Expand commands array as needed
        if(count >= size) {
            size += 10;
            commands = (char**) realloc(commands, size*sizeof(char *));
        }

        // This memory should be freed when commands is freed
        char* input = (char*) malloc(MAX_LENGTH * sizeof(char));
        memset(input, 0, MAX_LENGTH * sizeof(char));

        if( fgets(input, MAX_LENGTH, stdin) == NULL) {
            free(input);
            break;
        }
        // Cleanup non-blank read
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        commands[count] = input;
        count++;  
    }


    // Create one child process per command, store in hash table
    int i = 0;
    for(; i < count; ++i) {
        time_t start = time(NULL);
        cpid = fork();
        
        if(cpid < 0) {
            return 1;
        }

        // Child should not finish the loop
        else if(cpid == 0) {
            // Use i to know which command this child is responsible for
            break;
        }
        // Parent must initialize hash node
        else {
            struct nlist* tempNode = insert(cpid);
            tempNode->command = commands[i];
            tempNode->index = i;
            tempNode->starttime = start;
        }
    }

    // Parent thread coordinates workers and stores their output. Restarts as needed.
    if(cpid > 0) {
        int pid, status;
        
        while ((pid = wait(&status)) > 0) {
            // Get appropriate node and populate it
            time_t end = time(NULL);
            struct nlist* node = lookup(pid);
            node->finishtime = end;

            // Open necessary files
            sprintf(fileName, "%d.out", pid);
            FILE* fd_out = fopen(fileName, "a");
            sprintf(fileName, "%d.err", pid);
            FILE* fd_err = fopen(fileName, "a");

            if (WIFEXITED(status)) {
                fprintf(fd_err, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(fd_err, "Killed with signal %d\n", WTERMSIG(status));  
            }

            fprintf(fd_err, "spawning too fast\n"); 
            fprintf(fd_out, "Finished child %d pid of parent %d\n", pid, getpid());
            fprintf(fd_out, "Finished at %ld, runtime duration %ld\n", node->finishtime, node->finishtime-node->starttime);
            fclose(fd_out);
            fclose(fd_err);


            if(node->finishtime - node->starttime > 2) {
                // Restart
                time_t start = time(NULL);
                int cpid2 = fork();
                if(cpid2 < 0) { return 1; }
                // Child
                if(cpid2 == 0) {
                    // Open necessary files
                    sprintf(fileName, "%d.out", getpid());
                    FILE* fd_out = fopen(fileName, "a");
                    sprintf(fileName, "%d.err", getpid());
                    FILE* fd_err = fopen(fileName, "a");

                    fprintf(fd_err, "RESTARTING\n"); 
                    fprintf(fd_out, "RESTARTING\n");
                    fclose(fd_out);
                    fclose(fd_err);

                    return executeCommand(commands, node->index);
                }
                // Parent
                else {
                    struct nlist* tempNode = insert(cpid2);
                    tempNode->command = node->command;
                    tempNode->index = node->index;
                    tempNode->starttime = start;
                }
            }
        }

        // Free all commands in the array after all execution is complete.
        for(int j = 0; j < count; j++) {
            free(commands[j]);
        }
        free(commands);

        // Free all nodes in hashtable
        for(int j = 0; j < HASHSIZE; j++) {
            struct nlist* tempNode = hashtab[j];
            while(tempNode != NULL) {
                struct nlist* nextNode = tempNode->next;
                free(tempNode);
                tempNode = nextNode;
            }
            hashtab[j] = NULL;
        }

    }

    // Execute one command per child
    else {
        return executeCommand(commands, i);
    }

    return 0;
}

int executeCommand(char **commands, int index) {
    char fileName[MAX_LENGTH];

    // Prepare log files
    sprintf(fileName, "%d.out", getpid());
    int fd_out = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);
    FILE* out = fopen(fileName, "a");
    sprintf(fileName, "%d.err", getpid());
    int fd_err = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);
    FILE* err = fopen(fileName, "a");

    if(dup2(fd_out, 1) != 1) {
        fprintf(stdout, "dup2 didn't work!");
    }
    if(dup2(fd_err, 2) != 2) {
        fprintf(stderr, "dup2 didn't work!");
    }

    fprintf(out, "Starting command %d: child %d pid of parent %d\n", index, getpid(), getppid());
    fflush(out);

    // Begin processing commands
    char* command = strdup(commands[index]);
    
    int length = strlen(command);
    char **args = (char **)calloc(length, sizeof(char *));
    int j = 0;
    args[j++] = strtok(command, " ");

    // Break up command into argument vector for execvp
    while((args[j++] = strtok(NULL, " ")) != NULL) { }

    execvp(args[0], args);
    fprintf(err, "Could not execute: %s\n", commands[index]);
    return 2;
}