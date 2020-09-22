#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
int comparer(char *, char *);
int strcomparer(char *a, char *b);
void mergeSort(char *[],int , int, int (*comparator)(char *, char *));
void merge(char *[],int , int, int (*comparator)(char *, char *));

int areNumbers=0;

char *data[500];

int main(int argc, char **argv){
    char *filename;
    if (argc==3){
        //numeros
        areNumbers=1;
        filename=argv[2];
    }
    else if (argc==2){
        //strings
        if(strcmp(argv[1],"-n")==0){
            printf("Missing file name\n");

            return 0;
        }
        filename=argv[1];
    }
    else{
        printf("Wrong parameters\n");
        return 0;
    }

    FILE *file;
    if((file = fopen(filename,"r"))==0){
        printf("Error: can't open %s",filename);
        return 1;
    }
    char line[50];
    int size = 0;
    while(fgets(line,50,file)){
        data[size] = (char*)malloc(strlen(line) + sizeof(char*));
        strcpy(data[size],line);
        size++;
    }
    fclose(file);
    
    mergeSort((void *)data,0,size-1,areNumbers? comparer:strcomparer);
    for (int i = 0; i < size; i++){
        printf("%s", data[i]);
    }
}

int comparer(char *a, char *b){
    int aa, bb;
    aa = atoi(a);
    bb = atoi(b);
    return aa<bb;
}
int strcomparer(char *a, char *b){
    int c=strcmp(a,b);
    if(c>0)
        return 0;
    return 1;
}

void mergeSort(char* position0[],int start, int to,int (*comparator)(char *, char *)){
    if(to>start){
        int mid=start+(to-start)/2;
        mergeSort(position0,start,mid,comparator);
        mergeSort(position0,mid+1,to,comparator);
        merge(position0,start,to,comparator);
    }
}

void merge(char* position0[],int start, int to,int (*comparator)(char *, char *)){
    char *tmp[100];
    int midpoint=start+(to-start)/2;
    int a=0,b=0;
    for(int i=0; i< to-start+1; i++){
        if( start+a <= midpoint && midpoint+1+b <=to){
            if(comparator( position0[start+a] , position0[midpoint+1+b] )){
                tmp[i]=malloc(sizeof(char*)*50);
                strcpy(tmp[i],position0[start+a]);
                a++;
            }
            else{
                tmp[i]=malloc(sizeof(char*)*50);
                strcpy(tmp[i],position0[midpoint+1+b]);
                b++;
            }
        }
         else if(start+a <= midpoint){
            tmp[i]=malloc(sizeof(char*)*50);
            strcpy(tmp[i],position0[start+a]);
            a++;
        }
        else{
            tmp[i]=malloc(sizeof(char*)*50);
            strcpy(tmp[i],position0[midpoint+1+b]);
            b++;
        }
    } 
   for(int i=0;i<to-start+1; i++){
        position0[i+start]=tmp[i];
    }
}



