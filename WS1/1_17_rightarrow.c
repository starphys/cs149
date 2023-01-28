#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )  {

   if( argc != 3 ) {
      printf("Two arguments expected.\n");
      return 0;
    }

   int baseInt = atoi(argv[1]);
   int headInt = atoi(argv[2]);
   
   printf("%5d\n", headInt);
   printf("%5d%d\n", headInt, headInt);
   printf("%d%d%d%d%d%d%d\n", baseInt, baseInt, baseInt, baseInt, headInt, headInt, headInt);
   printf("%d%d%d%d%d%d%d%d\n", baseInt, baseInt, baseInt, baseInt, headInt, headInt, headInt, headInt);
   printf("%d%d%d%d%d%d%d\n", baseInt, baseInt, baseInt, baseInt, headInt, headInt, headInt);
   printf("%5d%d\n", headInt, headInt);
   printf("%5d\n", headInt);

   return 0;
}
