all:  main.o
	gcc -o  diseaseMonitor main.o -g

main.o: main.c
	gcc -g -c main.c

clean: 
	rm  *.o
	rm diseaseMonitor
