#include <stdio.h>

int main(){
    int c;
    int buffer[3000];
    int pointer=0;
    do{
        c=getchar();
        buffer[pointer]=c;
        if(c==10){//enter pressed
            for(int i=1; i<=pointer; i++){
                putchar(buffer[pointer-i]);
            }
            putchar(10);
            pointer=0;
            
        }
        else{
            
            pointer++;
        }

    }while(c!=EOF);
    return 0;
}
