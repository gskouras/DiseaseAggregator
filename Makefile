all:  master worker server client


# Target to compile worker files seperately#
worker: query_server.o worker.o patient_list.o hash.o tree.o date_list.o
	gcc -o  workers  query_server.o worker.o patient_list.o date_list.o hash.o tree.o -g -Wall

worker.o: ./master/worker/Code/worker.c
	gcc -g -c ./master/worker/Code/worker.c

patient_list.o: ./master/worker/Code/patient_list.c
	gcc -g -c  ./master/worker/Code/patient_list.c

date_list.o: ./master/worker/Code/date_list.c
	gcc -g -c  ./master/worker/Code/date_list.c

hash.o: ./master/worker/Code/hash.c
	gcc -g -c  ./master/worker/Code/hash.c

tree.o: ./master/worker/Code/tree.c
	gcc -g -c  ./master/worker/Code/tree.c

query_server.o: ./master/worker/Code/query_server.c
	gcc -g -c  ./master/worker/Code/query_server.c

# Target to compile Aggregator files seperately#
master: main.o pipe.o input_list.o
	gcc -o masters main.o pipe.o input_list.o -g -Wall

main.o:	 ./master/code/main.c
	gcc -c -g ./master/code/main.c

pipe.o:	 ./master/code/pipe.c
	gcc -c -g ./master/code/pipe.c

input_list.o:	./master/code/input_list.c
	gcc -c -g ./master/code/input_list.c


#target to compile server files seperately#
server: server.o worker_list.o
	gcc -o servers server.o worker_list.o -pthread -Wall

server.o:	./server/code/server.c
	gcc -c -g ./server/code/server.c

worker_list.o: ./server/code/worker_list.c
	gcc -g -c  ./server/code/worker_list.c

#target to compile client files seperately#
client: client.o
	gcc -o clients client.o -pthread -Wall

client.o:	./client/code/client.c
	gcc -c -g ./client/code/client.c


clean:
	rm *.o
	rm masters
	rm workers
	rm servers
	rm clients

# run:
# 	./masters

# val:
# 	valgrind --trace-children=yes --track-origins=yes --leak-check=full ./masters