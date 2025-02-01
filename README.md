# Multi-Threaded-Diagonal-Sums

Thai Nguyen (811-432-318)

**Multithreaded program using POSIX threads in C to compute diagonal sums in a 2D grid.**
Implements file I/O operations in a Unix environment, including opening, reading, writing, and safely closing files with proper permission handling.     
Utilizes pointers and proper dynamic memory allocation/deallocation.

## COMPILATION:
make compile
RUNNING:
make run

## FINDINGS:
When the grid is small for the main thread to compute by itself in a near insta\
ntaneous amount of time,
for example, similar to in1.txt, where the size is 5x5, it, then the use of add\
itional POSIX threads
will only serve to be unnecessary and only add to the computation time.

For example, in my finding of another example input grid that is 5x5:

    Elapsed time for computing the diagonal sums using 1 thread(s): 0.000386 seconds.
    Elapsed time for computing the diagonal sums using 3 thread(s): 0.001333 seconds.
