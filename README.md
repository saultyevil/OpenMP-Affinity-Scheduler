Note, this was created very early on in my programming career - pls ignore the Makefile mess. I wish C had a better build system.

A scheduling method developed for ACM2 which allows threads to share work between other threads once a thread has finished its assigned work in its own local set of iterations. The directory `source` contains all of the source code for the affinity scheduler.

To build the scheduler, use the Makefile of your choice. If you are building on OS X, you will need to use gcc-7 or some other C compiler which supports the -fopenmp flag as the version of Clang which ships with OS X by default does not. Otherwise, if you are using Linux, you can use `MakefileGCC` and if you are building on a machine where the Cray Compiler is available, use `MakefileCC` to build, i.e. to build and run using GCC7 (such as on OS X)

```
$ cp MakefileGCC7 Makefile
$ make
./loops_affinity
``` 

The directory `loops` contains the original serial version of the benchmark loops, `loops.c`,  and a version of `loops.c` which uses the default OpenMP schedulers which can be used by setting the OMP envrionment variable `export OMP_SCHEDULE=type,chunk_size` at runtime, where the available schedule types are `auto`, `dynamic`, `static`, and `guided`. The `report` directory contains a tex file for the accompaning report and a directory `results` containing all of the results used in the report, as well as a Python script to plot the results.


## Acknowledgements 

I would like to acknowledge financial support from the EPSRC Centre for Doctoral Training in Next Generation Computational Modelleing grant EP/L015382/1.
