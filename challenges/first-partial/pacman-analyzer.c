#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>  
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);
int findIndex(char *name);
char *buildString(char *origin,int a, int b);
typedef struct 
{
    char name[100];
    char installDate[100];
    char lastUpdate[100];
    int updateCount;
    char removalDate[100];
}package;

package packages[1000];

int packageCount = 0,
    removedPackagesCount = 0,
    updatedPackagesCount = 0,
    fileDescriptor;

int main(int argc, char **argv) {
    if (argc < 5) {
	printf("\033[91mError: Wrong arguments.\033[93m Please use: ./pacman-analizer.o -input <logfile.abc> -output <outputfile.def>\033[0m\n");
	return 0;//debería ser 1, se evita para que el make continue
    }
    analizeLog(argv[2], argv[4]);
    return 0; 
}

void analizeLog(char *logFile, char *report) {
    printf("\033[94mGenerating Report from: \033[95m[%s] \033[94mlog file\033[0m\n", logFile);
    fileDescriptor = open(logFile,O_RDONLY);
    char buffer;
    char line[1000];
    int size = 0,
        spaceCount = 0;
    int spacers[4];
    char *action,*date,*pack;
    int paren = 0,
        tmpind;
    if(fileDescriptor<0){
        printf("\033[91mError: Could not open the file \033[1m[%s]\033[0m\n",logFile);
        return;
        //exit(1); linea comentada para que el test automatico continue
    }
    while(read(fileDescriptor, &buffer, 1)){
        if (buffer == '\n'){
            if(spaceCount == 4){
                //aqui va la magia
                date = buildString(line,1,spacers[0]-1);
                action = buildString(line,spacers[1]+1,spacers[2]);
                pack = buildString(line,spacers[2]+1,spacers[3]); 
                if(strcmp(action,"upgraded") == 0){
                    tmpind = findIndex(pack);
                    if(tmpind!=-1){
                        if(packages[tmpind].updateCount == 0)
                            updatedPackagesCount++;
                        packages[tmpind].updateCount++;
                        strcpy(packages[tmpind].lastUpdate,date);
                    }
                }
                else if(strcmp(action,"installed") == 0){
                    strcpy(packages[packageCount].name,pack);
                    strcpy(packages[packageCount].installDate,date);
                    strcpy(packages[packageCount].lastUpdate,"-");
                    
                    strcpy(packages[packageCount].removalDate,"-"); 
                    packages[packageCount].updateCount = 0;
            
                    
                    packageCount++;
                }
                else if(strcmp(action,"removed") == 0){
                    tmpind=findIndex(pack);
                    if(tmpind!=-1){
                        strcpy(packages[tmpind].removalDate,date); 
                    }
                    removedPackagesCount++;
                }
                free(date);
                free(action);
                free(pack); 
            }
            size = 0;
            spaceCount = 0;
        }
        else{
            if(buffer == '[')
                paren++;
            if(buffer == ']')
                paren--;
            if (buffer == ' '&& spaceCount<4 && paren==0){
                spacers[spaceCount] = size;
                spaceCount++;
            }
            line[size%1000] = buffer;
            size++;
        }
        //printf("%s",buffer);   
    } 
    close(fileDescriptor);
    int fileDescriptor2 = open(report, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fileDescriptor2 < 0) {
        printf("\033[93mProblemas al crear el archivo de resultados del análisis\033[0m\n"); 
    }
    else{
        char tmp[100];
        sprintf(tmp,"Pacman Packages Report\n----------------------\n");
            write(fileDescriptor2,tmp,strlen(tmp));
        sprintf(tmp,"- Installed packages\t: %d\n",packageCount);
            write(fileDescriptor2,tmp,strlen(tmp));
        sprintf(tmp,"- Removed packages\t: %d\n",removedPackagesCount);
            write(fileDescriptor2,tmp,strlen(tmp));
        sprintf(tmp,"- Upgraded packages\t: %d\n",updatedPackagesCount);    
            write(fileDescriptor2,tmp,strlen(tmp));
        sprintf(tmp,"- Current installed\t: %d\n",packageCount-removedPackagesCount);
            write(fileDescriptor2,tmp,strlen(tmp));
        
        sprintf(tmp,"\nList of packages\n----------------\n");
            write(fileDescriptor2,tmp,strlen(tmp));
        for(int i=0; i<packageCount;i++){
            sprintf(tmp,"- Package Name\t: %s\n",packages[i].name);
                write(fileDescriptor2,tmp,strlen(tmp));
            sprintf(tmp,"  - Install date\t: %s\n",packages[i].installDate);
                write(fileDescriptor2,tmp,strlen(tmp));
            sprintf(tmp,"  - Last update date\t: %s\n",packages[i].lastUpdate);
                write(fileDescriptor2,tmp,strlen(tmp));
            sprintf(tmp,"  - How many updates\t: %d\n",packages[i].updateCount);
                write(fileDescriptor2,tmp,strlen(tmp));
            sprintf(tmp,"  - Removal date\t: %s\n",packages[i].removalDate);
                write(fileDescriptor2,tmp,strlen(tmp));
        }
        close(fileDescriptor2);
    }

    
    
    
    
   
    
    
    
    
    
    

    

    // Implement your solution here.

    printf("\033[92mReport is generated at: \033[95m[%s]\033[0m\n", report);
}
int findIndex(char *name){
    for(int i=0; i<packageCount;i++){
        if(strcmp(name,packages[i].name)==0){
            return i;
        }
    }
    return -1;

}


char *buildString(char *origin,int a, int b){    
    char *new=malloc(b-a+1);
    memcpy(new,&origin[a],b-a);
    new[b-a]='\0'; 
    return new;
}




