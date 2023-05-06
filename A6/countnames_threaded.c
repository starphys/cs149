/**
 * Description: This module counts the recurrences of unique strings in any number of files concurrently using threads.
 * Author names: Evan Astle
 * Author emails: david.astle@sjsu.edu
 * Last modified date: 05/05/2023
 * Creation date: 05/05/2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define HASHSIZE 101
#define MAX_NAMES 100
#define MAX_LENGTH 31

//thread mutex lock for access to the log index
pthread_mutex_t tlock_log = PTHREAD_MUTEX_INITIALIZER;

//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock_data = PTHREAD_MUTEX_INITIALIZER;

//thread mutex lock for access to the name counts data structure
pthread_mutex_t tlock_insert = PTHREAD_MUTEX_INITIALIZER;

void* thread_runner(void*);

pthread_t tid1, tid2;

//struct points to the thread that created the object.
//This is useful for you to know which is thread1. Later thread1 will also deallocate.
struct THREADDATA_STRUCT {
    pthread_t creator;
};

typedef struct THREADDATA_STRUCT THREADDATA;
THREADDATA* p = NULL;

//variable for indexing of messages by the logging function.
int logindex=0;
int *logip = &logindex;

void logprint(char* message) {
    // variables to store date and time components
    int hours, minutes, seconds, day, month, year;

    // time_t is arithmetic time type
    time_t now;

    // Obtain current time
    // time() returns the current time of the system as a time_t value
    time(&now);

    // localtime converts a time_t value to calendar time and
    // returns a pointer to a tm structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);
    hours = local->tm_hour; // get hours since midnight (0-23)
    minutes = local->tm_min; // get minutes passed after the hour (0-59)
    seconds = local->tm_sec; // get seconds passed after minute (0-59)
    day = local->tm_mday; // get day of month (1 to 31)
    month = local->tm_mon + 1; // get month of year (0 to 11)
    year = local->tm_year + 1900; // get year since 1900

    pthread_mutex_lock(&tlock_log);
    if (hours < 12) // before midday
        fprintf(stdout, "Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d am: %s\n", 
        ++logindex, pthread_self(), getpid(), day, month, year, hours, minutes, seconds, message);
    else // after midday
        fprintf(stdout, "Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d pm: %s\n", 
        ++logindex, pthread_self(), getpid(), day, month, year, hours - 12, minutes, seconds, message);
    pthread_mutex_unlock(&tlock_log);
}

struct node {
    int count;              // instances so far
    char* name;             // name being counted
    struct node *next;     // next entry in chain 
    pthread_mutex_t lock;   // each node has individual lock for thread safety
};


static struct node *hashtab[HASHSIZE]; /* pointer table */

/**
 * @brief hashing function that hashes based on names
 * 
 * @param s name to be hashed
 * @return unsigned index in hash table
 */
unsigned hash(char* s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }

    return hashval % HASHSIZE;
}

/**
 * @brief Searches hashtable for an entry that corresponds to name.
 * 
 * @param name to be searched for
 * @return struct node* if found, else NULL
 */
