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
#define DOWN 0
#define UP 1
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
	int rowstep = 8;
	int colstep = 8;
  int temp,c;
	int i,j,k,l;
	int flag = 0;
	if (M==64){
		rowstep =8 ;
		colstep =4 ;
		for (i = 0 ;i<N; i += rowstep){
			for (j = 0; j < M; j+=colstep){
				for (k= i; k< i+rowstep;k++){
						for (l = j; l < j+colstep; l++){
								if ((((k<<3) + (l>>3))&0x1f)==(((l<<3)+ (k>>3))&0x1f)){
											if (flag == 0){
											c = l;
											flag = 1;
											continue;
												}
								}
								temp =A[k][l];		
								B[l][k] = temp;
							}
						if (flag == 1){
								temp = A[k][c];
								B[c][k] = temp;
						}	
						flag = 0;
					}
			}		
	}
}
	else{
			 rowstep =8;
			colstep = 8;
		for (i = 0 ;i<N; i += rowstep){
			for (j = 0; j < M; j+=colstep){
				for (k = i; k< i+rowstep;k++){
					for (l = j; l < j+colstep; l++){
								if ((l ==k)) {flag = 1;continue;}
								//if (l/8 == ((k*4)%))
								temp =A[k][l];		
								B[l][k] = temp;
							}
						if (flag == 1){
							temp = A[k][k];
							B[k][k] = temp;
							flag = 0;
						}
					}
			}
		}	
	}
}		


/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
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

}

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
    registerTransFunction(trans, trans_desc); 

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

