/*  usernames: lbelyi and ncarvajalerker
 *  team name: lbelyi-ncarvajalerker
 *  REAL team name : The Nutella Haters
 *
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
  // 12 local variables at most
  // no arrays or calloc'string
  // A is not mutable but B is
  // no recursion
  // helper functions must abide by the 12 variable rule
  // cache is a (5, 1, 5) for (s, E, b)
  int i, j, i_block, j_block; //for iteration i=collums j=rows and then through collum and row blocks
  int b_s; //block size
  int temp; //temparay variable
  int d; //diagonal 

  if (N == 32 && M == 32) {
    b_s = 8;

    //2 loops iterates through blocks and 2 loops iterate across each block. 
    for (j_block = 0; j_block < (N / b_s); j_block++) {
      for (i_block = 0; i_block < (M / b_s); i_block++) {
        for (i = (i_block * b_s); i < ((i_block + 1) * b_s); i++) {
          for(j = (j_block * b_s); j < ((j_block + 1) * b_s); j++) {
            if(i != j){  //When row and collumn is not equal then we can change the value in B array to the value in A
              B[j][i] = A[i][j];
            } else {
              //When a row == column it touched the diagonal of the matrix so use 2 temporary vars to stop and store the position
              temp = A[i][j];
              d = i;
            }
          }

           // Each traverse only has 1 element in the diagonal line that is assigned outside of the loop.
          if (i_block == j_block) {
            B[d][d] = temp;
          }
        }
      }
    }

  }
  else if (N == 64) { 
     int colRun, rowRun, k, a0, a1, a2, a3, a4, a5, a6, a7; // supporting variables

    // two loops to go through each row and column block 
    for(colRun=0; colRun<64; colRun+=8 ){
        for(rowRun=0; rowRun<64; rowRun+=8 ){
        //The first loop is for a 4 collumn 8 row of A. use loop to store variables. the nchange to the poistion of B. Not everything will be transposed correct, there will be some misses.
          for(k=0; k<4; k++){ 
            a0 = A[colRun+k][rowRun+0];
            a1 = A[colRun+k][rowRun+1];
            a2 = A[colRun+k][rowRun+2];
            a3 = A[colRun+k][rowRun+3];
            a4 = A[colRun+k][rowRun+4];
            a5 = A[colRun+k][rowRun+5];
            a6 = A[colRun+k][rowRun+6];
            a7 = A[colRun+k][rowRun+7];

            B[rowRun+0][colRun+k+0] = a0; 
            B[rowRun+0][colRun+k+4] = a5; 
            B[rowRun+1][colRun+k+0] = a1; 
            B[rowRun+1][colRun+k+4] = a6; 
            B[rowRun+2][colRun+k+0] = a2; 
            B[rowRun+2][colRun+k+4] = a7; 
            B[rowRun+3][colRun+k+0] = a3; 
            B[rowRun+3][colRun+k+4] = a4; 
          }
          //This is for the stored variables from the loop above that are correct and puts them in the right position. moves one matrix to another matrix b.
          a0 = A[colRun+4][rowRun+4];
          a1 = A[colRun+5][rowRun+4];
          a2 = A[colRun+6][rowRun+4];
          a3 = A[colRun+7][rowRun+4];
          a4 = A[colRun+4][rowRun+3];
          a5 = A[colRun+5][rowRun+3];
          a6 = A[colRun+6][rowRun+3];
          a7 = A[colRun+7][rowRun+3];

          B[rowRun+4][colRun+0] = B[rowRun+3][colRun+4];
          B[rowRun+4][colRun+4] = a0; 
          B[rowRun+3][colRun+4] = a4;
          B[rowRun+4][colRun+1] = B[rowRun+3][colRun+5];
          B[rowRun+4][colRun+5] = a1;
          B[rowRun+3][colRun+5] = a5;
          B[rowRun+4][colRun+2] = B[rowRun+3][colRun+6];
          B[rowRun+4][colRun+6] = a2;
          B[rowRun+3][colRun+6] = a6;
          B[rowRun+4][colRun+3] = B[rowRun+3][colRun+7];
          B[rowRun+4][colRun+7] = a3;
          B[rowRun+3][colRun+7] = a7;

          // This loops deal with the other elements that remain.
          for(k=0;k<3;k++){
            a0 = A[colRun+4][rowRun+5+k];
            a1 = A[colRun+5][rowRun+5+k];
            a2 = A[colRun+6][rowRun+5+k];
            a3 = A[colRun+7][rowRun+5+k];
            a4 = A[colRun+4][rowRun+k];
            a5 = A[colRun+5][rowRun+k];
            a6 = A[colRun+6][rowRun+k];
            a7 = A[colRun+7][rowRun+k];

            B[rowRun+5+k][colRun+0] = B[rowRun+k][colRun+4];
            B[rowRun+5+k][colRun+4] = a0;
            B[rowRun+k][colRun+4] = a4;
            B[rowRun+5+k][colRun+1] = B[rowRun+k][colRun+5];
            B[rowRun+5+k][colRun+5] = a1;
            B[rowRun+k][colRun+5] = a5;
            B[rowRun+5+k][colRun+2] = B[rowRun+k][colRun+6];
            B[rowRun+5+k][colRun+6] = a2;
            B[rowRun+k][colRun+6] = a6;
            B[rowRun+5+k][colRun+3] = B[rowRun+k][colRun+7];
            B[rowRun+5+k][colRun+7] = a3;
            B[rowRun+k][colRun+7] = a7;
          }
        }
      }
    }

  else {
    b_s = 16;
    
    for (j_block = 0; j_block < M; j_block += b_s){
      for (i_block = 0; i_block < N; i_block += b_s){
        for(i = i_block; (i < N) && (i < i_block + b_s); i++){
          for(j = j_block; (j < M) && (j < j_block + b_s); j++){
            if (i != j){ //if i and j not same (rows and collums)
              B[j][i] = A[i][j];
            else{ //if i and j the same (rows and collums)
              temp = A[i][j];
              d = i;
            }
          }
          
          //Diagonal
          if(i_block == j_block){
            B[d][d] = temp;
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
