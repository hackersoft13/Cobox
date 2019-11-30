Pipator: pipe.o
	gcc -Wall pipe.o -o Pipator

pipe.o:
	gcc -c pipe.c


clean:
	rm *.o Pipator