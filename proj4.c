#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "proj4.h"

void initializeGrid(grid * g, char * fileName) {
  /* Open grid file w/ read perm's */
  FILE *file = fopen(fileName, "r");
  if (file == NULL) { // error
    fprintf(stderr, "Input file failed to open.\n");
    exit(EXIT_FAILURE);
  } // if

  /* Determine grid size n by reading first line char by char */
  unsigned int ch = fgetc(file); // Get char
  g->n = 0;
  while (ch != '\n' && ch != EOF) { // Parse line char by char
    g->n++; // For each char, increment grid size n
    ch = fgetc(file); // Get next char
  } // while

  /* Reset file pointer to beginning of file */
  //  rewind(file);

  /* Close and reopen the file to reset pointer to beginning */
  fclose(file);
  file = fopen(fileName, "r");
  if (file == NULL) {
    fprintf(stderr, "Error reopening input file.\n");
    exit(EXIT_FAILURE);
  } // of
  
  /* Allocate memory for rows, then columns, then fill in w/ values from file */
  g->p = malloc(g->n * sizeof(unsigned char *)); // Malloc rows
  if (g->p == NULL) { // Error
    fprintf(stderr, "Failed to allocate memory for input grid.\n");
    fclose(file); 
    exit(EXIT_FAILURE);
  } // if
  for (int i = 0; i < g->n; i++) { // For each row,
    g->p[i] = malloc(g->n * sizeof(unsigned char)); // Malloc columns
    if (g->p[i] == NULL) { // Error
      fprintf(stderr, "Failed to allocate memory for input grid.\n");
      fclose(file); 
      exit(EXIT_FAILURE);    
    } // if
    for (int j = 0; j < g->n; j++) { // For each column,     
      ch = fgetc(file); // Get char of column line,
      g->p[i][j] = ch - '0'; // And write char as integer on the input grid
    } // for
    fgetc(file); // DON'T add newline character
  } // for
  
  fclose(file);
} // initializeGrid

void initializeOutput(grid * input, grid * output) {
  output->n = input->n; // Set output n to input n

  /* Allocate memory for rows, then columns, then fill in w/ 0s (calloc) */
  output->p = malloc(output->n * sizeof(unsigned char *)); // Malloc rows
  if (output->p == NULL) { // Error
    fprintf(stderr, "Failed to allocate memory for output grid.\n");
    exit(EXIT_FAILURE);
  } // if
  for (int i = 0; i < output->n; i++) { // For each row,
    output->p[i] = calloc(output->n, sizeof(unsigned char)); // Calloc collumns
    if (output->p[i] == NULL) { // Error
    fprintf(stderr, "Failed to allocate memory for output grid.\n");
    exit(EXIT_FAILURE);    
    } // if    
  } // for
} // initializeOutput
  

void diagonalSums(grid * input, unsigned long s, grid * output, int t) {
  initializeOutput(input, output); // Init output grid w/ 0's
  pthread_t mainThreadID = pthread_self(); // Get main thread id
  
  /* If only 1 thread, do everything on main thread */
  if (t == 1) {
    /* Initialize thread_arg struct (in header file) */
    thread_args args;
    args.inputGrid = input;
    args.outputGrid = output;
    args.targetSum = s;
    args.startRow = 0;
    args.endRow = input->n;
    args.mainThreadID = mainThreadID; // Set main thread id
    
    calcDiagSums(&args); // Main thread computes all diagonals
  } else {
    /* Else if > 1 thread and <= 3, create additional POSIX threads and structs */
    thread_args args[t]; // Array of t amount of pthread_create arguments
    pthread_t threadIDs[t - 1]; // Array to hold other (non-main) thread IDs
    
    /* Initialize thread_arg structs */
    for (int i = 0; i < t; i++) {
      args[i].inputGrid = input;
      args[i].outputGrid = output;
      args[i].targetSum = s;
      /* Calculate start rows for each thread: */
      /* eg. 9 rows: 1st (main) thread starts at row 0, 
	 2nd thread starts at row 3, and 3rd at row 6. */
      args[i].startRow = i * ((input->n) / t);
      /* Calculate end rows for each thread: */
      /* If last thread, end row is nth row, 
	 else, end row = start row of next thread */
      args[i].endRow = (i == t - 1) ? input->n : (i + 1) * ((input->n) / t);
      args[i].mainThreadID = mainThreadID; // Set main thread id

      /* If thread is not main, calculate diagonal sums in another thread */
      if (i > 0) {
	if (pthread_create(&threadIDs[i - 1], NULL, calcDiagSums, &args[i]) != 0) {
	  fprintf(stderr, "Failed to create additional POSIX threads.\n");
	} // if
      } // if
    } // for
    
    calcDiagSums(&args[0]); // Main thread calculation (pass in main thread args)
    
    /* Join additional threads to main after they all have terminated. */
    for (int i = 1; i < t; i++) {
      if (pthread_join(threadIDs[i - 1], NULL) != 0) {
	fprintf(stderr, "Failed to join additional POSIX threads to main.\n");
      } // if
    } // for
  } // else
} // diagonalSums

