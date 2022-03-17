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
char mode='f';
//function declarations
void allocate(int memory,struct Node * addr){
    
    struct Node * node = addr;
    if(mode=='f') {
    while( node->next!=NULL){
        if(!(node->free)&&node->space>memory){
            break;
        }
        node=node->next;
    }
    if (node->next!=NULL||node->space>=memory){
    if(node->space==memory){
        node->free=1;
    }
    else{
    node->free=1;
    struct Node * newNode=(struct Node *)((char *)node+memory/sizeof(struct Node*));
    
    newNode->prev=node;
    newNode->free=0;
    newNode->PID=++maxPID;
    newNode->address=node->address+memory;
    newNode->next= node->next;
    newNode->space= node->space-memory;
    node->space=memory;
    node->next=newNode;
    }
    }
    }
    else if(mode=='b'){
        struct Node *best = addr->next;
        while( node->next!=NULL){
        if(!(node->free)&&node->space>memory){
            if(node->space<best->space||best->free==1){
            best=node;
            }
        }
        node=node->next;
        }
        if(best==addr->next){
            best=node;
        }
        if(!best->free&&best->space>memory){
            
            best->free=1;
            struct Node * newNode=(struct Node *)((char *)best+memory/sizeof(struct Node*));
            newNode->next= best->next;
            newNode->prev=best;
            newNode->free=0;
            newNode->PID=++maxPID;
            newNode->address=best->address+memory;
            newNode->space= best->space-memory;
            best->space=memory;
            best->next=newNode;
        }
        else if(best->space==memory){
            best->free=1;
        }
    }
    else if(mode=='w'){
        struct Node *worst = addr->next;
        while( node->next!=NULL){
        if(!(node->free)&&node->space>memory){
            if(node->space>worst->space||worst->free==1){
            worst=node;
            }
        }
        node=node->next;
        }
        if(worst==addr->next||node->space>worst->space){
            worst=node;
        }
        if(!worst->free&&worst->space>memory){
            
            worst->free=1;
            struct Node * newNode=(struct Node *)((char *)worst+memory/sizeof(struct Node*));
            newNode->next= worst->next;
            newNode->prev=worst;
            newNode->free=0;
            newNode->PID=++maxPID;
            newNode->address=worst->address+memory;
            newNode->space= worst->space-memory;
            worst->space=memory;
            worst->next=newNode;
        }
        else if(worst->space==memory){
            worst->free=1;
        }
    }
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
            freemem += node->space;  
            if(node->next!=NULL){
                struct Node * temp = node;
                node->next->address=prev->address+prev->space;
                node=node->next;
                node->prev=prev;
                prev->next=node;
            }                    
        }
        prev=node;
        node=node->next;
        
    }  
    node = prev;
    if(!(node->free)) {
        node->address=node->prev->address+node->prev->space;
        node->space=freemem;
    }
    else{
    struct Node * freeSpace = (struct Node *)((char *)prev+prev->space/sizeof(struct Node));
    node->next=freeSpace;
    freeSpace->free=0;
    freeSpace->next=NULL;
    freeSpace->prev=node;
    freeSpace->address=node->address+node->space;
    freeSpace->PID=++maxPID;
    freeSpace->space=freemem;
    }
}
void status(struct Node *addr){
    printf("----------\n");
    struct Node * node = addr;
    if(node->address=-1&&node->next!=NULL)
        node=node->next;
    
    while(node!=NULL){
        printf("PID: %d, Address: %d, Used Memory: %d, Process Running: %d\n"
        ,node->PID,node->address,node->space,node->free);
        node=node->next;
    }
}

int main(){
    float MAX_SPACE = 0;
    struct Node *head = (struct Node *)malloc(sizeof (struct Node));
    printf("Space in MB? \n");
    scanf("%f",&MAX_SPACE);

    printf("Mode ?(f,b,w) \n");
    //scanf("%c",&mode);
    mode='w';
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
    
    release(1,head);
    status(head);
    allocate(MINBLOCK*12,head);
    
    status(head);
    release(3,head);
    status(head);
    allocate(MINBLOCK,head);
    status(head);
    
    
    status(head);
    allocate(987136,head);
    
    status(head);
    release(2,head);
    status(head);
    allocate(MINBLOCK,head);
    status(head);    
    release(4,head);
    status(head);
    allocate(MINBLOCK*3,head);
    status(head);
    compact(head);
    status(head);
    printf("Done\n");
    return 0;
}