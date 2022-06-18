#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

/* mutex and semaphore */
pthread_mutex_t buffer_mutex;
sem_t full;
sem_t empty;


void *producer(void *param) {
    buffer_item item;
    while (1) {
    /* sleep for a random period of time */
    int sleep_time = rand()%10;
    sleep(sleep_time);

    /* generate a random number */
    sem_wait(&empty);
    
    pthread_mutex_lock(&buffer_mutex);
    
    item = rand();
    if (insert_item(item))
        printf("producer error condition\n");
    else
        printf("producer produced %d\n",item);
        
    pthread_mutex_unlock(&buffer_mutex);
    
    sem_post(&full);
    }
}

void *consumer(void *param) 
{ 
     buffer_item item;
    while (1) {
    /* sleep for a random period of time */
    int sleep_time = rand()%10;
    sleep(sleep_time);


    sem_wait(&full);
    
    pthread_mutex_lock(&buffer_mutex);
     
    if (remove_item(&item))
        printf("consumer error condition\n");
    else
        printf("consumer consumed %d\n",item);
        
    pthread_mutex_unlock(&buffer_mutex);
    
    sem_post(&empty);
    }
}

int main(int argc, char*argv[])
{
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    if(argc!=4)
    {
        printf("Please input:\n");
        printf("1.sleep time\n");
        printf("2.number of producers\n");
        printf("3.number of consumers\n");
        return 1;
    }
    int sleep_time = atoi(argv[1]);
    int num_producer = atoi(argv[2]);
    int num_consumer = atoi(argv[3]);
   
    /* 2. Initialize buffer */
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);  
    for(int i=0; i<BUFFER_SIZE; i++)  waiting[i]=1;
    
    /* 3. Create producer thread(s) */
    pthread_t producer_thread[num_producer];
    for(int i=0; i<num_producer; i++)
    {
        pthread_create(&producer_thread[i], NULL, producer, NULL);
    }

    /* 4. Create consumer thread(s) */
    pthread_t consumer_thread[num_consumer];
    for(int i=0; i<num_consumer; i++)
    {
        pthread_create(&consumer_thread[i], NULL, consumer, NULL);
    }
   
    /* 5. Sleep */
    printf("main() sleep\n");
    sleep(sleep_time);
    printf("main() wake up\n");

    /* 6. Exit */
    sem_destroy(&full);
    sem_destroy(&empty);
    
    pthread_mutex_destroy(&buffer_mutex);
    
    for(int i=0; i<num_producer; i++) pthread_cancel(producer_thread[i]);
    for(int i=0; i<num_consumer; i++) pthread_cancel(consumer_thread[i]);
    
    //fclose(error_record);
    
    printf("Exit\n");
    return 0;
}
