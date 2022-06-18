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
int total_running_time=0;
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
    total_running_time+=burst;
    newtask->tid=__sync_fetch_and_add(&value,1);
    newtask->first_ask=1;

    //insert the task to list
    insert(head_first_in_head,newtask);
    number_of_tasks++;
}

void resort_priority()
{
    struct node* temp=(*head_first_in_head)->next;
    struct node* prev=(*head_first_in_head);
    struct node* pprev=(*head_first_in_head);
    int flag=1;
    for(int i=1;i<number_of_tasks&&flag;i++){
         flag=0;
         temp=(*head_first_in_head)->next;
         prev=(*head_first_in_head);
         pprev=(*head_first_in_head);
         for(int j=0;j<number_of_tasks-i;j++)
         {
             if(prev->task->priority>temp->task->priority)
             {
                 if(prev==(*head_first_in_head))
                 {
                    prev->next=temp->next;
                    temp->next=prev;
                    (*head_first_in_head)=temp;
                    temp=(*head_first_in_head)->next;
                    prev=(*head_first_in_head);
                 }
                 else
                 {
                    prev->next=temp->next;
                    temp->next=prev;
                    pprev->next=temp;
                    prev=temp;
                    temp=temp->next;
                 }
                 
                 flag=1;
             }
             pprev=prev;
             prev=temp;
             temp=temp->next;
         }
    }     
}

/*
The link is one where adding occurs in the head, so the first-coming task occurs in the tail of list.
*/
int schedule_single_task()
{
    struct node* temp=(*head_first_in_head);
    struct node* prev=temp;
    while(temp->next!=NULL)
    {
        prev=temp;
        temp=temp->next;
    }

    //stimulation of running the task which is in the tail of list
    //counting associated information(waitingtime, turnaround time, response time)

   if(temp->task->burst>QUANTUM)
   {
        if(temp->task->first_ask)
        {
           total_response_time+=current_time;
           temp->task->first_ask=0;
        }
        
        run(temp->task,QUANTUM);
        temp->task->burst-=QUANTUM;
        current_time+=QUANTUM;
        
        temp->next=(*head_first_in_head);
        (*head_first_in_head)=temp;
        prev->next=NULL;
   }
   else
   {
        if(temp->task->first_ask)
        {
           total_response_time+=current_time;
           temp->task->first_ask=0;
        }
        
        run(temp->task,temp->task->burst);
        current_time+=temp->task->burst;
        total_trunaround_time+=current_time;
        
        if((*head_first_in_head)->next==NULL) (*head_first_in_head)=NULL;
        
        prev->next=NULL;
   
   }
    
    //Terminate judgment
    if((*head_first_in_head)==NULL)
        return 1;
    else
        return 0;
}



void schedule()
{
    resort_priority();
    while(!schedule_single_task());
    total_waiting_time=total_trunaround_time-total_running_time;
    printf("Average turnaround time of the Priority with RR Scheduling is %.3lf\n",(double)total_trunaround_time/(double)number_of_tasks);
    printf("Average waiting time of the Priority with RR Scheduling is %.3lf\n",(double)total_waiting_time/(double)number_of_tasks);
    printf("Average response time of the Priority with RR Scheduling is %.3lf\n",(double)total_response_time/(double)number_of_tasks);
    
}
