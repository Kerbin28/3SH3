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
int maxPID =0;
int MINBLOCK = 1024*4;
//function declarations
void allocate(int memory,struct Node * addr){
    struct Node * node = addr;
    while( node->next!=NULL){
        if(!(node->free)&&node->space>memory){
            break;
        }
        node=node->next;
    }
    node->free=1;
    struct Node * newNode=(struct Node *)((char *)node+memory);
    newNode->prev=node;
    newNode->free=0;
    newNode->PID=++maxPID;
    newNode->address=node->address+memory;
    newNode->next= node->next;
    newNode->space= node->space-memory;
    node->space=memory;
    node->next=newNode;
}
void release(int PID,struct Node * addr){
    struct Node * node = addr;
    while(node!=NULL){
        if(node->PID==PID){
            node->free=0;
        }
        node=node->next;
    }
}

void compact(struct Node *addr){
    struct Node * node = addr;
    struct Node * prev = NULL;
    
    int freemem=0;

    while(node!=NULL){
        if(!(node->free)){
            /*
            if(node->prev!=NULL)
                node->prev->next=node->next;
            if(node->next!=NULL){
                node->next->prev=node->prev;
                node->next->address=node->address;
            }
            */
            if(node->next!=NULL){
                node->next=node->next->next;
                if(node->next->next!=NULL){
                    node->next->next->address=node->next->address;
                    node->next=node->next->next;
                    node->next->prev=node;
                }
            }
            freemem += node->space;                      
        }
        prev=node;
        node=node->next;
        
    }   
    struct Node * freeSpace = (struct Node *)((char *)prev+prev->space);
    prev->next=freeSpace;
    freeSpace->free=0;
    freeSpace->next=NULL;
    freeSpace->prev=prev;
    freeSpace->address=prev->address+prev->space;
    freeSpace->PID=++maxPID;
    freeSpace->space=freemem;

}
void status(struct Node *addr){
    struct Node * node = addr;
    while(node!=NULL){
        printf("PID: %d, Address: %d, UseMemory: %d, Process Running: %d\n"
        ,node->PID,node->address,node->space,node->free);
        node=node->next;
    }
}

int main(){
    float MAX_SPACE = 0;
    struct Node *head = (struct Node *)malloc(sizeof (struct Node));
    printf("Space in MB? ");
    scanf("%f",&MAX_SPACE);
    MAX_SPACE=(int)(MAX_SPACE*1048576);
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
    allocate(MINBLOCK,head);
    allocate(MINBLOCK*10,head);
    allocate(MINBLOCK*2,head);
    allocate(MINBLOCK*5,head);
    status(head);
    printf("----------\n");
    release(1,head);
    status(head);
    allocate(MINBLOCK*12,head);
    printf("----------\n");
    status(head);
    release(3,head);
    printf("----------\n");
    status(head);
    compact(head);
    printf("----------\n");
    status(head);
    printf("Done\n");
    return 0;
}