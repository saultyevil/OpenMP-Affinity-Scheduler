#include <stdlib.h>
#include <stdio.h>

#include "loops.h"

int output_runtime(int n_threads, double loop1_time, double loop2_time)
{

    FILE *out_f;
    
    if ((out_f = fopen("time_output.txt", "a")) == NULL)
    {
        printf("Cannot open output file. Exiting.");
        exit(-1);
    }

    fprintf(out_f, "%d\t%f\t%f\n", n_threads, loop1_time, loop2_time);

    if (fclose(out_f) != 0)
    {
        printf("Cannot close output file. Exiting.");
        exit(-1);
    }

    return 0;
}
