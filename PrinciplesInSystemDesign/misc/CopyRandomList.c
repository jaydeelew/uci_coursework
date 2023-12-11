#include <stdlib.h> 
#include <stdio.h>

struct Node {
    int val;
    struct Node *next;
    struct Node *random;
};

struct Node* copyRandomList(struct Node* head) {
    if(head == NULL)
        return head;

    struct Node *curr = head;    

    while(curr!= NULL) {
        // create copies on the heap of all nodes passed into copyRandomList() via head
        struct Node *temp = (struct Node*) malloc(sizeof(struct Node));
        temp->val = curr->val;
        temp->next = curr->next;  // the temp node and curr will both point to curr's next node
        temp->random = curr->random;
        curr->next = temp;  // temp node is inserted between copied node and copied node's original next node
        curr = temp->next;  // curr pointer is now set to next original node or NULL
    }

    curr = head->next;  // curr now points to the first copied node on the linked list

    // this loop sets random pointers of copied & interweaved nodes to point to corresponding copied nodes
    while (curr != NULL){
        struct Node *temp = curr;  
        if(temp->random != NULL)
            temp->random = temp->random->next;  // have temp, this copied node, point to random's next node(corresp copied node)
        if(curr->next != NULL)
            curr = curr->next->next;  // move curr pointer to next copied node
        else
            break;
    }
   
    struct Node *copy = NULL;
    struct Node *copyHead = NULL;
    curr = head;  // curr now points to head of original linked list
    copy = head->next;  // copy now points to head of copied linked list
    copyHead = copy;

    //  this loop sets all next pointers of original and copied nodes to their next nodes respectively
    while(curr != NULL){   
        // last original node points to its copy which points to NULL
        // so when the last copy points to NULL, we have last original node point to NULL
        if(copy->next == NULL){ 
            curr->next = NULL;
            break;
        }
        curr->next = copy->next;  // curr leap frogs over copy to next original node
        copy->next = curr->next->next;  // copy leap frogs over next original node to next copy
        curr = curr->next;
        copy = copy->next;
    }
    
    return copyHead;
}

void printLinkedList(struct Node* head){
    struct Node *currNode = head;
    while(currNode != NULL){
        int randomVal = -1;
        if(currNode->random != NULL)
            randomVal = currNode->random->val;
        printf("%d %d  ", currNode->val, randomVal);
        currNode = currNode->next;
    }
    printf("\n");
}

int main(){
    struct Node *one = (struct Node*) malloc(sizeof(struct Node));
    struct Node *ten = (struct Node*) malloc(sizeof(struct Node));
    struct Node *eleven = (struct Node*) malloc(sizeof(struct Node));
    struct Node *thirteen = (struct Node*) malloc(sizeof(struct Node));
    struct Node *seven = (struct Node*) malloc(sizeof(struct Node));
    one->val = 1;
    one->next = NULL;
    one->random = seven;
    ten->val = 10;
    ten->next = one;
    ten->random = eleven;
    eleven->val = 11;
    eleven->next = ten;
    eleven->random = one;
    thirteen->val = 13;
    thirteen->next = eleven;
    thirteen->random = seven;
    // head
    seven->val = 7;
    seven->next = thirteen;
    seven->random = NULL;

    printLinkedList(seven);  // original linked list
    printLinkedList(copyRandomList(seven));  // copied linked list
}