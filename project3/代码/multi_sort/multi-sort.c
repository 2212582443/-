# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>

struct range
{
    int start;
    int end;
};

int *array;
int *result;


void* quick_sort(void* r){
    struct range* rs=(struct range*) r;
    if(rs->start>=rs->end) return NULL;
    int i,j,tmp;int flag=1;
    for(i=1;i<rs->end-rs->start+1&&flag;++i){
    flag=0;
    for(j=rs->start;j<rs->end+1-i;j++)
    if(array[j+1]<array[j]){
          tmp=array[j];
          array[j]=array[j+1];
          array[j+1]=tmp;
          flag=1;
    }
    
    }
          
}


         
void* merge(void* r){
         struct range* rm=(struct range*) r;
         int left=rm->start,right=rm->end,mid=(right+left+1)/2;
         int i=left,j=mid,k=0;
         while(i<mid&&j<=right){
               if(array[i]<array[j]) {result[k++]=array[i++];}
               else {result[k++]=array[j++];}
         }
         
         while(i<mid) result[k++]=array[i++];
         while(j<=right) result[k++]=array[j++];
}
         


int main(){
    
    //input array
    printf("Please input the length of your unsorted array:");
    
    int n;
    scanf("%d",&n);

    //the unsorted array             -> array
    //the result of the sorted array -> result
    array=(int*) malloc(n*sizeof(int));
    result=(int*) malloc(n*sizeof(int));

    printf("Please input the elements to sort:\n");
    for(int i=0;i<n;++i){
        scanf("%d",&array[i]);
    }
    
    //two sort pthread
    pthread_t tid_former;//thread for sorting the former half
    pthread_t tid_later; //thread for sorting the later half
    struct range* sort_former=(struct range*) malloc(sizeof(struct range));// ranges for the former thread
    struct range* sort_later=(struct range*) malloc(sizeof(struct range)); // ranges for the later thread
    
    sort_former->start=0;
    sort_former->end=n/2-1;
    sort_later->start=n/2;
    sort_later->end=n-1;

    int ERROR[3];
    ERROR[0]=pthread_create(&tid_former,NULL,quick_sort,sort_former);
    ERROR[1]=pthread_create(&tid_later,NULL,quick_sort,sort_later);
    
    pthread_join(tid_former,NULL);
    pthread_join(tid_later,NULL);
    
    // one merge pthread
    pthread_t tid_merge;   
    struct range* sort_merge=(struct range*) malloc(sizeof(struct range));
    sort_merge->start=0;
    sort_merge->end=n-1;

    ERROR[2]=pthread_create(&tid_merge,NULL,merge,sort_merge);
    
    pthread_join(tid_merge,NULL);
    
    //misidentification
    if(ERROR[0]){
            printf("Fail to create the first sorting thread!\n");
            exit(1);
        }
     
    if(ERROR[1]){
            printf("Fail to create the second sorting thread!\n");
            exit(1);
        }
        
    if(ERROR[2]){
            printf("Fail to create the merging thread!\n");
            exit(1);
        }    

    //output result
    printf("Array after sort:\n");
    for(int i=0;i<n;++i){
        printf("%d ",result[i]);
    }
printf("\n");
    return 0;
}
