#include <stdio.h>
#include <string.h>
int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);
int main(int argc,char **argv) {
    if(argc>3){
        int len1=mystrlen(argv[2]);
        int len2=mystrlen(argv[3]);
        if(strcmp(argv[1],"-add") == 0){
            printf("Initial length\t: %d\n",mystrlen(argv[2]));
            char *newStr=mystradd(argv[2],argv[3]);
            printf("New String\t: %s\n" ,newStr);
            printf("New length\t: %d\n",mystrlen(newStr));
        }
        else if(strcmp(argv[1],"-find") == 0){
            int ind=mystrfind(argv[2],argv[3]);
            if(ind<0){
                printf("String ['%s'] was not found.\n",argv[3]);
            }
            else{
                printf("['%s'] string was found at [%d] position\n",argv[3],ind);
            }
        }
        else{
            printf("Wrong parameters\n");
        }

    }
    else{
        printf("Wrong parameters\n");
    }
    printf("\n");
    return 0;
}
