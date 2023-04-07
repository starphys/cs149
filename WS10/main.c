#include<stdio.h>
#include<string.h>

#include "ItemToPurchase.h"

int main(void) {
   ItemToPurchase first, second;
   char input[MAX_LENGTH];
   int length = 0;
   fprintf(stdout, "Item 1\nEnter the item name:\n");
   fgets(first.itemName, MAX_LENGTH, stdin);
   length = strlen(first.itemName);
   if(first.itemName[length-1] == '\n') { first.itemName[length-1] = '\0'; }
   
   fprintf(stdout, "Enter the item price:\n");
   fgets(input, MAX_LENGTH, stdin);
   sscanf(input, "%d", &(first.itemPrice));
   
   fprintf(stdout, "Enter the item quantity:\n");
   fgets(input, MAX_LENGTH, stdin);
   sscanf(input, "%d", &(first.itemQuantity));
   
   fprintf(stdout, "\nItem 2\nEnter the item name:\n");
   fgets(second.itemName, MAX_LENGTH, stdin);
   length = strlen(second.itemName);
   if(second.itemName[length-1] == '\n') { second.itemName[length-1] = '\0'; }
   
   fprintf(stdout, "Enter the item price:\n");
   fgets(input, MAX_LENGTH, stdin);
   sscanf(input, "%d", &(second.itemPrice));
   
   fprintf(stdout, "Enter the item quantity:\n");
   fgets(input, MAX_LENGTH, stdin);
   sscanf(input, "%d", &(second.itemQuantity));


   fprintf(stdout, "\nTOTAL COST\n");
   PrintItemCost(first);
   PrintItemCost(second);
   
   fprintf(stdout, "\nTotal: $%d\n", first.itemPrice*first.itemQuantity + second.itemPrice*second.itemQuantity);
   
   return 0;
}