void *calcDiagSums(void *passedArgs) {
  /* Set thread args to the passed-in args */
  thread_args *args = (thread_args *)passedArgs;
  /* Hold args in new vars (for ease of access) */
  grid *input = args->inputGrid;
  grid *output = args->outputGrid;
  unsigned long targetSum = args->targetSum;
  int startRow = args->startRow;
  int endRow = args->endRow;
  pthread_t mainThreadID = args->mainThreadID;
  
  /* Calc diagonals from startRow to endRow until targetSum reached */
  for (int i = startRow; i < endRow; i++) {
    for (int j = 0; j < input->n; j++) {
      unsigned long sum = 0;      
      
      // Top-left to bottom-right diags
      for (int k = 0; i + k < input->n && j + k < input->n && sum < targetSum; k++) {
	sum += input->p[i + k][j + k];
	/* If target sum found, write all values in diag up to sum into output grid */
	if (sum == targetSum) {
	  sum = 0;
	  for (int l = 0; l <= k; l++) {
	  //for (int l = 0; i + l < input->n && j + l < input->n && sum <= targetSum; l++) {
	    if (output->p[i + l][j + l] == 0) {
	      output->p[i + l][j + l] = input->p[i + l][j + l]; // Write diagonal values
	    } // if
	  } // for
	} // if
	
      } // for

      sum = 0; // Reset sum
      
      // Top-right to bottom-left diags
      for (int k = 0; i + k < input->n && j - k >= 0 && sum <= targetSum; k++) {
	sum += input->p[i + k][j - k];
	/* If target sum found, write all values in diag up to sum into output grid */
	if (sum == targetSum) {	  
	  for (int l = 0; l <= k; l++) {
	    if (output->p[i + l][j - l] == 0) {
	      output->p[i + l][j - l] = input->p[i + l][j - l];	
	    } // if
	  } // for
	} // if
	
      } // for

    } // for
  } // for
  //

  /* Exit thread if not the main thread. */
  if (pthread_self() != mainThreadID) {
    pthread_exit(NULL);
  } // if
  return 0; /* else, if main thread, return NULL/0 */
} // calcDiagSums

void writeGrid(grid * g, char * fileName) {
  FILE *file = fopen(fileName, "w");
  if (file == NULL) {
    fprintf(stderr, "Output file failed to open.\n");
    exit(EXIT_FAILURE);
  } // if
 
  /* Write the output grid onto the output file, row-by-row */
  for (int i = 0; i < g->n; i++) { // For each row,
    for (int j = 0; j < g->n; j++) { // For each column,
      fprintf(file, "%u", g->p[i][j]); // Write each int as a char
    } // for    
    fprintf(file, "\n"); // Add newline character at end of row
  } // for
  
  fclose(file);
} // writeGrid

void freeGrid(grid * g) {
  for (int i = 0; i < g->n; i++) { // for each row,
    free(g->p[i]); // free columns
  } // for
  free(g->p); // free rows
} // freeGrid
