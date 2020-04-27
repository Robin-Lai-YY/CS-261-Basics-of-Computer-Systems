#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY  3
#define NUM_ITERATIONS 100
#define NUM_CARS       20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             20
#define WAIT_TIME_BETWEEN_CROSSES 20

/**
 * You might find these declarations useful.
 */
enum Direction {EAST = 0, WEST = 1};
const static enum Direction oppositeEnd [] = {WEST, EAST};

struct Street {
  // TODO
  pthread_mutex_t mutex;
  pthread_cond_t  east;
  pthread_cond_t  west;
  int count;
  int enter;
  enum Dirction direction;
} Street;

void initializeStreet(void) {
  // TODO
  struct Street* street = malloc (sizeof (struct Street));
  pthread_mutex_create();
  pthread_cond_create (street->mutex);
  pthread_cond_create (street->mutex);
  street->count = 0;
  street->direction = 0;
  street->enter = 0;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
pthread_mutex_t waitingHistogramMutex = PTHREAD_MUTEX_INITIALIZER;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void enterStreet (enum Direction g) {
  // TODO
  struct Street* s;
  pthread_mutex_lock(&s->mutex);
  

    int c = s->enter;
    if (g == EAST) {
    pthread_cond_wait(&s->east, &s->mutex);
  } else if (g == WEST) {
    pthread_cond_wait(&s->west, &s->mutex);
  }
    int waitingTime = s->enter - c;
    if (waitingTime < WAITING_HISTOGRAM_SIZE) {
      waitingHistogram [waitingTime] ++;
    } else {
      waitingHistogramOverflow ++;
    }
  
  s->enter++;
  s->count++;
  occupancyHistogram[g][s->count]++;
  pthread_mutex_unlock(&s->mutex);
}

void leaveStreet(void) {
  // TODO
}

void recordWaitingTime (int waitingTime) {
  pthread_mutex_lock (&waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  pthread_mutex_unlock (&waitingHistogramMutex);
}

//
// TODO
// You will probably need to create some additional procedures etc.
//

int main (int argc, char** argv) {
  
  initializeStreet();
  pthread_t pt [NUM_CARS];

  // TODO


  printf ("Times with 1 car  going east: %d\n", occupancyHistogram [EAST] [1]);
  printf ("Times with 2 cars going east: %d\n", occupancyHistogram [EAST] [2]);
  printf ("Times with 3 cars going east: %d\n", occupancyHistogram [EAST] [3]);
  printf ("Times with 1 car  going west: %d\n", occupancyHistogram [WEST] [1]);
  printf ("Times with 2 cars going west: %d\n", occupancyHistogram [WEST] [2]);
  printf ("Times with 3 cars going west: %d\n", occupancyHistogram [WEST] [3]);
  
  printf ("Waiting Histogram\n");
  for (int i=0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
