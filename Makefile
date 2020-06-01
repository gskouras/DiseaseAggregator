all:  Aggregator Worker


# Target to compile worker files seperately#
Worker: query_server.o worker.o patient_list.o hash.o tree.o date_list.o
	gcc -o  workers  query_server.o worker.o patient_list.o date_list.o hash.o tree.o -g 

worker.o: ./worker/Code/worker.c
	gcc -g -c ./worker/Code/worker.c

patient_list.o: ./worker/Code/patient_list.c
	gcc -g -c  ./worker/Code/patient_list.c

date_list.o: ./worker/Code/date_list.c
	gcc -g -c  ./worker/Code/date_list.c

hash.o: ./worker/Code/hash.c
	gcc -g -c  ./worker/Code/hash.c

tree.o: ./worker/Code/tree.c
	gcc -g -c  ./worker/Code/tree.c

query_server.o: ./worker/Code/query_server.c
	gcc -g -c  ./worker/Code/query_server.c


# Target to compile Aggregator files seperately#
Aggregator: main.o pipe.o input_list.o
	gcc -o diseaseAggregator main.o pipe.o input_list.o

main.o:	 ./code/main.c
	gcc -c -g ./code/main.c

pipe.o:	 ./code/pipe.c
	gcc -c -g ./code/pipe.c

input_list.o:	./code/input_list.c
	gcc -c -g ./code/input_list.c

clean:
	rm *.o
	rm log*
	rm diseaseAggregator
	rm workers

run:
	./diseaseAggregator

val:
	valgrind --trace-children=yes --track-origins=yes --leak-check=full ./diseaseAggregator