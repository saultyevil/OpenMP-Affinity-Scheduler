#include <stdio.h>
#include <omp.h>

#include "loops.h"

/* **************************************************************************
 *
 * The number of threads can be defined using the OpenMP envrionment variable
 * OMP_NUM_THREADS.
 *
 * Sometimes the program seg faults before it even reaches the loop
 * repetitions on OS X, but not on Ubuntu or ARCHER. But it works if you try
 * to run it a few times ¯\_(ツ)_/¯ -- turns out it was something to do with
 * my Makefile and it should work no with no issues.
 *
 * ************************************************************************** */

int main(void)
{
  double start, stop, main_start, main_stop, loop1_time, loop2_time, total_time;
  int n_thread_max = omp_get_max_threads();

  /* Pointers to loop functions
   */
  void (*loop1_func)(int, int) = loop1;
  void (*loop2_func)(int, int) = loop2;

  printf("\nBeginning runs\n");
  printf("--------------\n");
  printf("REPETITIONS      : %d\n", REPS);
  printf("N_THREADS USABLE : %d\n\n", n_thread_max);

  main_start = omp_get_wtime();

/* ******************************* LOOP 1 *********************************** */

  printf("Beginning loop 1.\n");
  init1();

  start = omp_get_wtime();
  for (int r = 0; r < REPS; r++)
  {
    affinity_loop(loop1_func);  // Call the affinity loop
  }
  stop = omp_get_wtime();
  loop1_time = stop - start;

  valid1();
  printf("Total time for %d repetitions of loop 1 = %f seconds.\n\n", \
         REPS, loop1_time);

/* ******************************* LOOP 2 *********************************** */

  printf("Beginning loop 2.\n");
  init2();

  start = omp_get_wtime();
  for (int r = 0; r < REPS; r++)
  {
    affinity_loop(loop2_func);  // Call the affinity loop
  }
  stop = omp_get_wtime();
  loop2_time = stop - start;

  valid2();
  printf("Total time for %d repetitions of loop 2 = %f seconds.\n", \
         REPS, loop2_time);

/* ******************************** END  ************************************ */

  main_stop = omp_get_wtime();
  total_time = main_stop - main_start;

  output_runtime(n_thread_max, loop1_time, loop2_time);
  printf("\nRuns finished in %f seconds.\n", total_time);

  return 0;
}
