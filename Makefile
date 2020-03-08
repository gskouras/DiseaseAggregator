all:  main.o patient.o
	gcc -o  diseaseMonitor main.o patient.o -g

main.o: Code/main.c
	gcc -g -c Code/main.c

patient.o: Code/patient.c
	gcc -g -c Code/patient.c

clean: 
	rm  *.o
	rm diseaseMonitor
