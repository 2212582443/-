/*
Name: Yaoxuan Cheng
Student ID: 520021910431
*/

#include "cachelab.h"
#include <getopt.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

typedef struct CACHE_LINE
{
    int valid;
    int tag;
    int RU;
}cache_line;
cache_line **cache;

FILE *input;
int verbose,s,S,E,b,B,hits,misses,evictions,time;

void init();
void memory_access(char operation,unsigned int address,int size);

int main(int argc, char*argv[])
{
    verbose=0,s=-1,S=-1,E=-1,b=-1,B=-1;
    char trace_name[50];
    
    int ch;
    opterr=0;
    while ((ch = getopt(argc,argv,"hvs:E:b:t:"))!= -1)
    {
        
        switch (ch)
        {
        case 'h':
            printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
                   "-h: Optional help flag that prints usage info\n"
                   "-s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
                   "-E <E>: Associativity (number of lines per set)\n"
                   "-b <b>: Number of block bits (B = 2^b is the block size)\n"
                   "-t <tracefile>: Name of the valgrindtrace to replay\n");
            break;
        case 'v': 
            verbose=1;
            break;

        case 's':
            s=atoi(optarg);
            S=1<<s;
            break;

        case 'E':
            E=atoi(optarg);
            break;

        case 'b':
            b=atoi(optarg);
            B=1<<b;
            break;

        case 't':
            strcpy(trace_name, optarg);
            break;

        default:
            printf("Please input the correct command format.\n");
            printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
                   "-h: Optional help flag that prints usage info\n"
                   "-s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
                   "-E <E>: Associativity (number of lines per set)\n"
                   "-b <b>: Number of block bits (B = 2^b is the block size)\n"
                   "-t <tracefile>: Name of the valgrindtrace to replay\n");
            return -1;
        }
    }
    if(S<0||E<0||B<0) return -1;
    input=fopen(trace_name,"r");
    if(input==NULL) 
    {
      printf("Can't find the corresponding trace file.\n");
      return-1;
    }
    
    init();
    
   char operation;
   unsigned int address;
   int size;
   
   while(fscanf(input," %c %x,%d",&operation,&address,&size)!=EOF) memory_access(operation,address,size);
   
   printSummary(hits, misses, evictions);
    
   for(int i=0; i<S; ++i)  free(cache[i]);
   free(cache);
   fclose(input);
   
   return 0;
}


void init()
{
  hits=0;
  misses=0;
  evictions=0;
  time=0;
  
  cache = (cache_line**)malloc(sizeof(cache_line*) * S);
  for(int i=0; i<S; i++)
  {
    cache[i] = (cache_line*)malloc(sizeof(cache_line) * E);
    for(int j=0; j<E; j++)
    {
      cache[i][j].valid = 0;
      cache[i][j].tag = -1;
      cache[i][j].RU = -1;
    }      
  }
}

void memory_access(char operation,unsigned int address,int size)
{
   if(operation!='L'&&operation!='S'&&operation!='M') return;
   int tag = (address>>(b+s));
   int set = (address>>b) & ((0xFFFFFFFF)>>(64-s));
   
   int if_hit=0;
   int if_eviction=1;
   
   for(int i=0;i<E;i++)
   {
     if(cache[set][i].tag==tag&&cache[set][i].valid)
     {
       if_hit=1;
       hits++;
       cache[set][i].RU=time;
       break;
     }
   }
   
   if(!if_hit)
   {
     misses++;
     for(int i=0;i<E;i++)
     {
       if(!cache[set][i].valid)
       {
         cache[set][i].valid=1;
         cache[set][i].tag=tag;
         cache[set][i].RU=time;
         if_eviction=0;
         break;
       }
     }
     
     if(if_eviction)
     {
       evictions++;
       int earliest_time=time;
       int earliest_lines;
       for(int i=0;i<E;i++)
       {
         if(cache[set][i].RU<earliest_time)
         {
           earliest_time=cache[set][i].RU;
           earliest_lines=i;
         }
       }
       cache[set][earliest_lines].valid=1;
       cache[set][earliest_lines].tag=tag;
       cache[set][earliest_lines].RU=time;
       
     }
     
   }
   
   if(operation=='M') hits++;
   
   time++;
   
   if(verbose)
   {
     switch(operation)
     {
     case 'L':
         printf("%c %x,%d ",operation,address,size);
         
         if(if_hit)
         {
           printf("hit\n");
           break;
         }else printf("miss");
         
         if(if_eviction) printf(" eviction\n");
         else printf("\n");
         break;
         
     case 'S':
         printf("%c %x,%d ",operation,address,size);
         
         if(if_hit)
         {
           printf("hit\n");
           break;
         }else printf("miss");
         
         if(if_eviction) printf(" eviction\n");
         else printf("\n");
         break;
         
     case 'M':
         printf("%c %x,%d ",operation,address,size);
         
         if(if_hit)
         {
           printf("hit\n");
           break;
         }else printf("miss");
         
         if(if_eviction) printf(" eviction hit\n");
         else printf(" hit\n");
         break;
         
     default:break;
         
     }
   }
}
