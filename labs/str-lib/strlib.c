#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mystrlen(char *str){
    int len=0;
    while(*str!='\0'){
        *str++;
        len++;
    }
    return len;
}

char *mystradd(char *origin, char *addition){
    int lenOrigin=mystrlen(origin);
    int lenAdd=mystrlen(addition);
    char *new=malloc(lenOrigin+lenAdd+1);
    memcpy(new,origin,lenOrigin);
    memcpy(&new[lenOrigin],addition,lenAdd);
    new[lenOrigin+lenAdd]='\0'; 
    return new;
}

int mystrfind(char *origin, char *substr){
    int len=mystrlen(origin);
    int lnesub=mystrlen(substr);
    for(int i=0;i<len;i++){
        if(origin[i]==substr[0]){
            for(int j=0;j<lnesub;j++){
                if(origin[i+j]!=substr[j])
                    break;
                else if(j==lnesub-1){
                    return i;
                }
            }
        }
    }
    return -1;
}
