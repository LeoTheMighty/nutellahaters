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
  int i, j;
  int i_block, j_block;
  int b_s;
  int temp;
  int d;

  if (N == 32 && M == 32) {
    b_s = 8;

    for (j_block = 0; j_block < (N / b_s); j_block++) {
      for (i_block = 0; i_block < (M / b_s); i_block++) {
        for (i = (i_block * b_s); i < ((i_block + 1) * b_s); i++) {
          for(j = (j_block * b_s); j < ((j_block + 1) * b_s); j++) {
            if(i != j){
              B[j][i] = A[i][j];
            } else {
              temp = A[i][j];
              d = i;
            }
          }

          if (i_block == j_block) {
            B[d][d] = temp;
          }
        }
      }
    }

  }
  else if (N == 64 && M == 64) {
    b_s = 4;

    for (j_block = 0; j_block < (N / b_s); j_block++) {
      for (i_block = 0; i_block < (M / b_s); i_block++) {
        for (i = (i_block * b_s); i < ((i_block + 1) * b_s); i++) {
          for(j = (j_block * b_s); j < ((j_block + 1) * b_s); j++) {
            if(i != j){
              B[j][i] = A[i][j];
            } else {
              temp = A[i][j];
              d = i;
            }
          }

          if (i_block == j_block) {
            B[d][d] = temp;
          }
        }
      }
    }
  }
  else {
    b_s = 8;

    for (j_block = 0; j_block < (N / b_s); j_block++) {
      for (i_block = 0; i_block < (M / b_s); i_block++) {
        for (i = (j_block * b_s); i < ((j_block + 1) * b_s); i++) {
          for(j = (i_block * b_s); j < ((i_block + 1) * b_s); j++) {
            if (i_block - j_block >= 0) {
              B[j][i] = A[i][j];
            }
          /*
          if(i != j){
            B[M][N] = A[M][N];
          }else{
            temp = A[M][N];
          }*/
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
