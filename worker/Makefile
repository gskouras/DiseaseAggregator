all: query_server.o worker.o patient_list.o hash.o tree.o date_list.o heap.o
	gcc -o  worker  query_server.o worker.o patient_list.o date_list.o hash.o tree.o heap.o -g 

worker.o: Code/worker.c
	gcc -g -c Code/worker.c

patient_list.o: Code/patient_list.c
	gcc -g -c Code/patient_list.c

date_list.o: Code/date_list.c
	gcc -g -c Code/date_list.c

hash.o: Code/hash.c
	gcc -g -c Code/hash.c

tree.o: Code/tree.c
	gcc -g -c Code/tree.c

heap.o: Code/heap.c
	gcc -g -c Code/heap.c

query_server.o: Code/query_server.c
	gcc -g -c Code/query_server.c

clean: 
	rm  *.o
	rm worker

run:
	./worker