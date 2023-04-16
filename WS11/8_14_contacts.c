#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct ContactNode_struct ContactNode;

typedef struct ContactNode_struct {
   char* contactName;
   char* contactPhoneNumber;
   ContactNode* nextNodePtr;
} ContactNode;

void InitializeContactNode(ContactNode* newNode, char* name, char* number) {
   newNode->contactName = name;
   newNode->contactPhoneNumber = number;
   newNode->nextNodePtr = NULL;
}
   
char* GetName(ContactNode *node) {
   return node->contactName;   
}
char* GetPhoneNumber(ContactNode *node) {
   return node->contactPhoneNumber;   
}

void InsertAfter(ContactNode* prev, ContactNode* toInsert) {
   ContactNode* temp = prev->nextNodePtr;
   prev->nextNodePtr = toInsert;
   toInsert->nextNodePtr = temp;
} 

ContactNode* GetNext(ContactNode *node) {
   return node->nextNodePtr;   
}
void PrintContactNode(ContactNode node) {
   fprintf(stdout,"Name: %s\nPhone number: %s\n", node.contactName, node.contactPhoneNumber);   
}

int main(void) {
   ContactNode* head = NULL, *tail = NULL;
   
   // Get each node and add it to the list
   for(int i = 0; i < 3; ++i) {
      char* name = (char*)malloc(51*sizeof(char));
      char* number = (char*)malloc(51*sizeof(char));
      
      // Get name
      fgets(name, 51, stdin);
      int length = strlen(name);
      if(name[length-1] == '\n') {
         name[length-1] = '\0';
      }
      
      // Get number
      fgets(number, 51, stdin);
      length = strlen(number);
      if(number[length-1] == '\n') {
         number[length-1] = '\0';
      }
      
      // Make and insert new node
      ContactNode* temp = (ContactNode*)malloc(sizeof(ContactNode));
      InitializeContactNode(temp, name, number);
      
      if(head == NULL) {
         tail = head = temp;
      }
      else {
         InsertAfter(tail, temp);
         tail = temp;
      }
      
      // Print new input
      fprintf(stdout, "Person %d: %s, %s\n", i+1, name, number);
   }
   
   // Print list
   ContactNode* current = head;
   fprintf(stdout, "\nCONTACT LIST\n");
   while(current) {
      PrintContactNode(*current);
      current = current->nextNodePtr;
      fprintf(stdout, "\n");
   }
   
   return 0;
}
