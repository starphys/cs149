#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 51

int main(void) {
    char userValues[MAX_LENGTH];
    
    while(1) {
      fgets(userValues, MAX_LENGTH, stdin);
      int length = strlen(userValues);
      if(userValues[length-1] == '\n') {
         userValues[length-1] = '\0';
         --length;
      }
            
      if(strcmp(userValues, "d") == 0 || 
         strcmp(userValues, "done") == 0 ||
         strcmp(userValues, "Done") == 0) {
         break;
      }
      
      for(int j = length-1; j >= 0; --j) {
         fprintf(stdout, "%c", userValues[j]);
      }
      fprintf(stdout,"\n");
   }

   return 0;
}