/*
 *  counter header
 *  Function prototypes, data, and constants for synchronized counter module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// SYNCHRONIZED COUNTER

// counter struct

#ifndef MAT_COUNTER
#define MAT_COUNTER

struct counter {
  int             mats;
  int		  maxMats;
  pthread_mutex_t lock;
} prodMats, consMats;

#endif

// counter methods
void init(struct counter *c, int max);
void increment(struct counter *c);
int isMax(struct counter *c);
int getCount(struct counter *c);
