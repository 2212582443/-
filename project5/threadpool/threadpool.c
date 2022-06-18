/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;
task task_queue[QUEUE_SIZE];

//the threads pool
pthread_t pool[NUMBER_OF_THREADS];

//the mutex lock of pool
pthread_mutex_t queue_mutex;

// work signal for threads;
int work[NUMBER_OF_THREADS];

// sem for thread
sem_t thread_sem;

int queue_size=0;
// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    if(queue_size<QUEUE_SIZE)
    {
      pthread_mutex_lock(&queue_mutex);
      
      task_queue[queue_size]=t;
      queue_size++;
      
      pthread_mutex_unlock(&queue_mutex);
      
      return 0;
    }else {
    printf("Enqueue Error\n");
    return 1;
    }
}

// remove a task from the queue
task dequeue() 
{
    if(queue_size==0)
    {
     worktodo.data=NULL;
     worktodo.function=NULL;
     printf("Dequeue Error\n");
    }else{
     pthread_mutex_lock(&queue_mutex);
     
      worktodo=task_queue[0];
      queue_size--;
      for(int i=0;i<queue_size;i++) task_queue[i]=task_queue[i+1];
      task_queue[queue_size].data=NULL;
      task_queue[queue_size].function=NULL;
      
     pthread_mutex_unlock(&queue_mutex);
    }
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    int *p=(int*) param;
    int id=*p;
    worktodo=dequeue();
    printf("Execute the %u thread:\n", id);
    execute(worktodo.function, worktodo.data);
    
    //finish task
    work[id]=0;
    sem_post(&thread_sem);
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;
    
    int s=enqueue(worktodo);
    int id=0;
    if(!s)
    {
     sem_wait(&thread_sem);
     while(1)
     {
     if(!work[id])
     {
     work[id]=1;
     break;
     }else{
     id=(id+1)%NUMBER_OF_THREADS;
     }
     }
    } else{
        printf("Pool Submit Error\n");
        return 1;
    }
    
    pthread_create(&pool[id],NULL,worker,&id);

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    queue_size=0;
    
    for(int i=0;i<NUMBER_OF_THREADS;i++) work[i]=0;
    
    sem_init(&thread_sem,0,NUMBER_OF_THREADS);
    
    pthread_mutex_init(&queue_mutex,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for(int i=0;i<NUMBER_OF_THREADS;i++) pthread_join(pool[i],NULL);
    sem_destroy(&thread_sem);
    pthread_mutex_destroy(&queue_mutex);
}
