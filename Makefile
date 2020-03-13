all:  main.o patient_list.o hash.o tree.o
	gcc -o  diseaseMonitor main.o patient_list.o hash.o tree.o -g

main.o: Code/main.c
	gcc -g -c Code/main.c

patient_list.o: Code/patient_list.c
	gcc -g -c Code/patient_list.c

hash.o: Code/hash.c
	gcc -g -c Code/hash.c

tree.o: Code/tree.c
	gcc -g -c Code/tree.c

clean: 
	rm  *.o
	rm diseaseMonitor
