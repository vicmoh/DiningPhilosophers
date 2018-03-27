/***************************
 * Name: Vicky Mohammad
 * Date: March 23, 2018
 ***************************/

--------------<<< HOW TO RUN THE PROGRAM >>>--------------
PLEASE READ!!! Due to using LinkList API, I have to organize with folders
such include for the header files To run the program.

* Type "make" to compile
(if inputFile.txt is in a folder use "./the_folder_name/inputFile.txt 
to run the program rather than just the name of the file or it will not find it".)

Example
* go to the file directory
* once compiled or "make",
* To run dine, type "./bin/dine 3 2" for example.
* To run holes, type "./bin/holes ./assets/data2.txt" or any "data.txt" of your liking.

-------------<<< RESTRICTION & LIMITATION >>>-------------
* Program might crash if there is comments in the input files
  So, make sure to have no comments for the input files
* if program file is not formated based on "alphbet space numbers" or any empty line, it may crash
* my ouput is 4 decimal places so, the answer should be around more less the same.
* program may crash if file is not the right extension, i tested on txt, so txt data file should work
* if you input the wrong file or file that doesnt exist it may crash or give 0.00 value ouput
so please make sure you run the program correctly :)

---------------------<<< REFERENCE >>>---------------------
* I use linklist API that I created from the CIS*2750 course and modified to have pop functions
* https://www.geeksforgeeks.org/program-next-fit-algorithm-memory-management/
* https://www.geeksforgeeks.org/dining-philosophers-solution-using-monitors/
* https://osandnetworkingcslab.wordpress.com/implementation-of-dining-philosophers-using-threads/

---------------------<<< REPORT >>>---------------------

--------------------
Dinning Philosophers
--------------------

I implemented using pthread and mutexes instead of semaphore, for some reason
semaphore does not work on windows bash. For Dining philosophers basically each philosophers
is represented as a thread, each thread will run onces, and takes turns eating a certain
amount of n times. Just when before the philosophers is eating the revelvant mutexes 
variables are lock. Once the philosophers ate, the mutexes variable are unlocked which
is n amount of times per thread. I created structure or object of philosophers which allow
the philosophers to pick using and assigned two forks, i represented them using integers and
macros which each correspond to the mutex variable array indexes, such that condition when
x is greater than 0, the philosophers is assigned fork or shopstick [x-1] and as well as [x]
, also second condition when x is equal to 0, the philosophers are assigned [#numOfFork - 1].
. For the wait or thinking times, i use 2 scenerio where sleap() for 2 seconds or 1 second
are posible wait times, these wait times are randomize between either 1 second or 2 second, 
hence the  output for running the program will alway be different.

----------------------
Hole Memory Management
----------------------

For the hole memory mamnagment i use a bitmap using and array of char with the size of 128
the rpcoess id which will be the letter is written or assign to the index map char array 
which will the simulation of the process being assign in representing in megabyte for each.
for the implementation, each implementation assumes that each process will be a unique, letter
alphabets from A to Z, if it is not a letter it may crash the program, hence process are 
limited to 26 characters. also, the size of the proccess required is equal or less than 128 mb and
greater than or eqaal to 1 mb within the 1 - 128mb. 


