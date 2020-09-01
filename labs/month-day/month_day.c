#include <stdio.h>
#include <stdbool.h>

/* month_day function's prototype*/
void month_day(int *year, int yearday, int *pmonth, int *pday);
bool leap(int year);

int main(int argc, char **argv) {
    char *monthnames[]={"Jan","Feb","Mar","Apr","May","June",
                    "July","Aug","Sept","Oct","Nov","Dec"};
    int year,yearday,pmonth,pday;
    sscanf(argv[1],"%d",&year);
    sscanf(argv[2],"%d",&yearday);
    month_day(&year,yearday,&pmonth,&pday);
    printf("%4s %2d, %d \n",monthnames[pmonth],pday,year);
    return 0;
}

bool leap(int year){
    return year%4==0 && (year%100!=0 ||year%400==0);
}
void month_day(int *year, int yearday, int *pmonth, int *pday){
    
    int monthdays[]={31,28,31,30,31,30,31,31,30,31,30,31};
    while(yearday>365){
        if(leap(*year)&&yearday==366)
            break;
        if(leap(*year)){
            *year+=1;
            yearday-=366;
        }
        else{
            *year+=1;
            yearday-=365;
        }

    }
    
    if(leap(*year)){
        monthdays[1]=29;
    }
    for(int i=0; i<12;i++){
        if(yearday>monthdays[i]){
            yearday-=monthdays[i];
        }
        else{
            *pmonth=i;
            *pday=yearday;
            break;
        }
    }
}

