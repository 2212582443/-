#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"        //define a function "run" which print the current running task 
#include "list.h"       //a linked list where adding and deleting occur in the head of it.
#include "schedulers.h" //define two functions "add" and "schedule", which should be completed in this file
#include "task.h"       //include struct Task

int total_waiting_time=0;
int total_response_time=0;
int total_trunaround_time=0;
int number_of_tasks=0;
int current_time=0;
int value=0;

struct node** head_first_in_head=NULL;

void add(char *name, int priority, int burst)
{
    //initialize the head
    if(head_first_in_head==NULL)
    {
        head_first_in_head=malloc(sizeof(struct node* ));
        (*head_first_in_head)=NULL;
    }

    //initialize a new task
    Task* newtask=(Task*) malloc(sizeof(Task));
    newtask->name=strdup(name);
    newtask->priority=priority;
    newtask->burst=burst;
    newtask->tid=__sync_fetch_and_add(&value,1);

    //insert the task to list
    insert(head_first_in_head,newtask);
    number_of_tasks++;
}

/*
The link is one where adding occurs in the head, so the first-coming task occurs in the tail of list.
*/
int schedule_single_task()
{
    struct node* temp=(*head_first_in_head);
    struct node* prev_of_temp=temp;
    struct node* shortest_job=temp;
    struct node* prev_of_sj=NULL;
    
    //find the shortest job
    while(temp->next!=NULL)
    {
        if(temp->task->burst<shortest_job->task->burst)
        {
             shortest_job=temp;
             prev_of_sj=prev_of_temp;
        }
        prev_of_temp=temp;
        temp=temp->next;
    }
    
    if(temp->task->burst<shortest_job->task->burst)
    {
             shortest_job=temp;
             prev_of_sj=prev_of_temp;
    }
    //stimulation of running the task which is in the tail of list
    //counting associated information(waitingtime, turnaround time, response time)
    total_waiting_time+=current_time;
    total_response_time+=current_time;    

    run(shortest_job->task,shortest_job->task->burst);

    current_time+=shortest_job->task->burst;
    total_trunaround_time+=current_time;
    
    //delete the shorest job
    if(shortest_job==(*head_first_in_head))
         (*head_first_in_head)=(*head_first_in_head)->next;
    else
         prev_of_sj->next=shortest_job->next;
         
    //Terminate judgment
    if((*head_first_in_head)==NULL)
        return 1;
    else
        return 0;
    
    
}

void schedule()
{
    while(!schedule_single_task());
    printf("Average turnaround time of the SJF is %.3lf\n",(double)total_trunaround_time/(double)number_of_tasks);
    printf("Average waiting time of the SJF is %.3lf\n",(double)total_waiting_time/(double)number_of_tasks);
    printf("Average response time of the SJF is %.3lf\n",(double)total_response_time/(double)number_of_tasks);
    
}
