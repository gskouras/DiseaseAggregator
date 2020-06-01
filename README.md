## Disease Aggregator

### [System Programming](http://cgi.di.uoa.gr/~mema/courses/k24/k24.html) - [Second Assigment](./resources/hw2-spring-2020.pdf)

Aim of this Project is to get acquainted with creating proccesses making use of the fork/exec system calls, inter-process communication(IPC) via named pipes and signals, the use of low-level I\O and bash script creation.

The goal is to implement a distributed information processing software that receives and asnwer queries about diseases infections. Speccifically, the application Disease Aggregator along with a series of Worker Procceses will co-operate to answers user defined queries.


### Implementation

First of all the application starts numWorkers Workers child processes and distributes subdirectories with the countries evenly to all worker proccesses and inform each Worker via named pipe for  the subdirectories which he will be undertaken.

Each Worker process, for each catalog assigned to it, reads all its files in chronological order based on file names and fills a series of data structures( hashtables, linked lists, binary trees )that it will use for answer questions asked by the parent process. Upon finishing, each worker will send, via named pipe, in parent process, summary statistics of the files he undertook.

When this procedure finish, the parent process receives [queries](./resources/manual.txt) via command line from user and send them to workers to proccess the input and calculate the results.

The architectural design between parent and child proccesses is depicted below.

>![alt text](./resources/parent-child.jpg "Communication")

### Compilation and Running

Compile
```bash
make
```
Execute with default parameters
```bash 
make run 
```
Execution with personalized parameters
 ```bash
  ./diseaseAggregator -w # -b # -i # 
 ```
where -w # is the number of worker proccesess, -b # is the buffer size for reading trough named-pipes and -i # is the path of the input directory

### Bash Script

The input directory can me mannually created by running files_create.sh executable(./resources/create_file

Execution
 ```bash
  ./create_files.sh diseaseFile countriesFile input_dir numFilesPerDirectory numRecordsPerFile
 ```
 
