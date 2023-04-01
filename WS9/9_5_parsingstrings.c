#include<stdio.h>
#include <string.h>

#define MAX_LENGTH 50

int main(void) {
   char userInput[MAX_LENGTH], firstWord[MAX_LENGTH], secondWord[MAX_LENGTH];
   
   while(1) {
      memset(firstWord, 0, sizeof(firstWord));
      memset(secondWord, 0, sizeof(secondWord));
      
      fprintf(stdout, "Enter input string:\n");
      fgets(userInput, MAX_LENGTH, stdin);
      if(userInput[0] == 'q') { break; }
      
      int length = strlen(userInput);
      int i = 0, j = 0, charFound = 0;
      char *toFill = firstWord;
      for(; i < length; ++i) {
         if(userInput[i] == ',') {
            charFound = 1;
            toFill = secondWord;
            j = 0;
         }
         else if(userInput[i] != ' ') {
            toFill[j] = userInput[i];
            ++j;
         }
      }
      if(charFound) {
         fprintf(stdout, "First word: %s\nSecond word: %s\n",firstWord, secondWord);
      }
      else {
         fprintf(stdout, "Error: No comma in string.\n\n");
      }
   }

   return 0;
}