/*
Author: Yaoxuan Cheng
ID: 520021910431
*/


/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  if(M==N)
  {
    if(M==32)
    {
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;//11
    for(i=0; i<N; i+=8)
        for(j=0; j<M; j+=8)
            for(k=i; k<i+8; k++)
            {
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  
            }
    
    return;
    }
    else if(M==64)
    {
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;//11
    for(i=0; i<N; i+=8)
        for(j=0; j<M; j+=8)
        {
          for(k=i;k<i+4;k++)
          {
            t0 = A[k][j];
            t1 = A[k][j+1];
            t2 = A[k][j+2];
            t3 = A[k][j+3];
            t4 = A[k][j+4];
            t5 = A[k][j+5];
            t6 = A[k][j+6];
            t7 = A[k][j+7];
            
            B[j][k] = t0;
            B[j+1][k] = t1;
            B[j+2][k] = t2;
            B[j+3][k] = t3;
            
            B[j][k+4] = t4;
            B[j+1][k+4] = t5;
            B[j+2][k+4] = t6;
            B[j+3][k+4] = t7; 
          }
          
          for(k=j;k<j+4;k++)
          {
            t0 = A[i+4][k]; 
            t1 = A[i+5][k]; 
            t2 = A[i+6][k]; 
            t3 = A[i+7][k];
            
	    t4 = B[k][i+4]; 
	    t5 = B[k][i+5]; 
	    t6 = B[k][i+6]; 
	    t7 = B[k][i+7];
					
	    B[k][i+4] = t0; 
	    B[k][i+5] = t1; 
	    B[k][i+6] = t2; 
	    B[k][i+7] = t3;
	    
	    B[k+4][i] = t4; 
	    B[k+4][i+1] = t5; 
	    B[k+4][i+2] = t6; 
	    B[k+4][i+3] = t7;
          }
          
          for (k=i+4;k<i+8;k++)
	  {
	    t0 = A[k][j+4]; 
	    t1 = A[k][j+5]; 
	    t2 = A[k][j+6]; 
	    t3 = A[k][j+7];
					
	    B[j+4][k] = t0; 
	    B[j+5][k] = t1; 
	    B[j+6][k] = t2; 
	    B[j+7][k] = t3;
	  } 
        }
    }
    
  }else
  {
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;//11
    for(j=0; j<M-M%8; j+=8){
        for(i=0; i<N-N%8; i++)
        {
                t0 = A[i][j];
                t1 = A[i][j+1];
                t2 = A[i][j+2];
                t3 = A[i][j+3];
                t4 = A[i][j+4];
                t5 = A[i][j+5];
                t6 = A[i][j+6];
                t7 = A[i][j+7];

                B[j][i] = t0;
                B[j+1][i] = t1;
                B[j+2][i] = t2;
                B[j+3][i] = t3;
                B[j+4][i] = t4;
                B[j+5][i] = t5;
                B[j+6][i] = t6;
                B[j+7][i] = t7;  
            
        }
        }
        for(i=0;i<N;i++)
           for(j=M-M%8;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%8;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              }  
  }
   
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
/*
char blocking8[] = "Blocking 8 transpose";
void transpose_blocking8(int M, int N, int A[N][M], int B[M][N])
{
    for(int i=0; i<N; i+=8)
        for(int j=0; j<M; j+=8)
            for(int k=i; k<i+8; k++)
                for(int m=j; m<j+8; m++)
                    B[m][k] = A[k][m];
}

char blocking16[] = "Blocking 16 transpose";
void transpose_blocking16(int M, int N, int A[N][M], int B[M][N])
{
    for(int i=0; i<N; i+=16)
        for(int j=0; j<M; j+=16)
            for(int k=i; k<i+16; k++)
                for(int m=j; m<j+16; m++)
                    B[m][k] = A[k][m];
}

char blocking8_t[] = "Blocking 8_t transpose";
void transpose_blocking8_t(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(i=0; i<N; i+=8)
        for(j=0; j<M; j+=8)
            for(k=i; k<i+8; k++)
            {
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  
            }
}

char blocking16_t[] = "Blocking 16_t transpose";
void transpose_blocking16_t(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(i=0; i<N; i+=16)
        for(j=0; j<M; j+=16)
            for(k=i; k<i+16; k++)
            {
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  

                t0 = A[k][j+8];
                t1 = A[k][j+9];
                t2 = A[k][j+10];
                t3 = A[k][j+11];
                t4 = A[k][j+12];
                t5 = A[k][j+13];
                t6 = A[k][j+14];
                t7 = A[k][j+15];

                B[j+8][k] = t0;
                B[j+9][k] = t1;
                B[j+10][k] = t2;
                B[j+11][k] = t3;
                B[j+12][k] = t4;
                B[j+13][k] = t5;
                B[j+14][k] = t6;
                B[j+15][k] = t7;

            }
}

char blocking8_t_1[] = "Blocking 8_t_1 transpose";
void transpose_blocking8_t_1(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(i=0; i<N; i+=8)
        for(j=0; j<M; j+=8)
        {
          for(k=i;k<i+4;k++)
          {
            t0 = A[k][j];
            t1 = A[k][j+1];
            t2 = A[k][j+2];
            t3 = A[k][j+3];
            t4 = A[k][j+4];
            t5 = A[k][j+5];
            t6 = A[k][j+6];
            t7 = A[k][j+7];
            
            B[j][k] = t0;
            B[j+1][k] = t1;
            B[j+2][k] = t2;
            B[j+3][k] = t3;
            
            B[j][k+4] = t4;
            B[j+1][k+4] = t5;
            B[j+2][k+4] = t6;
            B[j+3][k+4] = t7; 
          }
          
          for(k=j;k<j+4;k++)
          {
            t0 = A[i+4][k]; 
            t1 = A[i+5][k]; 
            t2 = A[i+6][k]; 
            t3 = A[i+7][k];
            
	    t4 = B[k][i+4]; 
	    t5 = B[k][i+5]; 
	    t6 = B[k][i+6]; 
	    t7 = B[k][i+7];
					
	    B[k][i+4] = t0; 
	    B[k][i+5] = t1; 
	    B[k][i+6] = t2; 
	    B[k][i+7] = t3;
	    
	    B[k+4][i] = t4; 
	    B[k+4][i+1] = t5; 
	    B[k+4][i+2] = t6; 
	    B[k+4][i+3] = t7;
          }
          
          for (k=i+4;k<i+8;k++)
	  {
	    t0 = A[k][j+4]; 
	    t1 = A[k][j+5]; 
	    t2 = A[k][j+6]; 
	    t3 = A[k][j+7];
					
	    B[j+4][k] = t0; 
	    B[j+5][k] = t1; 
	    B[j+6][k] = t2; 
	    B[j+7][k] = t3;
	  } 
        }
}

char blocking8_t_1_prim[] = "Blocking 8_t_1_prim transpose";
void transpose_blocking8_t_1_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(i=0; i<N-N%8; i+=8){
        for(j=0; j<M-M%8; j+=8)
        {
          for(k=i;k<i+4;k++)
          {
            t0 = A[k][j];
            t1 = A[k][j+1];
            t2 = A[k][j+2];
            t3 = A[k][j+3];
            t4 = A[k][j+4];
            t5 = A[k][j+5];
            t6 = A[k][j+6];
            t7 = A[k][j+7];
            
            B[j][k] = t0;
            B[j+1][k] = t1;
            B[j+2][k] = t2;
            B[j+3][k] = t3;
            
            B[j][k+4] = t4;
            B[j+1][k+4] = t5;
            B[j+2][k+4] = t6;
            B[j+3][k+4] = t7; 
          }
          
          for(k=j;k<j+4;k++)
          {
            t0 = A[i+4][k]; 
            t1 = A[i+5][k]; 
            t2 = A[i+6][k]; 
            t3 = A[i+7][k];
            
	    t4 = B[k][i+4]; 
	    t5 = B[k][i+5]; 
	    t6 = B[k][i+6]; 
	    t7 = B[k][i+7];
					
	    B[k][i+4] = t0; 
	    B[k][i+5] = t1; 
	    B[k][i+6] = t2; 
	    B[k][i+7] = t3;
	    
	    B[k+4][i] = t4; 
	    B[k+4][i+1] = t5; 
	    B[k+4][i+2] = t6; 
	    B[k+4][i+3] = t7;
          }
          
          for (k=i+4;k<i+8;k++)
	  {
	    t0 = A[k][j+4]; 
	    t1 = A[k][j+5]; 
	    t2 = A[k][j+6]; 
	    t3 = A[k][j+7];
					
	    B[j+4][k] = t0; 
	    B[j+5][k] = t1; 
	    B[j+6][k] = t2; 
	    B[j+7][k] = t3;
	  } 
        }
        }
        
        for(i=0;i<N;i++)
           for(j=M-M%8;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%8;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              }  
           
}

char blocking8_t_prim[] = "Blocking 8_t_prim transpose";
void transpose_blocking8_t_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(j=0; j<M-M%8; j+=8){
        for(i=0; i<N-N%8; i++)
        {
                t0 = A[i][j];
                t1 = A[i][j+1];
                t2 = A[i][j+2];
                t3 = A[i][j+3];
                t4 = A[i][j+4];
                t5 = A[i][j+5];
                t6 = A[i][j+6];
                t7 = A[i][j+7];

                B[j][i] = t0;
                B[j+1][i] = t1;
                B[j+2][i] = t2;
                B[j+3][i] = t3;
                B[j+4][i] = t4;
                B[j+5][i] = t5;
                B[j+6][i] = t6;
                B[j+7][i] = t7;  
            
        }
        }
        for(i=0;i<N;i++)
           for(j=M-M%8;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%8;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              }  
           
}

char blocking16_t_prim[] = "Blocking 16_t_prim transpose";
void transpose_blocking16_t_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    for(i=0; i<N-N%16; i+=16){
        for(j=0; j<M-M%16; j+=16)
            for(k=i; k<i+16; k++)
            {
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];

                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  

                t0 = A[k][j+8];
                t1 = A[k][j+9];
                t2 = A[k][j+10];
                t3 = A[k][j+11];
                t4 = A[k][j+12];
                t5 = A[k][j+13];
                t6 = A[k][j+14];
                t7 = A[k][j+15];

                B[j+8][k] = t0;
                B[j+9][k] = t1;
                B[j+10][k] = t2;
                B[j+11][k] = t3;
                B[j+12][k] = t4;
                B[j+13][k] = t5;
                B[j+14][k] = t6;
                B[j+15][k] = t7;

            }
            }
            
            
            
        for(i=0;i<N;i++)
           for(j=M-M%16;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%16;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              } 
}

char blocking18_t_prim[] = "Blocking 18_t_prim transpose";
void transpose_blocking18_t_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7,t8;
    for(i=0; i<N-N%18; i+=18){
        for(j=0; j<M-M%18; j+=18)
            for(k=i; k<i+18; k++)
            {
                t0 = A[k][j];
                t1 = A[k][j+1];
                t2 = A[k][j+2];
                t3 = A[k][j+3];
                t4 = A[k][j+4];
                t5 = A[k][j+5];
                t6 = A[k][j+6];
                t7 = A[k][j+7];
                t8 = A[k][j+8];

                B[j][k] = t0;
                B[j+1][k] = t1;
                B[j+2][k] = t2;
                B[j+3][k] = t3;
                B[j+4][k] = t4;
                B[j+5][k] = t5;
                B[j+6][k] = t6;
                B[j+7][k] = t7;  
                B[j+8][k] = t8;

                t0 = A[k][j+9];
                t1 = A[k][j+10];
                t2 = A[k][j+11];
                t3 = A[k][j+12];
                t4 = A[k][j+13];
                t5 = A[k][j+14];
                t6 = A[k][j+15];
                t7 = A[k][j+16];
                t8 = A[k][j+17];

                B[j+9][k] = t0;
                B[j+10][k] = t1;
                B[j+11][k] = t2;
                B[j+12][k] = t3;
                B[j+13][k] = t4;
                B[j+14][k] = t5;
                B[j+15][k] = t6;
                B[j+16][k] = t7;
                B[j+17][k] = t8;

            }
            }
            
            
            
        for(i=0;i<N;i++)
           for(j=M-M%18;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%18;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              } 
}

char blocking17_prim[] = "Blocking 17_prim transpose";
void transpose_blocking17_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,m;
    for(i=0; i<N-N%17; i+=17){
        for(j=0; j<M-M%17; j+=17)
            for(k=i; k<i+17; k++)
                for(m=j;m<j+17;m++)
                {
                  B[m][k]=A[k][m];
                }
                }
            
        for(i=0;i<N;i++)
           for(j=M-M%17;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%17;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              } 
}

char blocking15_prim[] = "Blocking 15_prim transpose";
void transpose_blocking15_prim(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,m;
    for(i=0; i<N-N%15; i+=15){
        for(j=0; j<M-M%15; j+=15)
            for(k=i; k<i+15; k++)
                for(m=j;m<j+15;m++)
                {
                  B[m][k]=A[k][m];
                }
                }
            
        for(i=0;i<N;i++)
           for(j=M-M%15;j<M;j++)
           {
             k=A[i][j];
             B[j][i]=k;
           }
         for(i=N-N%15;i<N;i++)
              for(j=0;j<M;j++)
              {
                k=A[i][j];
                B[j][i]=k;
              } 
}

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}*/

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 
    
    //registerTransFunction(transpose_blocking16, blocking16); 
    
    //registerTransFunction(transpose_blocking8_t, blocking8_t); 
    
    //registerTransFunction(transpose_blocking8_t_1_prim, blocking8_t_1_prim);
    
    //registerTransFunction(transpose_blocking16_t_prim, blocking16_t_prim);
    
    //registerTransFunction(transpose_blocking8_t_prim, blocking8_t_prim);
    
    //registerTransFunction(transpose_blocking15_prim, blocking15_prim);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

