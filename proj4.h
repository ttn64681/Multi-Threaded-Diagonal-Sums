#ifndef PROJ4_H
#define PROJ4_H

/*
 * The struct grid_t contains a pointer p to a 2D array of 
 * unsigned chars with n rows and n columns stored on
 * the heap of this process. Once this is initialized properly,
 * p[i][j] should be a valid unsigned char for all i = 0..(n-1)
 * and j = 0..(n-1).
 * Do not alter this typedef or struct in any way.
 */
typedef struct grid_t {
  unsigned int n;
  unsigned char ** p;
} grid;


/*
 * Initialize g based on fileName, where fileName
 * is a name of file in the present working directory
 * that contains a valid n-by-n grid of digits, where each
 * digit in the grid is between 1 and 9 (inclusive).
 * Do not alter this function prototype in any way.
 */
void initializeGrid(grid * g, char * fileName);


/*
 * This function will compute all diagonal sums in input that equal s using
 * t threads, where 1 <= t <= 3, and store all of the resulting
 * diagonal sums in output. Each thread should do
 * roughly (doesn't have to be exactly) (100 / t) percent of the 
 * computations involved in calculating the diagonal sums. 
 * This function should call (or call another one of your functions that calls)
 * pthread_create and pthread_join when 2 <= t <= 3 to create additional POSIX
 * thread(s) to compute all diagonal sums. 
 * Do not alter this function prototype in any way.
 */
void diagonalSums(grid * input, unsigned long s, grid * output, int t);


/*
 * Write the contents of g to fileName in the present
 * working directory. If fileName exists in the present working directory, 
 * then this function should overwrite the contents in fileName.
 * If fileName does not exist in the present working directory,
 * then this function should create a new file named fileName
 * and assign read and write permissions to the owner. 
 * Do not alter this function prototype in any way.
 */
void writeGrid(grid * g, char * fileName);

/*
 * Free up all dynamically allocated memory used by g.
 * This function should be called when the program is finished using g.
 * Do not alter this function prototype in any way.
 */
void freeGrid(grid * g);

/*
 * You may add any additional function prototypes and any additional
 * things you need to complete this project below this comment. 
 * Anything you add to this file should be commented. 
 */


/*
 * This struct is used to pass multiple arguments
 * to the pthread_create call, including the input grid,
 * the output grid, target sum, start row, end row, and
 * the main thread's id in order to distinguish it from
 * additional POSIX threads.
 */
typedef struct pthread_create_args {
  grid *inputGrid;
  grid *outputGrid;
  unsigned long targetSum;
  int startRow;
  int endRow;
  pthread_t mainThreadID;
} thread_args; 


/*                                                                                                
 * Initialize output grid based on the amount of rows/columns of initial 
 * input grid. The only difference between output grid and initial grid is 
 * that the output grid values are all intialized to 0.
 *
 * INPUT:
 *
 * @param grid the output grid to intitialize
 *                                                                                                
 * OUTPUT:                                                                                        
 * @return void (NONE)                                                                            
 */
void initializeOutput(grid * input, grid * output);


/*
 * This helper function is called by diagonalSums to calculate the diagonal
 * sums of a grid and mark it in an output grid.
 *
 * INPUTS:
 * passedArgs the args passed in via generic pointer type by pthread_create
 *
 * OUTPUT:
 * @return void (NONE)
 */
void *calcDiagSums(void *passedArgs);


#endif

