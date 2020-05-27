all:	main.o pipe.o input_list.o
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

run:
	./diseaseAggregator

val:
	valgrind --trace-children=yes ./diseaseAggregator