#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 729
#define reps 100

int jmax[N];
double a[N][N], b[N][N], c[N];

/* **************************************************************************
 *
 * Function prototypes
 *
 * ************************************************************************** */

void init1(void);
void init2(void);
void loop1(void);
void loop2(void);
void valid1(void);
void valid2(void);

/* **************************************************************************
 *
 * Any OpenMP variables, i.e. number of threads, schedule type, chunk size,
 * are set via the environment variables OMP_NUM_THREADS and OMP_SCHEDULE.
 * The different kinds of schedule are defined below, as in omp.h.
 *
 * typedef enum omp_sched_t {
 *   omp_sched_static = 1,
 *   omp_sched_dynamic = 2,
 *   omp_sched_guided = 3,
 *   omp_sched_auto = 4
 * } omp_sched_t;
 *
 * ************************************************************************** */

int main(void)
{
  double start1, start2, end1, end2;
  int n_chunks, n_threads;
  omp_sched_t schedule_type;

  omp_get_schedule(&schedule_type, &n_chunks);
  n_threads = omp_get_max_threads();
  printf("\nBeginning run\n");
  printf("-------------\n");
  printf("N_THREADS : %d\n", n_threads);
  printf("SCHEDULE  : %d\n", schedule_type);
  printf("N_CHUNKS  : %d\n\n", n_chunks);

  /* start of loop 1 work */
  printf("Beginning loop 1.\n");
  init1();
  start1 = omp_get_wtime();

  /* start of loop 1 repetitions */
  for (int r = 0; r < reps; r++)
  {
    loop1();
  }

  end1 = omp_get_wtime();
  valid1();
  printf("Total time for %d repetitions of loop 1 = %f seconds.\n\n",
         reps, (float)(end1 - start1));

  /* start of loop 2 work */
  printf("Beginning loop 2.\n");
  init2();
  start2 = omp_get_wtime();

  /* start of loop 2 repititions */
  for (int r = 0; r < reps; r++)
  {
    loop2();
  }

  end2 = omp_get_wtime();
  valid2();
  printf("Total time for %d repetitions of loop 2 = %f seconds.\n",
         reps, (float)(end2 - start2));

  printf("\nRuns finished.\n");
  printf("Total run time: %f seconds.\n\n", (float)(end2 - start1));

  /* print the results to file */
  FILE *out_f;
  if ((out_f = fopen("time_output.txt", "a")) == NULL)
  {
    printf("Cannot open output file. Exiting.");
    exit(-1);
  }

  fprintf(out_f, "%d\t%d\t%f\t%f\n",
  schedule_type, n_chunks, (float)(end1 - start1), (float)(end2 - start2));

  if (fclose(out_f) != 0)
  {
    printf("Cannot close output file. Exiting.");
    exit(-1);
  }

  return (0);
}

/* **************************************************************************
 *
 * Loop functions
 *
 * ************************************************************************** */

void loop1(void)
{
  #pragma omp parallel for default(none), schedule(runtime), shared(a, b)
  for (int i = 0; i < N; i++)
  {
    for (int j = N - 1; j > i; j--)
    {
      a[i][j] += cos(b[i][j]);
    }
  }
}

void loop2(void)
{
  double rN2;

  rN2 = 1.0/(double)(N * N);

  #pragma omp parallel for default(none), schedule(runtime), \
    shared(b, c, rN2, jmax)
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < jmax[i]; j++)
    {
      for (int k = 0; k < j; k++)
      {
	      c[i] += (k+1) * log (b[i][j]) * rN2;
      }
    }
  }
}

/* **************************************************************************
 *
 * Initialisation functions
 *
 * ************************************************************************** */

void init1(void)
{
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      a[i][j] = 0.0;
      b[i][j] = 3.142 * (i + j);
    }
  }
}

void init2(void)
{
  int expr;

  for (int i = 0; i < N; i++)
  {
    expr = i%( 3 * (i/30) + 1);
    if (expr == 0)
    {
      jmax[i] = N;
    }
    else
    {
      jmax[i] = 1;
    }
    c[i] = 0.0;
  }

  for (int i = 0; i<N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      b[i][j] = (double)(i * j + 1)/(double)(N * N);
    }
  }
}

/* **************************************************************************
 *
 * Verification functions
 *
 * ************************************************************************** */

void valid1(void)
{
  double suma = 0.0;

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      suma += a[i][j];
    }
  }
  printf("Loop 1 check: sum of a is %lf\n", suma);
}

void valid2(void)
{
  double sumc = 0.0;

  for (int i = 0; i < N; i++)
  {
    sumc += c[i];
  }
  printf("Loop 2 check: sum of c is %f\n", sumc);
}
