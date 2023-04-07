#ifndef ITEM_TO_PURCHASE_H
#define ITEM_TO_PURCHASE_H

#define MAX_LENGTH 50

typedef struct ItemToPurchase {
   int itemPrice;
   int itemQuantity;
   char itemName[MAX_LENGTH];
} ItemToPurchase;

void MakeItemBlank(ItemToPurchase* item);
void PrintItemCost(ItemToPurchase item);

#endif