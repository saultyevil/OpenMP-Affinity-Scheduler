#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "loops.h"

void affinity_loop(void (*loop_func)(int, int))
{
  int n_threads = omp_get_max_threads();
  int local_set_iters = (int) ceil((double) N/n_threads);  // always round up

  /* These arrays are used to store the upper and lower limits for the current
   * threads and chunks. Initialise the arrays here because I don't want them to
   * be private to a thread
   */
  int *thread_lowers = malloc(sizeof(int) * n_threads);
  int *thread_uppers = malloc(sizeof(int) * n_threads);
  int *chunk_lowers = malloc(sizeof(int) * n_threads);
  int *chunk_uppers = malloc(sizeof(int) * n_threads);

  #pragma omp parallel default(none), \
    shared(loop_func, n_threads, local_set_iters, thread_lowers, thread_uppers,\
      chunk_lowers, chunk_uppers)
  {
    int thread_affininty_check = CONTINUE;
    int thread_id = omp_get_thread_num();

    /* Calculate the lower and upper limits for each thread. If the upper limit
     * is > N - 1, the upper limit is set to N - 1.
     */
    thread_lowers[thread_id] = thread_id * local_set_iters;
    if ((thread_uppers[thread_id] = \
      (thread_id + 1) * local_set_iters - 1) > N - 1)
    {
      thread_uppers[thread_id] = N - 1;
    }

    /* While loop - the threads will keep doing work until there is no more
     * work to steal from any other thread
     *
     * The loop function will originally be assigned no chunks to do, as the
     * first assigned chunks will be done in share_iterations
     */

    chunk_lowers[thread_id] = chunk_uppers[thread_id] = 0;

    while (thread_affininty_check != STOP)
    {
      loop_func(chunk_lowers[thread_id], chunk_uppers[thread_id]);

      /* Only one thread should be looking at being able to take work at
       * once, otherwise some threads will try to execute the same chunk
       */
      #pragma omp critical (ThreadUpdate)
      {
        thread_affininty_check = share_iterations(n_threads, thread_id, \
          thread_lowers, thread_uppers, chunk_lowers, chunk_uppers);
      }
    }
  }

  free(thread_uppers); free(thread_lowers);
  free(chunk_lowers); free(chunk_uppers);
}

/* **************************************************************************
 *
 * Main function for sharing interations -- searches for a thread with work
 * to do, be itself or another thread, and then the work is given to each
 * thread
 *
 * ************************************************************************** */
int share_iterations(int n_threads, int current_thread, int *thread_lowers,
  int *thread_uppers, int *chunk_lowers, int *chunk_uppers)
{
  int iters_to_take;
  double n_iters_share = 1.0/n_threads;

  /* Loaded thread is an array of size 2. The first entry will be the thread ID
  * of the thread to take work from and the second entry will be the number of
  * iterations it has left to do.
  */
  int *loaded_thread = malloc(sizeof(int) * 2);

  /* If there is work to do in the current thread's local iteration set, do
   * work on its own set
   */
  if ((loaded_thread[1] = \
    thread_uppers[current_thread] - thread_lowers[current_thread]) >  0)
  {
    loaded_thread[0] = current_thread;
  }
  /* Else the thread has no work left to do in its local iteration set, so take
   * work from other threads
   */
  else
  {
    loaded_thread = find_loaded_thread(n_threads, loaded_thread, thread_lowers,\
      thread_uppers);
  }

  /* If there is no work in the thread's loacal set, or in another thread's
   * local set, find_loaded_thread will return STOP and all the work is done
   */
  if (loaded_thread[0] == STOP)
  {
    free(loaded_thread);
    return (STOP);
  }

  /* The number of iterations to take from the loaded thread will be the
  * fraction 1/p * iterations left ROUNDED UP -- this should avoid the situation
  * where the iterations will round down to 0 and thus the program will never
  * end
  */
  iters_to_take = (int) ceil(loaded_thread[1] * n_iters_share);

  chunk_lowers[current_thread] = thread_lowers[loaded_thread[0]];
  chunk_uppers[current_thread] = thread_lowers[loaded_thread[0]] + \
    iters_to_take;

  thread_lowers[loaded_thread[0]] += iters_to_take;

  free(loaded_thread);
  return CONTINUE;
}

/* **************************************************************************
 *
 * This function deals with finding the most loaded thread - it loops through
 * the boundary arrays and finds the thread which has the biggest difference
 * between the upper and lower boundary
 *
 * ************************************************************************** */
int *find_loaded_thread(int n_threads, int *loaded_thread, int *thread_lowers,
  int *thread_uppers)
{
  int iters_left;

  for (int thread = 0; thread < n_threads; thread++)
  {
    iters_left = thread_uppers[thread] - thread_lowers[thread];

    if (iters_left > loaded_thread[1])
    {
      loaded_thread[0] = thread;  // want to take work from this thread
      loaded_thread[1] = iters_left;
    }
  }

  if (loaded_thread[1] == 0)  // there is no work left to do
  {
    loaded_thread[0] = STOP;
    loaded_thread[1] = STOP;
  }

  return loaded_thread;
}
