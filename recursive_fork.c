#define _XOPEN_SOURCE // Include definitions for Kill function
#include <stdio.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define NUMPROC 3  // Number of processes
#define NUMTHRD 3  // Number of threads

/*************** Function Declarations **************/
void proc (int p);
void *thread_id (void *n);

/*************** Helper Functions *******************/
// Code for each process p
void proc (int p) {
  if (p > NUMPROC) return;
  int status, vals[NUMTHRD];
  pid_t child;
  child = fork();
  if (child != 0) {
    printf("Spawning process %d pid %d \n", p, child);
    waitpid(child, &status, 0);
    proc(p + 1);
  }
  else {
    pthread_t thr[NUMTHRD];
    for (int j = 1; j <= NUMTHRD; j++){
      vals[j - 1] = j;
      pthread_create(&thr[j - 1], NULL, &thread_id, vals + j - 1);
      pthread_join(thr[j - 1], 0);
    }
//   for (int k = 1; k <= NUMTHRD; k++)
//     pthread_join(thr[k - 1], 0);
  }
}

// Code for each thread n
void *thread_id (void *n) {
  printf("I am thread pid %d.%d \n", (int) getpid(), *(int *)n);
  return NULL;
}

/**************** Main Function *********************/
int main (int argc, char** argv) {
  proc(1);
  return 0;
}
