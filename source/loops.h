/* **************************************************************************
 *
 * Constants
 *
 * ************************************************************************** */

#define N 729
#define REPS 100

/* constants for work sharing function returns */
#define CONTINUE -1
#define STOP -2

/* constant for DEBUG paramteres */
#define TRUE 1
#define FALSE 0

/* **************************************************************************
 *
 * Global variables
 *
 * ************************************************************************** */

int jmax[N];
double a[N][N], b[N][N], c[N];

/* **************************************************************************
 *
 * Function prototypes
 *
 * ************************************************************************** */

void init1(void);
void init2(void);
void valid1(void);
void valid2(void);
void loop1(int n_lower, int n_upper);
void loop2(int n_lower, int n_upper);
int output_runtime(int n_threads, double loop1_time, double loop2_time);

/* ******************* Functions for affinity scheduling ******************** */
void affinity_loop(void (*loop_func)(int, int));
int share_iterations(int n_threads, int current_thread, int *thread_lowers, \
  int *thread_uppers, int *chunk_lowers, int* chunk_uppers);
int *find_loaded_thread(int n_threads, int *loaded_thread, int *thread_lowers, \
  int *thread_uppers);

/* ********************** Some debug functions ****************************** */
void print_int_array(int *array, char var_name[]);
