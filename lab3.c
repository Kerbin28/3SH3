#include <stdio.h>
#include <stdlib.h>
//struct declarations
struct Node{
    int space;
    // 0 for free, 1 for inuse
    int address;
    int free;
    struct Node * next;
    struct Node * prev;
    int PID;
};
//variable declarations
int freeMemory=0;
int maxPID =0;
int MINBLOCK = 1024*4;
//function declarations
void allocate(int memory,struct Node * addr){
    struct Node * node = addr;
    while( node->next!=NULL&&!(!(node->free)&&node->space>memory)){
        node=node->next;
    }
    node->free=1;
    freeMemory-=memory;
    struct Node * newNode=node+1;
    newNode->prev=node;
    newNode->free=0;
    newNode->PID=++maxPID;
    newNode->address=node->address+memory;
    newNode->next= node->next;
    newNode->space= node->space-memory;
    node->space=memory;
    node->next=newNode;
}
void release(){

}

void compact(){

}
void status(){
    
}

int main(){
    float MAX_SPACE = 0;
    struct Node *head = (struct Node *)malloc(sizeof (struct Node));
    printf("Space in MB? ");
    scanf("%f",&MAX_SPACE);
    MAX_SPACE=(int)(MAX_SPACE*1048576);
    freeMemory=MAX_SPACE;
    struct Node * spaceAddress = (struct Node *)malloc(sizeof(struct Node)+MAX_SPACE);
    head->prev=NULL;
    head->next=spaceAddress;
    head->PID=-1;
    head->free=-1;
    head->address=-1;
    spaceAddress->prev=head;
    spaceAddress->next=NULL;
    spaceAddress->PID=0;
    spaceAddress->free=0;
    spaceAddress->address=0;
    spaceAddress->space=MAX_SPACE;
    allocate(MINBLOCK,spaceAddress);
    allocate(MINBLOCK*10,spaceAddress);
    allocate(MINBLOCK*2,spaceAddress);
    allocate(MINBLOCK*5,spaceAddress);
    printf("Done\n");
    return 0;
}