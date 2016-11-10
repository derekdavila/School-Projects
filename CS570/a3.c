/*
Name: Jesse Chhun  masc2013
Name: Derek Davila masc1970
file: a3.c
class: CS570 Summer 2016
notes: Assignment 3 a program to design an alarm clock with a timer
process a: clock thread
process b: alarm clock
process c: countdown timer
*/

#include <stdio.h>
#include <stdlib.h>
#include "a3.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

//Initialize global variables
sem_t FLAG;
int cdtimer;
int hrs,mins,secs;
char c;

/*
Runs countdown which output the countdown timer every second
*/
void run_CountDown(void * ptr){
    cdtimer = *((int *) ptr); //Cast cd timer
    printf("Starting countdown for %d seconds\n", cdtimer);
    sem_wait(&FLAG); //Decrement semaphore
    int mins,sec;
        while(cdtimer>=0){ //Run until the countdown equals 0
            if(cdtimer>60){          // if user enters second value greater than 60, change to minute:second
                mins=cdtimer/60;
                sec=cdtimer%60;
                printf("Countdown timer = %d minutes: %d seconds\n",mins,sec);
                sec--;
                cdtimer--;
            sem_post(&FLAG); //Increment semaphore
            sleep(1); //Sleep for 1 second to output every second
            }
            if(cdtimer<=60){
                printf("Countdown timer = %d seconds\n",cdtimer);
                cdtimer--;
            sem_post(&FLAG); //Increment semaphore
            sleep(1); //Sleep for 1 second to output every second
            }
        }

    pthread_exit(0); //Exit thread when finished
}


/*
Runs clock function which takes local system time and outputs the time every second
*/
void run_clock(void * ptr)
{
    cdtimer = *((int *) ptr); //Cast cdtimer
    time_t rawtime;
    struct tm * timeinfo; //Create struct to hold current time of day
    int hour;

    while(cdtimer >= 0)
    {
        sem_wait(&FLAG); //Decrement semaphore
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if(timeinfo->tm_hour > 12)
            hour = timeinfo->tm_hour - 12;
        else
            hour = timeinfo->tm_hour;
        printf("Time of day: %d:%d:%d\n", hour, timeinfo->tm_min, timeinfo->tm_sec);
        sem_post(&FLAG);  //Increment semaphore
        sleep(1); //Sleep for 1 second to output every second
    }

    pthread_exit(0); //Exit thread when finished
}

/*
Runs alarm clock which outputs if the user inputed time is reached while the program is running
*/
void run_alarm(void *ptr)
{
    cdtimer = *((int *) ptr); //Cast cdtimer
    time_t rawtime;
    struct tm * timeinfo; //Create struct to hold current time of day
    int hour, minute, second;
    while(cdtimer >= 0)
    {
        sem_wait(&FLAG); //Decrement semaphore
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if(timeinfo->tm_hour > 12)
            hour = timeinfo->tm_hour - 12;
        else
            hour = timeinfo->tm_hour;
        minute = timeinfo->tm_min;
        second = timeinfo->tm_sec;
        if(hour == hrs && minute == mins && second == secs)
            printf("ALARM ALARM ALARM ALARM ALARM ALARM ALARM\n");
        sem_post(&FLAG);    //Increment semaphore
        sleep(1);    //Sleep for 1 second to output every second
    }

    pthread_exit(0); //Exit thread when finished
}

/******
check input for x if not continue to get characters
until countdown ends
*********/
void run_check(void *ptr){
    while(c!='x')
    {
        c=getch();
    }
     pthread_exit(0);

}
/****************
create threads
run thread simulatiously
check timer for > 0
and if =0 ends

if user enters x, program quits

*****************/
void parent_process(void * ptr)
{
    cdtimer = *((int *) ptr); //Cast cdtimer

    //Create all needed threads
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;
    pthread_t thread_4;

    //Create threads for all functions to be run simultaneously
    pthread_create(&thread_1, NULL, (void *) &run_CountDown, (void *) &cdtimer);
    pthread_create(&thread_2, NULL, (void *) &run_clock, (void *) &cdtimer);
    pthread_create(&thread_3, NULL, (void *) &run_alarm, (void *) &cdtimer);
    pthread_create(&thread_4, NULL, (void *) &run_check, (void *) &cdtimer);

    //Check for user inputed "x" if found, cancel all threads
    while(cdtimer>0)
        {
        if(c == 'x' && cdtimer > 0)
        {
            pthread_cancel(thread_1);
            pthread_cancel(thread_2);
            pthread_cancel(thread_3);
            pthread_cancel(thread_4);
            printf("Process canceled by user\n");
            pthread_exit(0);
        }

        }

        //Wait for program threads to finish running is program is not canceled by user
        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);
        pthread_join(thread_3, NULL);

        //Cancel the last thread which was looking for the x input from the user
        pthread_cancel(thread_4);

        printf("Process canceled by Countdown\n");

    pthread_exit(0);

}



/************************************************************
http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux/7469410
this is the site where we found the getch() function
it copy pasted so we can use the function getch()
************************************************************/
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }




int main(int argc, char const *argv[])
{
    /*input validation*/

      /*checks to see if there are any inputs*/
        if(argv[1]==NULL){
            printf("Not enough inputs for program to run\n");
            return 1;
      }
        /*check to see if there are 4 inputs*/
      if(argc>5){
            printf("Too many input for program to run\n");
            return 1;
        }

    //Initualize semaphore to allow user between threads
    sem_init(&FLAG, 0, 1);

    //Take in command line arguments
    hrs = atoi(argv[1]);
    mins = atoi(argv[2]);
    secs = atoi(argv[3]);

    //Check if cdtimer value was given, if not use default
    if(argv[4] == NULL)
        cdtimer = 10;
    else
        cdtimer = atoi(argv[4]);


    printf("Enter x to stop the program\n");

    //Create and run parent process
    pthread_t parent;
    pthread_create(&parent, NULL, (void *) &parent_process, (void *) &cdtimer);
    pthread_join(parent, NULL);

    //Destoy semaphore when finished
    sem_destroy(&FLAG);

    return 0;
}