all:  master worker server client


# Target to compile worker files seperately#
worker: query_server.o worker.o patient_list.o hash.o tree.o date_list.o
	gcc -o  workers  query_server.o worker.o patient_list.o date_list.o hash.o tree.o -g -Wall

worker.o: ./masters/worker/code/worker.c
	gcc -g -c ./masters/worker/code/worker.c

patient_list.o: ./masters/worker/code/patient_list.c
	gcc -g -c  ./masters/worker/code/patient_list.c

date_list.o: ./masters/worker/code/date_list.c
	gcc -g -c  ./masters/worker/code/date_list.c

hash.o: ./masters/worker/code/hash.c
	gcc -g -c  ./masters/worker/code/hash.c

tree.o: ./masters/worker/code/tree.c
	gcc -g -c  ./masters/worker/code/tree.c

query_server.o: ./masters/worker/code/query_server.c
	gcc -g -c  ./masters/worker/code/query_server.c

# Target to compile Aggregator files seperately#
master: main.o pipe.o input_list.o
	gcc -o master main.o pipe.o input_list.o -g -Wall

main.o:	 ./masters/code/main.c
	gcc -c -g ./masters/code/main.c

pipe.o:	 ./masters/code/pipe.c
	gcc -c -g ./masters/code/pipe.c

input_list.o:	./masters/code/input_list.c
	gcc -c -g ./masters/code/input_list.c


#target to compile server files seperately#
server: server.o worker_list.o query_handler.o
	gcc -o whoServer server.o worker_list.o query_handler.o -pthread -Wall

server.o:	./server/code/server.c
	gcc -c -g ./server/code/server.c

worker_list.o: ./server/code/worker_list.c
	gcc -g -c  ./server/code/worker_list.c

query_handler.o: ./server/code/query_handler.c
	gcc -g -c  ./server/code/query_handler.c

#target to compile client files seperately#
client: client.o
	gcc -o whoClient client.o -pthread -Wall

client.o:	./client/client.c
	gcc -c -g ./client/client.c


clean:
	rm *.o
	rm master
	rm workers
	rm whoServer
	rm whoClient
