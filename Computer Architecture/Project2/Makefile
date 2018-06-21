all: calc format
	gcc -Wall calc.c -o calc
	gcc -Wall format.c -o format

calc: calc.c calc.h
		gcc -Wall calc.c -o calc

		
format: format.c format.h
		gcc -Wall format.c -o format


clean:
		rm  calc
		rm  format
