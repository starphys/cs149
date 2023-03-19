#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 52

int CalcNumCharacters(char* userString, char userChar);

int main(void) {
   char userString[MAX_LENGTH], userChar;
   int result = 0;
   
   fgets(userString, MAX_LENGTH, stdin);
   userChar = userString[0];
   
   result = CalcNumCharacters(&(userString[2]), userChar);
   
   if(result == 1) {
      fprintf(stdout, "%d %c\n", result, userChar);
   }
   else {
      fprintf(stdout, "%d %c's\n", result, userChar);
   }

   return 0;
}

int CalcNumCharacters(char* userString, char userChar) {
   int count = 0;
   
   for(int i = 0; i < strlen(userString); ++i) {
      if(userString[i] == userChar) {
         ++count;
      }
   }
   
   return count;
}