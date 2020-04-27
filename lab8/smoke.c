#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NUM_ITERATIONS 1000
#define TRUE 1

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  pthread_mutex_t mutex;
  pthread_cond_t  match;
  pthread_cond_t  paper;
  pthread_cond_t  tobacco;
  pthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  pthread_mutex_init(&agent->mutex,  NULL);
  pthread_cond_init(&agent->paper,   NULL);
  pthread_cond_init(&agent->match,   NULL);
  pthread_cond_init(&agent->tobacco, NULL);
  pthread_cond_init(&agent->smoke,   NULL);
  return agent;
}

int sum = 0;
pthread_cond_t match_and_paper;
pthread_cond_t paper_and_tobacco;
pthread_cond_t match_and_tobacco;

//
// TODO
// You will probably need to add some procedures and struct etc.
//

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  pthread_mutex_lock (&a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    pthread_cond_wait (&a->smoke, &a->mutex);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 3;
    signal_count [matching_smoker [r]] ++;
    int c = choices [r];
    if (c & MATCH) {
      VERBOSE_PRINT ("match available\n");
      pthread_cond_signal (&a->match);
    }
    if (c & PAPER) {
      VERBOSE_PRINT ("paper available\n");
      pthread_cond_signal (&a->paper);
    }
    if (c & TOBACCO) {
      VERBOSE_PRINT ("tobacco available\n");
      pthread_cond_signal (&a->tobacco);
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    pthread_cond_wait (&a->smoke, &a->mutex);
  }
  
  pthread_mutex_unlock (&a->mutex);
  return NULL;
}

void* tobacco_listener (void* av){
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&a->tobacco, &a->mutex);
    sum = sum + TOBACCO;
    if(sum == 6){
      pthread_cond_signal(&paper_and_tobacco);
      sum = 0;      
    }
    else if(sum == 5){
      pthread_cond_signal(&match_and_tobacco);
      sum = 0;
    }
    else if(sum == 3){
      pthread_cond_signal(&match_and_paper);
      sum = 0;
    }
  }
  pthread_mutex_unlock(&a->mutex);
}

void* match_listener (void* av){
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&a->match, &a->mutex);
    sum = sum + MATCH;
    if(sum == 6){
      pthread_cond_signal(&paper_and_tobacco);
      sum = 0;      
    }
    else if(sum == 5){
      pthread_cond_signal(&match_and_tobacco);
      sum = 0;
    }
    else if(sum == 3){
      pthread_cond_signal(&match_and_paper);
      sum = 0;
    }
  }
  pthread_mutex_unlock(&a->mutex);
}

void* paper_listener (void* av){
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&a->paper, &a->mutex);
    sum = sum + PAPER;
    if(sum == 6){
      pthread_cond_signal(&paper_and_tobacco);
      sum = 0;      
    }
    else if(sum == 5){
      pthread_cond_signal(&match_and_tobacco);
      sum = 0;
    }
    else if(sum == 3){
      pthread_cond_signal(&match_and_paper);
      sum = 0;
    }
  }
  pthread_mutex_unlock(&a->mutex);
}


void* wait_tobacco (void* av){
  num_active_threads++;
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&match_and_paper, &a->mutex);
    pthread_cond_signal(&a->smoke);
    smoke_count [TOBACCO]++;
  }
  pthread_mutex_unlock(&a->mutex);
}

void* wait_match (void* av){
  num_active_threads++;
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&paper_and_tobacco, &a->mutex);
    pthread_cond_signal(&a->smoke);
    smoke_count [MATCH]++;
  }
  pthread_mutex_unlock(&a->mutex);
}

void* wait_paper (void* av){
  num_active_threads++;
  struct Agent* a = av;
  pthread_mutex_lock(&a->mutex);
  while(TRUE){
    pthread_cond_wait(&match_and_tobacco, &a->mutex);
    pthread_cond_signal(&a->smoke);
    smoke_count [PAPER]++;
  }
  pthread_mutex_unlock(&a->mutex);
}

int main (int argc, char** argv) {
  
  struct Agent* a = createAgent();
  pthread_t agent_thread;

  pthread_t match, paper, tobacco;
  pthread_t match_c, paper_c, tobacco_c;
  
  pthread_cond_init(&match_and_paper, NULL);
  pthread_cond_init(&paper_and_tobacco, NULL);
  pthread_cond_init(&match_and_tobacco, NULL);

  
  pthread_create(&match_c, NULL, match_listener, a);
  pthread_create(&paper_c, NULL, paper_listener, a);
  pthread_create(&tobacco_c, NULL, tobacco_listener, a);

  pthread_create(&match, NULL, wait_match, a);
  pthread_create(&paper, NULL, wait_paper, a);
  pthread_create(&tobacco, NULL, wait_tobacco, a);


  pthread_create (&agent_thread, NULL, agent, a);
  pthread_join (agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}