README 
Name:  Derek Davila  Jesse Chhun
Masc:  masc1970      masc2013
class: CS570 Assignment 2 

*USE masc2013 for grading*
--------------------------------------------------------------------------------------------
*Assignment information: 
Design and develop 3 page replacement algorithms 
for Optimal page replacement, second chance, and clock.
--------------------------------------------------------------------------------------------
*File names:
README.txt
a2.c 
a2.h
Makefile
pages.txt
--------------------------------------------------------------------------------------------
*compile instructions
make
./a2
--------------------------------------------------------------------------------------------
*Operating instructions:
pages.txt must be in the same directory as 
a2.c, a2.h, and Makefile

--------------------------------------------------------------------------------------------
*List of Novel/design decisions 
in all three algorithms 
we based our designs off of pages and frames array.
The first for loops initially checks and fills the page 
array with duplicates and from there the designs differ 

*for our Optimal page replacement function After we filled the array with inital values, we check for duplicates inside the page array, We find the value that is farthest away by checking how long it takes to come back to the array point and replace it.

*for our Second chance (SC) algorithm we started by creating 2 arrays to hold pages and uvals. The pages array would initially fill with our forloop that run while i<frames
catching all duplicates in the process and setting the uval to one. Once we filled the array we decided that the best way to link the uval with there page array was to create a pointer that held an index. The pointer would iterate through the array everytime a fault had occur. When the Pointer runs off the end it is reset to the start of the array which also simulates the clock algorithm.  

*To make our Clock Algorithm we took our second chance algorithm and used it as a base because they both used U-values. We changed the order in which the loop was initiated and had the U-values set while pages were being input into the array instead of if they were found again. We did run into a small bug when the u value array would end up in a spot where it would hold 2 consecutive 1's which would cause our program to skip changing one one of the uvalues to 0 so we replaced the if statement with a while loop.

--------------------------------------------------------------------------------------------
*List/descriptions of any extra features/algorithms/functionalies that are not required
N/A
--------------------------------------------------------------------------------------------
*List/descriptions of known bugs or deficiences 

*note for second chance algorithm we based our code 
using the following rules from this website:http://www.mathcs.emory.edu/~cheung/Courses/355/Syllabus/9-virtual-mem/SC-replace.html
we used this because the book didnt really give us an example for the algorithm, just a definition of the algorithm. So our Second Chance algorithm is based on this website rules which are similar to the book were just not sure if there exactly the same which would cause the wrong number of faults to be printed.

Pages.txt cannot take more then 100 elements, 
the array that holds the data is set to 100 so number of elements over 100 will cause the wrong output of faults
--------------------------------------------------------------------------------------------
Lessons Learned:
*Start early, for this program we started programing this bit by bit before the second exam, which also help us prepare for the exam.

*Used paper and pencil to walk through algorithms, Print statements were nice to see what was inside the array but what really help us catch alot of our bugs was 
using paper and pencil to walk through our algorithms.

*Read instructions carefully, On this assignment we began working on
a FIFO algorithm instead of the second chance due to missreading instructions 
so we modified our FiFO algorithm to match second chance algorithm
Because of this mistake however we learned that Second chance algorithm is similar to both FIFO and Clock. 

*While doing partner work make sure to pass and accept input in the same way, While working on this program, we realized that we were coding with different inital starting main functions which passed different values as frame and count
this would naturally throw off code we were writing and inturn make it so the partner could not test the other code without errors or obtaining different results.
--------------------------------------------------------------------------------------------
Data Analysis:
for our initial data test, we used the following sequence and numbers
inital test data set
Frames:3
Pages:2 3 2 1 5 2 4 5 3 2 5 2 
Results:
	OPT:6
	SC:6
	Clock:8
We initially chose this data set because it was a default sequence from powerpoint 3 (class lecture) which had results for both optimal and clock. In this sequence we can observed that SC algorithm was matching optimal while the clock algorithm was lagging behind.

If we increased the number of frames to 4 this sequence the following results occur
Frames 4:
	OPT:  5 
	SC:   6
	Clock:6

This time, We can see that SC and clock are not optimal but return the same number of page faults. If we increased the number of frames again to 5, all dataset return 5 because that is the number of different pages. Based off of this inital test we determine that SC was slightly better to clock because it was closer to optimal.

the next set of data we tried was the following 
Frames:3
Pages:2 17 4 7 6 4 2 6 3 6 2 4
Results:
	OPT:7
	SC:8
	Clock:8
this set of data had 12 pages like the previous version and 3 frames and we can see that sc and clock were both identical. They were also slower then optimal. When we increase this data set to 4 frames
Frames 4:
	OPT:6
	SC:7
	Clock:7
we find that the same results occur. however when we increase it to 5 frames we find that 
Frames 5:
	OPT:6
	SC:6 
	Clock:7
we find that SC matches optimal but clock remains consistant. When we hit 6 frames we have enough frames for all different number so all results are the same.

for our final set of data that we tested we tried the following
Frames: 3
Pages:1 2 2 3 3 4 1 2 2 3 3 4
Results:
	OPT:5
	SC:6
	Clock:8  
in this final, we check for duplicates and we repeat the set of number twice. Here we can see that there is a larger difference between SC and clock. Increasing the number of frames in this test case would result in the same results cause there are only 4 number but if we decrease the frames to 2 we get the following
frames: 2
	OPT:6
	SC:8
	Clock:8
Both SC and clock are slower then optimal. 

In conclusion we find that in general Second chance tends to be more optimal then clock. This could be because SC seems to be a mixture of both clock and fifo algorithm put together. As we increase the number of frames we see that all programes tend to return the same number of faults, however with limited number of frames we can see that SC is closer to optimal then clock.