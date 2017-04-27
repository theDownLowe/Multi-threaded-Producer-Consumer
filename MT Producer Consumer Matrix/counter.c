/*
 *  Signal example 
 *  Based on Operating Systems: Three Easy Pieces by R. Arpaci-Dusseau and A. Arpaci-Dusseau
 * 
 *  This module provides a shared synchronized counter data structure. The counter is used to track
 *  whether the producer and consumer threads have produced and consumed up to #LOOPS(pcmatrix.c) 
 *  matrices.
 *
 *  Wes J. Lloyd
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include libraries required for this module only
#include <stdio.h>
#include <pthread.h>
#include "counter.h"

// SYNCHRONIZED COUNTER METHOD IMPLEMENTATION

void init(struct counter *c, int max) {
  c->mats = 0;
  c->maxMats = max;
  pthread_mutex_init(&c->lock, NULL);
}

void increment(struct counter *c) {
  pthread_mutex_lock(&c->lock);
  c->mats++; 
  pthread_mutex_unlock(&c->lock); 
}

int isMax(struct counter *c) {
  pthread_mutex_lock(&c->lock); 
   
  if (c->mats >= (c->maxMats - 1)) {
    pthread_mutex_unlock(&c->lock);
    return 1;	 			// If MAX
  } else {
    pthread_mutex_unlock(&c->lock);
    return 0;				// If not MAX
  }
}

int getCount(struct counter *c) {
  return c->mats;
}
