#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include <pthread.h>

long * readMatrix(char *filename);
long * getColumn(int col, long *matrix);
long * getRow(int row, long *matrix);
int getLock();
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long * multiply(long *matA, long *matB);
int saveResultMatrix(long *result);

int NUM_BUFFERS;
char* RESULT_MATRIX_FILE;
int MATRIX_DIMENSION=2000;

long **buffers;
pthread_mutex_t *mutex;
pthread_t thread[2000];

typedef struct {
    int row;
    int col;
    long *matrixA;
    long *matrixB;

}args;

int main(int argc, char **argv){
    initLogger("stdout");
    if(argc<5){
        errorf("Wrong arguments");
        exit(1);
    }
    else{
        for(int i=1; i<argc;i++){
            if(strcmp(argv[i],"-n")==0){
                i++;
                if(i>=argc){
                    errorf("Wrong arguments");
                    exit(1);
                }
                NUM_BUFFERS=atoi(argv[i]);
            }
            else if(strcmp(argv[i],"-out")==0){
                i++;
                if(i>=argc){
                    errorf("Wrong arguments");
                    exit(1);
                }
                RESULT_MATRIX_FILE=argv[i];
            }
        }
    }

    buffers=malloc(NUM_BUFFERS*sizeof(long *));
    mutex=malloc(NUM_BUFFERS*sizeof(pthread_mutex_t));
    long *matrixA=readMatrix("matA.dat");
    infof("Finished Reading A\n");
    long *matrixB=readMatrix("matB.dat");
    infof("Finished Reading B\n");
    for(int i = 0; i < NUM_BUFFERS; i++){
        pthread_mutex_init(&mutex[i], NULL);
    }

    
    long *result = multiply(matrixA,matrixB);
    infof("Finished multiplication\n");
    saveResultMatrix(result);
    free(matrixA);
    free(matrixB);
    free(mutex);
    free(buffers);
    return 0;
    
}
long multiplicationFragment(args *arg){
    int buffer1 = -1,
        buffer2 = -1;
    while(buffer1 == -1 || buffer2 ==-1){
        if(buffer1 == -1){
            buffer1 = getLock();
        }
        if(buffer2 == -1){
            buffer2 = getLock();
        }
    }
    buffers[buffer1] = getRow(arg->row,arg->matrixA);
    buffers[buffer2] = getColumn(arg->col,arg->matrixB);
    long result = dotProduct(buffers[buffer1],buffers[buffer2]);
    free(buffers[buffer1]);
    free(buffers[buffer2]);
    free(arg);
    releaseLock(buffer1);
    releaseLock(buffer2);
    return result;
}

long * multiply(long *matA, long *matB){
    infof("Starting multiplication. This may take several minutes\n");
    long *result= malloc(MATRIX_DIMENSION*MATRIX_DIMENSION*sizeof(long));
    for(int i=0; i<MATRIX_DIMENSION; i++){
        for(int j=0;j<MATRIX_DIMENSION;j++){
            //pthread_create(&thread[j],NULL,)
            args *ar=malloc(sizeof(args));
            ar -> row = i;
            ar -> col = j;
            ar -> matrixA = matA;
            ar -> matrixB = matB;
            pthread_create(&thread[j],NULL,(void * (*) (void *))multiplicationFragment,(void *)ar);
        }
        for(int j=0; j<MATRIX_DIMENSION; j++){
            void *res;
            pthread_join(thread[j], &res);
            result[MATRIX_DIMENSION * j + i] = (long) res;
        }
    }
    return result;
}
long * readMatrix( char *filename){
    int size=0;
    FILE *f =fopen(filename,"r");
    if(f==NULL){
        errorf("Error opening the Matrix data file");
        exit(2);
    }
    char c;
    while((c=fgetc(f))!=EOF){
        if(c=='\n')
            size++;
    }
    rewind(f);
    long *matrix=malloc(size*sizeof(long));
    int index=0;
    while(fscanf(f,"%ld",&matrix[index])!= EOF){
        index++;
    }
    fclose(f);
    return matrix;   
}

long * getRow(int row, long *matrix){
    long *r=malloc(MATRIX_DIMENSION*sizeof(long));
    int rowStart=MATRIX_DIMENSION*(row);
    for(int i=0;i<MATRIX_DIMENSION;i++){
        r[i]=matrix[rowStart+i];
    }
    return r;
}

long * getColumn(int column, long *matrix){
    long *c=malloc(MATRIX_DIMENSION*sizeof(long));
    for(int i=0;i<MATRIX_DIMENSION;i++){
        c[i]=matrix[column];
        column+=MATRIX_DIMENSION;
    }
    return c;
}

int getLock() {
    for(int i = 0; i < NUM_BUFFERS; i++){
        if(pthread_mutex_trylock(&mutex[i])==0){
            return i;
        }   
    }
    return -1;
}
int releaseLock(int lock) {
    return pthread_mutex_unlock(&mutex[lock]);
}
long dotProduct(long *vec1, long *vec2){
    long result = 0;
    for(int i=0; i<MATRIX_DIMENSION; i++){
        result+=vec1[i]*vec2[i];
    }
    return result;
}

int saveResultMatrix(long *result) {    
    FILE *file;
    file = fopen(RESULT_MATRIX_FILE, "w+");
    if(file==NULL){
        return -1;
    }
    int size=MATRIX_DIMENSION*MATRIX_DIMENSION;
    for(int i = 0; i < size; i++) {
        fprintf(file, "%ld\n", result[i]);
    }
    fclose(file);
    return 0;
}