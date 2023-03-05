#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 51

void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]);

int main(void) {
    char input[MAX_LENGTH], output[MAX_LENGTH];
    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));


    fgets(input, MAX_LENGTH, stdin);

    RemoveNonAlpha(input, output);

    fprintf(stdout, "%s\n", output);

    return 0;
}

void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]) {
    int j = -1;

    for(int i = 0; userString[i] != '\0'; ++i) {
        if(!isalpha(userString[i])) {continue;}
        userStringAlphaOnly[++j] = userString[i];
    }
    userStringAlphaOnly[++j] = '\0';
}
