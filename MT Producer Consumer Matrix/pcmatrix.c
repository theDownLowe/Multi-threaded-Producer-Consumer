/*
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 * 
 *  Creates producer and consumer threads and launches them to produce matrices
 *  and consume matrices.  Each thread produces a total sum of the value of
 *  randomly generated elements.  Producer sum and consumer sum must match.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int main (int argc, char * argv[])
{
  time_t t;
  int numw = NUMWORK;

  // Seed the random number generator with the system time
  srand((unsigned) time(&t));

  

  printf("\n----- Phase 1 -----\n");
  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With 1 producer and 1 consumer threads.\n");

  pthread_t pr;
  pthread_t co;

  init(&prodMats, LOOPS);
  init(&consMats, LOOPS);

  int *prs;
  int *cos;

  pthread_create(&pr, NULL, prod_worker, NULL);
  pthread_create(&co, NULL, cons_worker, NULL);
  pthread_join(pr, (void**)&prs);
  pthread_join(co, (void**)&cos);

  printf("Produced = Consumed --> %d = %d\n",*prs,*cos);

  free(prs);
  free(cos);
 


  printf("\n----- Phase 2 -----\n");
  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With 2 producer and 1 consumer threads.\n");

  pthread_t pr_2_1, pr_2_2, co_2;

  init(&prodMats, LOOPS);
  init(&consMats, LOOPS);

  int *prs_2_1;
  int *prs_2_2;
  int *cos_2;

  pthread_create(&pr_2_1, NULL, prod_worker, NULL);
  pthread_create(&pr_2_2, NULL, prod_worker, NULL);
  pthread_create(&co_2, NULL, cons_worker, NULL);
  pthread_join(pr_2_1, (void**)&prs_2_1);
  pthread_join(pr_2_2, (void**)&prs_2_2);
  pthread_join(co_2, (void**)&cos_2);

  int prSum = (*prs_2_1 + *prs_2_2);

  printf("Produced = Consumed --> %d = %d\n",prSum, *cos_2);

  free(prs_2_1);
  free(prs_2_2);
  free(cos_2);


 
  printf("\n----- Phase 3 -----\n");
  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With 2 producer and 2 consumer threads.\n");


  pthread_t pr_3_1, pr_3_2, co_3_1, co_3_2;

  init(&prodMats, LOOPS);
  init(&consMats, LOOPS);

  int *prs_3_1;
  int *prs_3_2;
  int *cos_3_1;
  int *cos_3_2;

  pthread_create(&pr_3_1, NULL, prod_worker, NULL);
  pthread_create(&pr_3_2, NULL, prod_worker, NULL);
  pthread_create(&co_3_1, NULL, cons_worker, NULL);
  pthread_create(&co_3_2, NULL, cons_worker, NULL);
  pthread_join(pr_3_1, (void**)&prs_3_1);
  pthread_join(pr_3_2, (void**)&prs_3_2);
  pthread_join(co_3_1, (void**)&cos_3_1);
  pthread_join(co_3_2, (void**)&cos_3_2);

  int prSum_3 = (*prs_3_1 + *prs_3_2);
  int coSum_3 = (*cos_3_1 + *cos_3_2);

  printf("Produced = Consumed --> %d = %d\n",prSum_3, coSum_3);

  free(prs_3_1);
  free(prs_3_2);
  free(cos_3_1);
  free(cos_3_2);



  printf("\n----- Phase 4 ----- \n");
  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With %d producer and consumer threads.\n",numw);

  pthread_t pr_4[numw];
  pthread_t co_4[numw];

  init(&prodMats, LOOPS);
  init(&consMats, LOOPS);

  int *prs_4[numw];
  int *cos_4[numw];  

  int i;
  for (i = 0; i < numw; i++) {
    pthread_create(&pr_4[i], NULL, prod_worker, NULL);
    pthread_create(&co_4[i], NULL, cons_worker, NULL);
  }

  int prSum_4 = 0;
  int coSum_4 = 0;

  for (i = 0; i < numw; i++) {
    pthread_join(pr_4[i], (void**)&prs_4[i]);
    pthread_join(co_4[i], (void**)&cos_4[i]);
    prSum_4 += *prs_4[i];
    coSum_4 += *cos_4[i];
  }

  printf("Produced = Consumed --> %d = %d\n",prSum_4, coSum_4);

  for (i = 0; i < numw; i++) {
    free(prs_4[i]);
    free(cos_4[i]);
  }


  return 0;
}
