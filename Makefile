compile:
	gcc -Wall -pedantic-errors proj4.c -g -c -pthread
	gcc -Wall -pedantic-errors main.c -g -c
	gcc -Wall -pedantic-errors main.o proj4.o -g -o proj4.out -pthread

run:
	./proj4.out in3.txt out3.txt 3 3; diff out3.txt correctOut3.txt | wc -c;

clean:
	rm *.out
	rm *.o

