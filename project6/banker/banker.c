#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int security_detection()
{
   int work[NUMBER_OF_RESOURCES];
   for(int i=0;i<NUMBER_OF_RESOURCES;i++) work[i]=available[i];
   
   int finish[NUMBER_OF_CUSTOMERS];
   for(int i=0;i<NUMBER_OF_CUSTOMERS;i++) finish[i]=0;
   
   for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
   {
     if(!finish[i])
     {
       int j;
       for(j=0;j<NUMBER_OF_RESOURCES;j++)
       {
         if(need[i][j]>work[j]) break;
       }
       if(j==NUMBER_OF_RESOURCES)
       {
         finish[i]=1;
         for(int k=0;k<NUMBER_OF_RESOURCES;k++) work[k]+=allocation[i][k];
         i=-1;
       }
      
     }
   
   }
   
   int j;
   for(j=0;j<NUMBER_OF_CUSTOMERS;j++) 
   {
     if(!finish[j]) break;
   }
   
   if(j==NUMBER_OF_CUSTOMERS) return 0;
   else return -1;



}

int request_resources(int customer_num, int request[])
{
  for(int i=0;i<NUMBER_OF_RESOURCES;i++)
  {
      if(request[i]>need[customer_num][i])
      {
        printf("The requested resources exceeds the actual required resources.\n");
        return -1;
      }
      if(request[i]>available[i])
      {
        printf("The requested resources exceeds the available resources.\n");
        return -1;
      }
  }
  
  for(int i=0;i<NUMBER_OF_RESOURCES;i++)
  {
      available[i]-=request[i];
      allocation[customer_num][i]+=request[i];
      need[customer_num][i]-=request[i];
  }
  
  if(security_detection()==-1)
  {
    printf("This request will make unsafe state!\n");
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
    {
      available[i]+=request[i];
      allocation[customer_num][i]-=request[i];
      need[customer_num][i]+=request[i];
    }
    return -1;
  }else{
    printf("This request is satisfied.\n");
    return 0;
  }
}

int release_resources(int customer_num, int release[])
{
  for(int i=0;i<NUMBER_OF_RESOURCES;++i)
  {
        if(allocation[customer_num][i]<release[i])
        {
            printf("The R%d to be released are more than the R%d that can be released,\n so only the remaining R%d are released.\n",i,i,i);
            available[i]+=allocation[customer_num][i];
            allocation[customer_num][i]=0;    
        }
        else
        {
            printf("The R%d are released normally\n",i);
            available[i]+=release[i];
            allocation[customer_num][i]-=release[i];
        }
  }
}

void display_all()
{
  printf("Customers   Allocation        Max            Need         Available\n");
  printf("            R1 R2 R3 R4    R1 R2 R3 R4    R1 R2 R3 R4    R1 R2 R3 R4\n");
  for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
  {
    printf("P%d          ",i);
    for(int j=0;j<NUMBER_OF_RESOURCES;j++) printf("%d  ",allocation[i][j]);
    printf("   ");
    for(int j=0;j<NUMBER_OF_RESOURCES;j++) printf("%d  ",maximum[i][j]);
    printf("   ");
    for(int j=0;j<NUMBER_OF_RESOURCES;j++) printf("%d  ",need[i][j]);
    if(i==0)
    {
    printf("   ");
    for(int j=0;j<NUMBER_OF_RESOURCES;j++) printf("%d  ",available[j]);
    }
    
    printf("\n");
  
  }

}



int main(int argc,char* argv[])
{
  if(argc!=5)
  {
     printf("Please input the number of resources of 4 type.\n");
     return 1;
  }
  for(int i=0;i<NUMBER_OF_RESOURCES;i++)  available[i]=atoi(argv[i+1]);
  
  FILE *max_req;
  max_req=fopen("max_request.txt","r");

  if(max_req==NULL)
  {
     printf("Can't find the input file.\n");
     return 2;
  }
 
  //init
   char *maximum_i=(char*) malloc(10*sizeof(char));
   for(int i=0;i<NUMBER_OF_CUSTOMERS;i++)
   {
     if(fgets(maximum_i,10,max_req)==NULL||maximum_i[0]>'9'||maximum_i[0]<'0') break;
     
     char *temp=strdup(maximum_i);
     for(int j=0;j<NUMBER_OF_RESOURCES;j++)
     {
        maximum[i][j]=atoi(strsep(&temp,","));
        allocation[i][j] = 0;
        need[i][j] = maximum[i][j]; 
     }
   }
   free(maximum_i);
   
   //command
   char command[50];
   int customer_num;
   int resources[NUMBER_OF_RESOURCES]={0};
   while(1)
   {
        printf("Banker>>");
        //read command
        fgets(command,50,stdin);
        if(command[0]=='e') break;
        
        if(command[0]=='*')
        {
            display_all();
            continue;
        }
        
        //read data
        char *temp=strdup(command);
        char *command_name=strsep(&temp," ");
        customer_num=atoi(strsep(&temp," "));//printf("%d\n",customer_num);
        
        for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        {
          char *tmp=strsep(&temp," ");
          if(tmp==NULL)
          {
            printf("Please input the correct number of resource requests.\n");
            break;
          }
          resources[i]=atoi(tmp);
          //printf("%d\n",resources[i]);
        }
        
        if(command[0]=='R'&&command[1]=='Q')
        {
             request_resources(customer_num, resources);
        }
        else if(command[0]=='R'&&command[1]=='L')
        {
             release_resources(customer_num, resources);
        }
        else{
            printf("Wrong command entered.\n");
        }
   }
   
   return 0;
   
}