struct node *lookup(char* name)
{
    struct node *np;
    for (np = hashtab[hash(name)]; np != NULL; np = np->next)
        if (strcmp(name, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/**
 * @brief atomically search and insert a new node.
 * 
 * @param name the name to be added
 * @return struct node* that was added, or NULL
 */
struct node *insert(char* name)
{
    struct node *np = NULL;
    unsigned hashval;

    // Only insert non existent nodes
    pthread_mutex_lock(&tlock_insert);    
    if ((np = lookup(name)) == NULL) { 
        np = (struct node *) malloc(sizeof(struct node));
        if (np == NULL) { return NULL; }
        np->name = strdup(name);
        np->count = 1;
        pthread_mutex_init(&(np->lock), NULL);
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else {
        pthread_mutex_lock(&(np->lock));
        np->count++;
        pthread_mutex_unlock(&(np->lock));
    }
    pthread_mutex_unlock(&tlock_insert);

    return np;
}
/**
 * @brief Lookup node and increment if found, otherwise add a new node.
 * Note, because insert performs an atomic lookup and insert, we will never create
 * an extra node with the same name.
 * @param name 
 * @return struct node* if found or successfully inserted, otherwise NULL
 * If NULL is returned, caller may reattempt incrementing this name. 
 */
struct node* increment(char* name) {
    struct node* toInc = NULL;
    toInc = lookup(name);
    // Found, increment!
    if(toInc != NULL) {
        pthread_mutex_lock(&(toInc->lock));
        toInc->count++;
        pthread_mutex_unlock(&(toInc->lock));
        return toInc;
    }
    
    // Not found, insert!
    return insert(name);

}

/**
 * @brief delete a list given a valid pointer to its head
 * NOT thread safe, only perform after threads have joined.
 * @param head pointer to first node in list.
 */
void deleteList(struct node* head) {
    struct node* current = head;
    while(current != NULL) {
        struct node* temp = current->next;
        free(current->name);
        free(current);
        current = temp;
    }
}

/**
 * @brief print a list given a valid pointer to its head
 * NOT thread safe, only perform after threads have joined.
 * @param head pointer to first node in list.
 */
void printList(struct node* head) {
    struct node* current = head;
    while(current != NULL) {
        fprintf(stdout,"%s. %d\n",current->name,current->count);
        current = current->next;
    }
}

/*********************************************************
// function main
*********************************************************/
int main(int argc, char* argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Program requires exactly two file names.\n");
        pthread_exit(NULL);
    }

    printf("==================== Log Messages ====================\n");

    printf("create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,argv[1]);

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,argv[2]);

    printf("wait for first thread to exit\n");
    pthread_join(tid1,NULL);
    printf("first thread exited\n");

    printf("wait for second thread to exit\n");
    pthread_join(tid2,NULL);
    printf("second thread exited\n");

    printf("==================== Name Counts ====================\n");
    //TODO print out the sum variable with the sum of all the numbers

    // Free all nodes in hashtable
    for(int j = 0; j < HASHSIZE; j++) {
        printList(hashtab[j]);
        deleteList(hashtab[j]);
        hashtab[j] = NULL;
    }
    
    exit(0);
}//end main

/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void* thread_runner(void* f)
{
    char* filename = (char *) f;
    pthread_t me;
    char buffer[100];
    me = pthread_self();
    sprintf(buffer, "This is thread %ld (p=%p)",me,p);
    logprint(buffer);

    pthread_mutex_lock(&tlock_data); // critical section starts
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator = me;
    }

    if (p != NULL && p->creator == me) {
        sprintf(buffer, "This is thread %ld and I created THREADDATA %p",me,p);
        logprint(buffer);
    } else {
        sprintf(buffer, "This is thread %ld and I can access the THREADDATA %p",me,p);
        logprint(buffer);
    }
    pthread_mutex_unlock(&tlock_data); // critical section ends

    FILE* inFile = fopen(filename, "r");
    sprintf(buffer, "opened file %s", filename);
    logprint(buffer);

    if(!inFile) {
        sprintf(buffer, "range: cannot open file\n");
        logprint(buffer);       
        pthread_exit(NULL);
    }

    // Track which line we are on
    int i = 0;

    // Read line by line, discard blank lines, compare non-blank lines to existing array
    while(true) {
        i++;

        // Prepare temp string for writing
        char* input = NULL;
        size_t length;
        
        // Watch for errors and EOF
        if(getline(&input, &length, inFile) == -1) {
            free(input);
            break;
        }

        // Check blank lines
        if(input[0] == '\n' || (input[0] == ' ' && (input[1] == '\n' || input[1] == '\0'))) {
            fprintf(stderr, "Warning - file %s line %d is empty.\n", filename, i);
            free(input);
            continue;
        }

        // Cleanup non-blank read
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        // Include this name in count
        // TODO: this is a stupid way to do this, don't do this.
        while(increment(input) == NULL) {}
        free(input);
    }
        
    // Something went wrong while reading
    if(ferror(inFile))
    {
        pthread_exit(NULL);
    }

    // Successfully read the entire file
    fclose(inFile);

    // critical section starts
    pthread_mutex_lock(&tlock_data);
    if (p!=NULL && p->creator==me) {
        sprintf(buffer, "This is thread %ld and I delete THREADDATA",me);
        logprint(buffer);     
        free(p);
        p = NULL;
    } else {
        sprintf(buffer, "This is thread %ld and I can access the THREADDATA",me);
        logprint(buffer);    
    }
    pthread_mutex_unlock(&tlock_data);

    pthread_exit(NULL);
    //return NULL;
}//end thread_runner