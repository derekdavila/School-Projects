-----------------------------------------------------------------------------
Name:Jesse Chhun  masc2013
Name:Derek Davila masc1970
Class:Cs570 Summer2016 
Assignment information: an alarm clock program which takes an input
of Hrs:mins:Sec and a countdown timer.
FileName: README.txt 

Grade on masc____

------------------------------------------------------------------------------------------------------
File Manifest: 
a3.c 
a3.h 
Readme.txt
Makefile

-------------------------------------------------------------------------------------------------------
Compile instructions:
make 

-------------------------------------------------------------------------------------------------------
operating instructions: 

aclock (hrs) (mins) (sec)
Or
aclock (hrs) (mins) (sec) (cd)

where input of Hours mins and seconds are set to prepare an alarm.

Note that cd 
is not necessary to run the program, it will with a default time 
of 10 seconds. 

the alarm will not sound if it not reached within the given time based on Countdown timer

NOTE THAT THIS PROGRAM WILL PRINT Current time

so time input of 1-12 oclock 
it does NOT take military time as input

for example 1400 must be enter in as 2 for the program to run as intended 

------------------------------------------------------------------------------------------------------
List of discriptions and significant design decisions 

for this program we had three main threads and a parent thread to create all of the child threads.
The main threads we had are run_countdown, run_alarm, run_clock. and the parent function was parent_process which created the child threads (there actually 4 threads the forth one is decribed below in extra features/algorithms)

run_countdown is a thread that takes in the value cd (if one was given) in seconds and converts the value into min with leftover seconds. Then for every second the process runs, it sleeps for one second simulating time moving. 

run_alarm is a thread that gets the values of hours minutes and seconds given 
in the program input. Assuming that these input are numbers it sets an "alarm"
at that current time and compares the clock (current time) to the set alarm

run_clock is a thread that impliments time_t which grabs the current time every second to compare to run alarm  


*note that a key decision in this program is to take the millitary time that time_t gives us
and convert it to standard time. THe program does understand the difference between AM and PM but a print 
it not set. 

-----------------------------------------------------------------------------------------------------
List of discriptions of extra features or algorithms and functionalities that were not required

Our program runs with a getch() function given by the following website:
http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux/7469410

This function is included to search for the character input, it is included in a version of C called Ncurses 
but due to a lack of time we could not get our program to work with NCurses so instead we imported getch() function
manually.  


Run_check- this is a thread to check for an input x, it utilizes the getch() function to search for character input. 
	This was implemented because when we were searching for an input of x the program stop at the end of the count
	down timer and never terminated. So to solve this problem we move the check into a child process and terminated 
	the child if it was never found. 	

------------------------------------------------------------------------------------------------
list of deficiencies and bugs

there are no known deficencies as long as inputs
are numbers (there is no error checking for alphabet letters in Hrs, Mins, Sec)

-------------------------------------------------------------------------------------------------
lessons learned
threads cannot kill themselves the parent has to send a kill message 
this led us to build a run_check which was a child process of the parent so we can 
end the input check and terminate the program if the cd timer hit 0.


