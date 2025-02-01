Thai Nguyen (811-432-318)

COMPILATION:
make compile
RUNNING:
make run

FINDINGS:
When the grid is small for the main thread to compute by itself in a near instantaneous amount of time,
for example, similar to in1.txt, where the size is 5x5, it, then the use of additional POSIX threads
will only serve to be unnecessary and only add to the computation time. 

For example, in my finding of another example input grid that is 5x5:

    Elapsed time for computing the diagonal sums using 1 thread(s): 0.000386 seconds.
    Elapsed time for computing the diagonal sums using 3 thread(s): 0.001333 seconds.

Thus, when the grid is small enough, additional POSIX threads do NOT serve to improve computation time as it would would large enough sizes.
