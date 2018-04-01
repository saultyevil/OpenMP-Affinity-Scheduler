#include <stdio.h>
#include <math.h>

#include "loops.h"

/* **************************************************************************
 *
 * Loop functions
 *
 * ************************************************************************** */

void loop1(int n_lower, int n_upper)
{
  for (int i = n_lower; i < n_upper; i++)
  {
    for (int j = N - 1; j > i; j--)
    {
      a[i][j] += cos(b[i][j]);
    }
  }
}

void loop2(int n_lower, int n_upper)
{
  double rN2 = 1.0/(N * N);

  for (int i = n_lower; i < n_upper; i++)
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
