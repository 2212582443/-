#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *memory;
int memory_size;

void c()
{
  int hole_begin=0,hole_end=-1;
  for(int i=0;i<memory_size;i++)
  {
    if(i==0&&memory[0]==-1) hole_begin=0;
    else if(memory[i]==-1&&memory[i-1]!=-1) hole_begin=i;
    
    if(i==memory_size-1&&memory[memory_size-1]==-1) hole_end=memory_size;
    else if(memory[i]==-1&&memory[i+1]!=-1) hole_end=i;
    
    if(hole_begin<=hole_end)
    {
     int j;
     int hole_size=hole_end-hole_begin+1;
     for(j=hole_end+1;j<memory_size;j++)
     {
       if(memory[j]==-1) break;
       memory[j-hole_size]=memory[j];
       memory[j]=-1;
     }
     hole_begin=j;
     i=j;
    }
  
  }
}

void state()
{
   int begin=0,end;
   for(int i=0;i<memory_size;i++)
   {
     if(memory[begin]!=memory[i])
     {
       end=i-1;
       
       if(memory[begin]!=-1)
       printf("Addresses [%d,%d] Process P%d\n",begin,end,memory[begin]);
       else
       printf("Addresses [%d,%d] Unused\n",begin,end);
       
       begin=i;
     }
   }
   if(memory[begin]!=-1)
   printf("Addresses [%d,%d] Process P%d\n",begin,memory_size-1,memory[begin]);
   else
   printf("Addresses [%d,%d] Unused\n",begin,memory_size-1);

}

void first_fit(int program,int size)
{
  int hole_begin=0,hole_end=-1,success=0;
  for(int i=0;i<memory_size;i++)
  {
    if(i==0&&memory[0]==-1) hole_begin=0;
    else if(memory[i]==-1&&memory[i-1]!=-1) hole_begin=i;
    
    if(i==memory_size-1&&memory[memory_size-1]==-1) hole_end=memory_size;
    else if(memory[i]==-1&&memory[i+1]!=-1) hole_end=i;
    
    if(hole_begin<=hole_end)
    {
     int hole_size=hole_end-hole_begin+1;
     if(hole_size<size)
     {
       hole_begin=hole_end+1;
       i=hole_begin;
     }
     else
     {
      for(int j=hole_begin;j<hole_begin+size;j++) memory[j]=program;
      success=1;
      break;
     }
     
    }
  
  }
  
  if(success) printf("Memory application succeeded.\n");
  else printf("Memory application failed.\n");
}

void worst_fit(int program,int size)
{
  int hole_begin=0,hole_end=-1;
  int max_begin=0,max_size=0;
  for(int i=0;i<memory_size;i++)
  {
    if(i==0&&memory[0]==-1) hole_begin=0;
    else if(memory[i]==-1&&memory[i-1]!=-1) hole_begin=i;
    
    if(i==memory_size-1&&memory[memory_size-1]==-1) hole_end=memory_size;
    else if(memory[i]==-1&&memory[i+1]!=-1) hole_end=i;
    
    if(hole_begin<=hole_end)
    {
     int hole_size=hole_end-hole_begin+1;
     if(hole_size>max_size)
     {
       max_size=hole_size;
       max_begin=hole_begin;
       hole_begin=hole_end+1;
       i=hole_begin;
     }
    }
  
  }
  
  if(max_size<size) printf("Memory application failed.\n");
  else{
    for(int i=max_begin;i<max_begin+size;i++) memory[i]=program;
    printf("Memory application succeeded.\n");
  }
}

void best_fit(int program,int size)
{
  int hole_begin=0,hole_end=-1;
  int min_begin=0,min_size=memory_size;
  for(int i=0;i<memory_size;i++)
  {
    if(i==0&&memory[0]==-1) hole_begin=0;
    else if(memory[i]==-1&&memory[i-1]!=-1) hole_begin=i;
    
    if(i==memory_size-1&&memory[memory_size-1]==-1) hole_end=memory_size;
    else if(memory[i]==-1&&memory[i+1]!=-1) hole_end=i;
    
    if(hole_begin<=hole_end)
    {
     int hole_size=hole_end-hole_begin+1;
     if(hole_size>=size&&hole_size<min_size)
     {
       min_size=hole_size;
       min_begin=hole_begin;
       hole_begin=hole_end+1;
       i=hole_begin;
     }
    }
  
  }
  if(min_size==memory_size) printf("Memory application failed.\n");
  else{
    for(int i=min_begin;i<min_begin+size;i++) memory[i]=program;
    printf("Memory application succeeded.\n");
  }
  
}

void request_memory(int program,int size,int fit)
{
  if(size>memory_size)
  {
    printf("Memory is not enough.\n");
    return;
  }
  switch(fit)
  {
   case 0:first_fit(program,size);break;
   case 1:worst_fit(program,size);break;
   case 2:best_fit(program,size);break;
   default:printf("Wrong fit input.\n");
  }
}

void release_memory(int program)
{
  int f=0;
  for(int i=0;i<memory_size;i++)
  {
   if(memory[i]==program) 
   {  memory[i]=-1;
      f=1;}
  }
  
  if(f) printf("The Process P%d has been released.\n",program);
  else printf("Can't find the Process P%d in memory.\n",program);
}

int main(int argc,char* argv[])
{
  if(argc!=2)
  {
     printf("Please input the memory capacity.\n");
     return 1;
  }
  memory_size=atoi(argv[1]);
  memory=(int*) malloc(sizeof(int)*memory_size);
  for(int i=0;i<memory_size;i++) memory[i]=-1;
  
   
   //command
   
   while(1)
   {
        printf("allocator>");
        //read command
        char *command=(char*)malloc(50*sizeof(char));
        fgets(command,50,stdin);
        
        
        if(command[0]=='E') break;
        
        if(command[0]=='C')
        {
            c();
            continue;
        }
        if(command[0]=='S')
        {
            state();
            continue;
        }
        
        int p,s,f=-1;
        if(command[0]=='R'&&command[1]=='Q')
        {
              char * c=strsep(&command," ");
              char * name=strsep(&command," ");
              int size=atoi(strsep(&command," "));
              char * strategy=strsep(&command," ");
              strcpy(name,name+1);
              p=atoi(name);
              s=size;
              
              if(strategy[0]=='F') f=0;
              else if(strategy[0]=='W') f=1;
              else if(strategy[0]=='B') f=2;
              
              request_memory(p,s,f);
              continue;
        }else if(command[0]=='R'&&command[1]=='L')
        {
             char * c=strsep(&command," ");
             char * name=strsep(&command," ");
             strcpy(name,name+1);
             p=atoi(name);
             
             release_memory(p);
             continue;
        }else{
            printf("Wrong command entered.\n");
            continue;
        }
   }
   
   
   return 0;
   
}
