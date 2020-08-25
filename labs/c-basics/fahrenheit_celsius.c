#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{   
    int fahr;
    if(argc==2){
        sscanf (argv[1],"%d",&fahr);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
    else if(argc==4){
        int end,increment;
        sscanf (argv[1],"%d",&fahr);
        sscanf (argv[2],"%d",&end);
        sscanf (argv[3],"%d",&increment);
        for (fahr; fahr <= end; fahr = fahr + increment)
	        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    }
    else{
        for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    }
    return 0;
}
