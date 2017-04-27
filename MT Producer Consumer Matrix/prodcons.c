/*
 *  prodcons module
 *  Producer Consumer module
 * 
 *  Implements routines for the producer consumer module based on 
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

// Producer consumer data structures
int ** buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

// Producer consumer locks and conditions
pthread_cond_t empty, fill;
pthread_mutex_t mutex;

// PRODUCER-CONSUMER put() get()
void put(int **matrix)
{
  buffer[fill_ptr] = matrix;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int **get() 
{
  int **tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

// PRODUCER - return total of all arrays produced
void *prod_worker(void *arg)
{
  int *sum = (int *)malloc(sizeof(int));
  *sum = 0;

  while (!isMax(&prodMats)) {
    pthread_mutex_lock(&mutex);
    int **matrix = AllocMatrix(ROW, COL);
    GenMatrix(matrix, ROW, COL);
    *sum += SumMatrix(matrix, ROW, COL);
    put(matrix);
    increment(&prodMats);

    while (count == MAX) {
      pthread_cond_wait(&empty, &mutex);
    }
    
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
 
  return sum;
}

// CONSUMER - return total of all arrays consumed
void *cons_worker(void *arg) 
{
  int *sum = (int *)malloc(sizeof(int));
  *sum = 0;

  while (!isMax(&consMats)) {
    pthread_mutex_lock(&mutex);

    increment(&consMats);

    while (count == 0) {
      pthread_cond_wait(&fill, &mutex);
    } 

    int** matrix = get();
    *sum += SumMatrix(matrix, ROW, COL);
    
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }

  return sum;
}

