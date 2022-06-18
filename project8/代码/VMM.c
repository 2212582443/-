# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define MEMORY_SIZE 256

typedef struct TLB_ITEM
{
    int RU;
    int page_number;
    int frame_number;
} TLB_item;
TLB_item TLB[TLB_SIZE];

typedef struct PAGE_TABLE_ITEM
{
 int vaild;
 int frame_number;
} page_table_item;
page_table_item page_table[PAGE_TABLE_SIZE];

typedef struct MEMORY_ITEM
{
  int RU;
  char frame[FRAME_SIZE];
} memory_item;
memory_item memory[MEMORY_SIZE];

int TLB_hit,Page_fault,count;

FILE *input,*output,*backing_store;

void init();
int frame_number_access(int page_number);
void TLB_replace(int page_number);
void page_fault_solve(int page_number);
int memory_access(int phy_add);

int main(int argc, char *argv[]) 
{	

	if(argc!=2) 
	{
	  printf("Please input correct address file name.\n");
	  return -1;
	}
	input = fopen(argv[1], "r");
	output = fopen("result.txt", "w");
	

	init();
		
	int log_add,phy_add,page_number, offset, frame_number, value;
	while(~fscanf(input, "%d", &log_add)) 
	{
		++ count;
		log_add = log_add & 0x0000ffff;
		
		page_number = (log_add >> 8) & 0x000000ff;
		offset = log_add & 0x000000ff;
		
		if(page_number<0||page_number>=PAGE_TABLE_SIZE) 
		{
                  fprintf(output,"Page number out of bounds.\n");
                  continue;
                }
                if(offset<0||offset>=FRAME_SIZE)
                {
                  fprintf(output,"Offset address out of bounds.\n");
                  continue;
                }
                
		frame_number = frame_number_access(page_number);
		
		phy_add=(frame_number << 8) + offset;
		value = memory_access(phy_add);
		
		fprintf(output, "Virtual address: %d Physical address: %d Value: %d\n", log_add,phy_add, value);
	}
	
	printf("The memory size (frame number) is %d\n",MEMORY_SIZE);
	double TLB_hit_rate=100.0 * TLB_hit / count;
	double Page_fault_rate=100.0 * Page_fault / count;
	printf("TLB hit rate is: %.1f %%\nPage fault rate is: %.1f %%\n", TLB_hit_rate, Page_fault_rate);
	
	fclose(input);
	fclose(output);
	fclose(backing_store);

	return 0;
}


void init()
{
  for(int i=0;i<TLB_SIZE;i++)
  {
    TLB[i].RU=-1;
    TLB[i].page_number=-1;
    TLB[i].frame_number=-1;
  }
  
  for(int i=0;i<PAGE_TABLE_SIZE;i++)
  {
    page_table[i].vaild=0;
    page_table[i].frame_number=-1;
  }
  
  for(int i=0;i<MEMORY_SIZE;i++)  memory[i].RU=-1;
  
  TLB_hit=0;
  Page_fault=0;
  count=0;
  
  backing_store = fopen("BACKING_STORE.bin", "rb");
}

int frame_number_access(int page_number)
{
 
 for(int i=0;i<TLB_SIZE;i++)
 {
   if(TLB[i].page_number==page_number)
   {
     TLB[i].RU=count;
     TLB_hit++;
     return TLB[i].frame_number;
   }
 } 
 
 if(page_table[page_number].vaild) 
 {
 TLB_replace(page_number);
 return page_table[page_number].frame_number;
 }
 else{  
   page_fault_solve(page_number);
   Page_fault++;
   return frame_number_access(page_number);
 }
}

void TLB_replace(int page_number)
{
  int earlist=count,LRU;
  for(int i=0;i<TLB_SIZE;i++)
  {
    if(TLB[i].RU<earlist)
    { 
      earlist=TLB[i].RU;
      LRU=i;
    }
  }
  
  TLB[LRU].page_number=page_number;
  TLB[LRU].frame_number=page_table[page_number].frame_number;
  TLB[LRU].RU=count;
}

void page_fault_solve(int page_number)
{
  int earlist=count,LRU;
  for(int i=0;i<MEMORY_SIZE;i++)
  {
    if(memory[i].RU<earlist)
    {
      earlist=memory[i].RU;
      LRU=i;
    }
  }
  
  memory[LRU].RU=count;
  fseek(backing_store, page_number*PAGE_SIZE, SEEK_SET);
  fread(memory[LRU].frame, sizeof(char), FRAME_SIZE, backing_store);
  
  for(int i=0;i<PAGE_TABLE_SIZE;i++)
  {
    if(page_table[i].frame_number==LRU)
    {
      page_table[i].vaild=0;
      break;
    }
  }
  
  page_table[page_number].frame_number=LRU;
  page_table[page_number].vaild=1;

}

int memory_access(int phy_add)
{
  int frame_number=(phy_add >> 8) & 0x000000ff;
  int offset=phy_add & 0x000000ff;
  
  memory[frame_number].RU=count;
  
  return (int)memory[frame_number].frame[offset];
}
