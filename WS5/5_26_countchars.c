#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 51

int main(void) {
   int count = 0;
   char input[MAX_LENGTH];
   memset(input, 0, sizeof(input));

   fgets(input, MAX_LENGTH, stdin);

   for(int i = 0; input[i] != '\0'; ++i) {
      if(input[i] == ' ' || input[i] == '!' || input[i] == '.' || input[i] == ',') {continue;}
      ++count;
   }

   fprintf(stdout, "%d\n", count);

   return 0;
}
