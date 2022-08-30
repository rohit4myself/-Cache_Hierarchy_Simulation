# -Cache_Hierarchy_Simulation
Sharing profile analysis for Cache Hierarchies


**Warning: belady.cpp program for bzip2 (traces with two filess) takes around 3 hours. Please do not terminate 
the program. Smaller files such as hmmer, gromace and h26ref, the program will execute in some 15-17 mins. 
All other programs take less than a minute.


CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Input File
 * Running the program
  * Q1, Q2
 * Execution Time Details of each program
 * Contact Details


INTRODUCTION
------------

Hi!! This is the Assignment #1 submission of course CS622: Advance Computer Architecture

* Folder contains 4 cpp, 1 txt files and 1 empty folder named trace.
	* cache-inclusion.cpp :- Implements Inclusion cache hierarchy policy.
	* cache-NINE.cpp :- Implements Non-Inclusive Non-Exclusive cache hierarchy policy.
	* cache-exclusion.cpp :- Implements Exclusive cache hierarchy policy.
	* Q2.cpp
	* README.txt 
	* trace :- Folder is empty, binary trace files are needed to be placed here.

* Output of Each cpp file will be printed out in terminal itself.

INPUT FILE
-------------
* All input binary files are needed to be placed in the folder named "trace".


RUNNING THE PROGRAM
----------------

Q1 
------
-> For each policy there is a separate file- cache-<policy_name>.cpp 
-> Files are cache-inclusion.cpp (inclusion policy), cache-NINE.cpp (NINE policy), cache-exclusion.cpp (exclusion poilcy)
-> To execute the file you need to specify the number of traces and prefix of application name.
	$ ./cache-<policy_name> number_of_traces application_name_prefix

For Inclusion policy:
-------------
-> Command to execute the file:
	$ g++ cache-inclusion.cpp -o cache-inclusion
	$ ./cache-inclusion number_of_traces application_name_prefix (eg: ./cache-inclusion 2 bzip2)
Output
-------------
-> Results are printed on the terminal window itself.Results consist of L2 accesses, L2 hits,L2 misses,L3 hits and L3 misses

For NINE policy:
-------------
-> Command to execute the file:
	$ g++ cache-NINE.cpp -o cache-NINE
	$ ./cache-NINE number_of_traces application_name_prefix (eg: ./cache-NINE 2 bzip2)

Output
-------------
-> Results are printed on the terminal window itself.Results consist of L2 accesses, L2 hits,L2 misses,L3 hits and L3 misses

For Exclusion policy:
-------------
-> Command to execute the file:
	$ g++ cache-exclusion.cpp -o cache-exclusion
	$ ./cache-exclusion number_of_traces application_name_prefix (eg: ./cache-exclusion 2 bzip2)

Output
-------------
-> Results are printed on the terminal window itself.Results consist of L2 accesses, L2 hits,L2 misses,L3 hits and L3 misses

-------------
Q2
------
-> For each replacement policy corresponding to L3 fully associative cache, there is a separate file - replacementpolicy.cpp
-> lru.cpp: Q2 part1 (L3 3c's misses for LRU policy) 
-> belady.cpp: Q2 part2 (L3 3c's misses for Belady policy)
-> To execute the file you need to specify the number of traces and prefix of application name.

For LRU replacement policy:
-------------
->Command to execute the file:
            $ g++ -o lru lru.cpp
      	    $ ./lru number_of_traces application_name_prefix (eg: ./lru 2 bzip2 or ./lru 1 hmmer)
      	    
 Output
----------     	    
-> Results are printed on the terminal window itself. Results consist of L3 misses, L3 cold misses and L3 capacity misses. 


For Belady replacement policy:
-------------  
->Command to execute the file:
            $ g++ -o belady belady.cpp
      	    $ ./belady number_of_traces application_name_prefix (eg: ./belady 2 bzip2 or ./belady 1 hmmer)
 Output
----------     	    
-> Results are printed on the terminal window itself. Results consist of L3 misses, L3 cold misses and L3 capacity misses.   	   
            

EXECUTION TIME OF THE PROGRAMS
------------------------------
Q1
---------
All the program files will execute under one minute.

Q2
---------
Part 1 :- L3 Fully associative cache with LRU and cache inclusion policy.
----------
The program for this problem will execute under one minute for all the trace files.
----------
Part 2:-L3 Fully associative cache with Belady Min's Policy and cache inclusion policy.
----------
The program for this problem will vary with the file size, for smaller files such as hmmer, gromace and h26ref,
the program will execute in some 15 mins, where as for larger trace files the execution time may go upto 
3 hours.
------------------------------------------------------------------------------------------------------------------
The time may vary for different processors.
------------------------------------------------------------------------------------------------------------------
