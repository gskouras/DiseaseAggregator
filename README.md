# DiseaseMonitor
### System Programming - First Assigment

Aim of this project is to build a software that receives, computes, manipulates and answers queries about diseases infections.

Specifically, a set of structures (hash tables, linked lists, binary trees) are implemented that allow the import and query of large patientRecord records.

An example of a patientRecord has the following attributes: 889 Mary Smith COVID-2019 China 25-1-2019 27-1-2019.
Complete set of records will be found in resources folder.

## Compilation and Running

```bash
make
```
```bash 
make run 
```
OR you can manually compile the programm by typing

```bash
gcc -o DiseaseMonitor main.c heap.c tree.c hash.c cli.c patient_list.c date_list.c
```
 And run it by typinng
 ```bash
  ./diseaseMonitor -p ./resources/sample -h1 # -h2 # -b #
 ```
  
  where -p is the input file,  h1 # h2 # are the size of hash table arrays -p and -b # is the size of Buckets in Bytes
