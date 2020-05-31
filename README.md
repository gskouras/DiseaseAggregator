## Disease Aggregator

### Intoduction

Aim of this Project is to get acquainted with creating proccesses making use of the fork/exec system calls, inter-process communication(IPC) via named pipes and signals, the use of low-level I\O and bash script creation.

The goal is to implement a distributed information processing software that receives and asnwer queries about diseases infections. Speccifically, the application Disease Aggregator along with a series of Worker Procceses will co-operate to answer user's queries.
(See ./resources/hw2-spring-2020.pdf for more details)

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

The input directory can me mannually created by running files_create.sh executable(./resources/create_files/).

Execution
 ```bash
  ./create_files.sh diseaseFile countriesFile input_dir numFilesPerDirectory numRecordsPerFile
 ```
