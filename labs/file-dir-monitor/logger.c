#include <stdio.h>
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <time.h>


//COLOR DEFINITION
#define PURPLE "\033[95m"
#define BLUE "\033[94m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET "\033[0m"
#define LOGGER "AdvancedLogger"


int toStd=1;//1 es stdout, 0 es syslog

char* getTime();
char buff[100];


int initLogger(char *logType) {
    printf("Initializing Logger on: %s\n", logType);
    if(strcmp(logType,"stdout")==0 || strcmp(logType,"")==0){
        toStd=1;
        return 0;
    }
    else if(strcmp(logType,"syslog")==0){
        toStd=0;
        return 0;
    }

    printf("Wrong args to init logger. guess i die");
    exit(1);
}

int infof(const char *format, ...) {
    va_list arg;
    va_start (arg, format);  
    
    if(toStd){
        printf(PURPLE);
        getTime();
        printf("Logger - %s: ",buff);
        vprintf (format, arg);
        printf("\n%s",RESET);
        
    }
    else{
        openlog(LOGGER, LOG_PID, LOG_USER);
        vsyslog(LOG_INFO,format,arg);
        closelog();
    }
     return 0;
}

int warnf(const char *format, ...) {

    va_list arg;
    va_start (arg, format);  
    
    if(toStd){
        printf(YELLOW);
        getTime();
        printf("Logger - %s: ",buff);
        vprintf (format, arg);
        printf("\n%s",RESET);
    }
    else{
        openlog(LOGGER, LOG_PID, LOG_USER);
        vsyslog(LOG_WARNING,format,arg);
        closelog();
    }
     return 0;
}

int errorf(const char *format, ...) {
    
    va_list arg;
    va_start (arg, format);  
    
    if(toStd){
        printf(RED);
        getTime();
        printf("Logger - %s: ",buff);
        vprintf (format, arg);
        printf("\n%s",RESET);
    }
    else{
        openlog(LOGGER, LOG_PID, LOG_USER);
        vsyslog(LOG_ERR,format,arg);
        closelog();
    }
    return 0;
}
int panicf(const char *format, ...) {

    va_list arg;
    va_start (arg, format);  
    
    if(toStd){
        printf(RED);printf(BOLD);
        getTime();
        printf("Logger - %s: ",buff);
        vprintf (format, arg);
        printf("\n%s",RESET);
    }
    else{
        openlog(LOGGER, LOG_PID, LOG_USER);
        vsyslog(LOG_EMERG,format,arg);
        closelog();
    }
     return 0;
}


/* int main(){
    infof("Esto es una prueba. numero: %d, char: %c , cadena: %s\n",69879,'k',"Probando");
    return 0;
} */
char* getTime(){
    time_t now = time (0);
    strftime (buff, 100, "%m-%Y-%d %H:%M:%S.000", localtime (&now));
}