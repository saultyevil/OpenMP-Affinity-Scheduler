#include <stdio.h>
#include <omp.h>

void print_int_array(int *array, char var_name[])
{
  int n_threads = omp_get_max_threads();

  printf("%s\n", var_name);
  for (int i = 0; i < n_threads; i++)
  {
    printf("%d ", array[i]);
  }
  printf("\n\n");  // spacer to make the output slightly more readable
}
