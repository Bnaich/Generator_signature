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
# TODO
Add docker support
