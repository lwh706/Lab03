#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#ifndef NUM_THREADS
#define NUM_THREADS 2
#endif

//文件指標
FILE *fptr;
//創建互斥鎖
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
//全域變數
char* filename = "test_員工編號.txt";
int sharedValue = 0;
char tmpStr[20];    

void* myFile(void* threadid){
    int i;
    int islock = 0;
    for(i=0; i < 10; i++){
        islock = pthread_mutex_lock(&myMutex);
        if (islock == 0){                                           //如果成功上鎖
            int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC);    //開檔
            if (fd < 0){
                printf("open error\n");
                exit(1);
            }
            read(fd, tmpStr, strlen(tmpStr));                       //讀檔
            sharedValue = atoi(tmpStr);                             //a->i字元轉成整數
            sharedValue = sharedValue + 10;
            sprintf(tmpStr, "%d", sharedValue);                     //a->i整數轉成字元
            write(fd, tmpStr, strlen(tmpStr));                      //寫檔
            close(fd);                                              //關檔
            pthread_mutex_unlock(&myMutex);                         //解鎖
        }
    }
    pthread_exit(NULL);
}

int main(){
    //建檔並存入0
    fptr = fopen(filename, "w+");
    sprintf(tmpStr, "%d", sharedValue);
    fputs(tmpStr, fptr);
    fclose(fptr);
    // 多執行緒建立及等待
    pthread_t threads[NUM_THREADS];
    int flag;
    long t;
    for (t = 0; t < NUM_THREADS; t++){
        flag = pthread_create(&threads[t], NULL, myFile, NULL);
        if (flag != 0){
            printf("ERORR; return code from pthread_create() is %d\n", flag);
            return 0;
        }
    }
    printf("Processing start!\n");
    sleep(10);
    for (t = 0; t < NUM_THREADS; t++){
        void *retval;
        flag = pthread_join(threads[t], &retval);
        if (flag != 0) {
            printf("tid=%d wait failaure\n", NUM_THREADS);
            return 0;
        }
    }
    printf("Processing finish!\n");
    return 0;
}

//編譯指令 gcc -Wall -m64 Lab03.c -lpthread