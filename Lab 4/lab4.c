#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int findPage(int *arr,int len,int page){
    for(int i=0;i<len;i++){
        if(arr[i]==page){
            return 1;
        }
    }
    return 0;
}
int findLRU(int *arr,int len,int *frames,int M,int index){
    int minIndex=INT_MAX;
    int min=INT_MAX;
    int minMinIndex=INT_MAX;

    for (int i=0;i<M;i++){
        int flag=0;
        for(int j=index;j>=0;j--){
            if(arr[j]==frames[i]){
                minIndex=j;
                if(minMinIndex>minIndex){
                    minMinIndex=minIndex;
                    min=i;
                }
                flag=1;
                break;
            }
        }
        if(!flag){
            return i;
        }
    }
    return min;
}
int indexPage(int *arr,int len,int page,int index){
    int arrVal[len];
    for(int i=0;i<len;i++){
        arrVal[i]=arr[i];
    }
    for(int i=index+1;i<len;i++){
        if(arrVal[i]==page){
            return i;
        }
    }
    return INT_MAX;
}
void printOutput(int len, int M,char pageFault[len],int outputs[len][M]){
printf("\n");
    for(int i=0;i<len;i++){
        printf("---");
    }
    printf("\n");
    for(int i=0;i<len;i++){
        printf("%3d",outputs[i][0]);
    }
        printf("\n");
    for(int i=0;i<len;i++){
        printf("%3d",outputs[i][1]);
    }

    printf("\n");
    for(int i=0;i<len;i++){
        printf("%3d",outputs[i][2]);
    }
    printf("\n");
    int faults=0;
    for(int i=0;i<len;i++){
        printf("%3c",pageFault[i]);
        if(pageFault[i]=='P'){
            faults++;
        }
    }

    printf("\n");
    for(int i=0;i<len;i++){
        printf("---");
    }
    printf("\n%3d page-faults\n",faults);
}
void calculateFIFO(int *vals,int len,int N, int M){
    printf("  ");
    int outputs[len][M];
    char pageFault[len];
    int frame[M];
    int values[len];
    for(int i=0;i<len;i++){
        values[i]=vals[i];
    }
    for(int i=0;i<M;i++){
        frame[i]=-1;
    }

    for(int i=0;i<len;i++){
        printf("%d  ",vals[i]);
        if(values[i]<=N&&findPage(frame,M,values[i])){
            for(int j=0;j<M;j++){
                outputs[i][j]=frame[j]; 
            }
            pageFault[i]=' ';            
        }
        else if(!findPage(frame,M,values[i])){
            for(int j=M-1;j>0;j--){
                frame[j]=frame[j-1];
                outputs[i][j]=frame[j]; 
            }
            frame[0]=values[i];
            outputs[i][0]=frame[0];
            pageFault[i]='P';
        }
    }
    printOutput(len,M,pageFault,outputs);
}
void calculateOptimal(int *vals,int len,int N, int M){
    printf("  ");
    int outputs[len][M];
    char pageFault[len];
    int frame[M];
    int values[len];
    for(int i=0;i<len;i++){
        values[i]=vals[i];
    }
    for(int i=0;i<M;i++){
        frame[i]=-1;
    }

    for(int i=0;i<len;i++){
        printf("%d  ",vals[i]);
        if(values[i]<=N&&findPage(frame,M,values[i])){
            for(int j=0;j<M;j++){
                outputs[i][j]=frame[j]; 
            }
            pageFault[i]=' ';            
        }
        else if(!findPage(frame,M,values[i])){
            int pageIndex=INT_MIN;
            int frameIndex=0;
                for(int k=0;k<M;k++){
                    int temp =indexPage(values,len,frame[k],i);
                    pageIndex = (pageIndex > temp) ? pageIndex : temp;
                    frameIndex = (pageIndex > temp) ? frameIndex : k;
                    if(pageIndex==INT_MAX){
                        break;
                    }
                }
            frame[frameIndex]=values[i];
            for(int j=0;j<M;j++){
                outputs[i][j]=frame[j]; 
            }
            pageFault[i]='P';
        }
    }
    printOutput(len,M,pageFault,outputs);
}
void calculateLRU(int *vals,int len,int N, int M){
    printf("  ");
    int outputs[len][M];
    char pageFault[len];
    int frame[M];
    int values[len];
    for(int i=0;i<len;i++){
        values[i]=vals[i];
    }
    for(int i=0;i<M;i++){
        frame[i]=-1;
    }

    for(int i=0;i<len;i++){
        printf("%d  ",vals[i]);
        if(values[i]<=N&&findPage(frame,M,values[i])){
            for(int j=0;j<M;j++){
                outputs[i][j]=frame[j]; 
            }
            pageFault[i]=' ';            
        }
        else if(!findPage(frame,M,values[i])){
             int pageIndex=INT_MAX;
            int frameIndex=0;
            frameIndex=findLRU(values,len,frame,M,i);
            frame[frameIndex]=values[i];
            for(int j=0;j<M;j++){
                outputs[i][j]=frame[j]; 
            }
            pageFault[i]='P';
        }
    }
    printOutput(len,M,pageFault,outputs);
}
int main(){
    char *token;
    int N = 0;
    int M = 0;
    int *valArr;
    int *start;
    int numVals = 0;
    FILE *file = fopen("sample.dat", "r");
    char *input = (char *)malloc(sizeof(char *) * 1000);
    if (file == NULL){
        printf("File not Found");
        return 0;
    }

    if (fgets(input, 50, file) != NULL){
        N = strtol(input, &input, 10);
        M = strtol(input, &input, 10);
    }
    if (fgets(input, 50, file) == NULL){
        return 0;
    }
    valArr = (int *)malloc(sizeof(int *) * strlen(input));
    token = strtok(input, " ");
    start = valArr;
    while (token != NULL && strcmp("-1", token) != 0){
        *start = strtol(token, NULL, 10);
        start++;
        numVals++;
        token = strtok(NULL, " ");
    }
    calculateFIFO(valArr,numVals,N,M);
    printf("\n");
    calculateOptimal(valArr,numVals,N,M);
    printf("\n");
    calculateLRU(valArr,numVals,N,M);
    fclose(file);

    return 0;
}