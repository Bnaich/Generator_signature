# Generator_signature
File signature generation program. Uses CRC 32 hash function

Utility was tested on Ubuntu 18.04. 
Works optimally in a multiprocessor environment.

# How to use the utility

```bash
$ cd src
$ cmake . && make
$ ./generate_signature 

FULL path for inner file:
/home/bnaich/111.zip
ENTER the FULL path to the output file: 
/home/bnaich/1111.txt
Please Enter size of block in Kb: (min = 1Kb; max = 262144Kb; default = 1024Kb)
262144
Choose log level: (DEBUG = 1; INFO = 2; NO_LOG = 3 (default - minimum messages)
3
Loglevel set as default: NO_LOG
MESSAGE: START PROCESSING
	File: /home/bnaich/veeam software/src/Signature.cpp
	line: 137
MESSAGE: STOP PROCESSING
	File: /home/bnaich/veeam software/src/Signature.cpp
	line: 159
MESSAGE: DURATION in milliseconds: 31607
	File: /home/bnaich/veeam software/src/main.cpp
	line: 18


```
# If You prefer Docker
Run it to build image from Generator_signature directory:

```bash
$ ./docker-build 
```
Run the following command (where after key -v You have to set the full path to file being processed)

```bash
$ docker run -it -v /home/$(whoami)/data:/data test/generate-signature:latest 
FULL path for inner file:
/data/111.zip
ENTER the FULL path to the output file: 
/data/555444.txt
Please Enter size of block in Kb: (min = 1Kb; max = 262144Kb; default = 1024Kb)
200000
Choose log level: (DEBUG = 1; INFO = 2; NO_LOG = 3 (default - minimum messages)
1
Loglevel set as DEBUG
MESSAGE: START PROCESSING
	File: /programm/Signature.cpp
	line: 137
MESSAGE: START READING
	File: /programm/Signature.cpp
	line: 51
MESSAGE: START CALCULATING
	File: /programm/Signature.cpp
	line: 144
MESSAGE: START WRITING
	File: /programm/Signature.cpp
	line: 111
MESSAGE: READ BLOCK 1/25
	File: /programm/Signature.cpp
	line: 68
MESSAGE: READ BLOCK 2/25
	File: /programm/Signature.cpp
	line: 68
.......
MESSAGE: STOP PROCESSING
	File: /programm/Signature.cpp
	line: 159
MESSAGE: DURATION in milliseconds: 37782
	File: /programm/main.cpp
	line: 18

```
