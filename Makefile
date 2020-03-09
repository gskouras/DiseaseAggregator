all:  main.o patient_list.o
	gcc -o  diseaseMonitor main.o patient_list.o  -g

main.o: Code/main.c
	gcc -g -c Code/main.c

patient_list.o: Code/patient_list.c
	gcc -g -c Code/patient_list.c

# diseaseHT.o: Code/diseaseHT.c
# 	gcc -g -c Code/diseaseHT.c

clean: 
	rm  *.o
	rm diseaseMonitor
