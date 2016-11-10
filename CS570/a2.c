/*
Name: Jesse Chhun  Masc2013
Name: Derek Davila  Masc1970
Class: Cs570 Summer
Assignment 2: Page algorithms
This assignment will take a file name "pages.txt"
and run 3 different frame algorithms on the file
to determine the number of page faults
*/


#include <stdio.h>
#include <stdlib.h>

FILE *fp;

void run_OPT(int frames, int list[], int count){
    int size = frames+1;
    int page[size];
    int i, j, max, indexToRemove, iter;
    int faults = 0;
    int found = 0;
    int checkFill = 0;
    int foundInFuture = 0;
   
    for(j=0; j<=frames; j++){ page[j] = -1;}
   
    //printf("Enter OPT function\n");
    for(i=0; i<count; i++){
        found = 0;
        foundInFuture = 0;
        max = 0;
        if(i <= frames || checkFill <= frames){
            for(j=0; j<= frames; j++){ //Check for duplicates
                if(page[j] == list[i]){
                    found = 1;
                }
            }
            if(found != 1){
                page[checkFill] = list[i];
                faults++;
                //printf("Faults: %d\n",faults);
                checkFill++;
                //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
            }
        }
        else{
            //Check to see if current value is already in the pages array
            for(j = 0; j <= frames; j++){
                if(page[j] == list[i]){
                    found = 1;
                    break; //If found break loop
                }
            }
            if(found != 1){ //If the current value is not found
                for(j = 0; j <= frames; j++){ //Check each frame in the page
                iter = i;
                foundInFuture = 0;
                    while(iter < count){
                        if(list[iter] == page[j]) { //If found in future take last occuring value
                            if(iter > max){
                                indexToRemove = j;
                                max = iter;
                            }
                            foundInFuture = 1;
                            break;
                        }   
                        iter++;
                    }
                    if(foundInFuture == 0)
                    {
                        indexToRemove = j;
                        break;
                    }
                }
                page[indexToRemove] = list[i];
                faults++;
                //printf("Faults: %d\n",faults);
                //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
            }
        }   
    }
    fp = fopen("faults.txt", "a"); //Append to faults file
    if(fp != NULL){
        fprintf(fp, "Optimal Replacement Algorithm: %d\n",faults);
        fclose(fp);
    }
    printf("Optimal Replacement Algorithm: %d\n",faults);   
}


void run_secondChance(int frames, int list[],int count){
    int size = frames + 1;
    int page[size];                /*creates an array size frames to use for storage*/
    int uval[size];            //Array to hold uval for each value in pages array
    int faults=0;                    /*counter for number of faults*/
    int i,j,indexToRemove,spot;  /* list of index*/
    int found;                        /* 0=new number, 1=duplicate in page array*/
    int checkFill = 0;        //Used to check if pages array is full
    int temppage;                    /*check variable*/
    int templist;
    int ptr=0;
    int changed = 0;

    //printf("\nEnter SecondChance(FIFO) function\n");
    //printf("Number of frames used:%d",size);
    //printf("\nSize of the array:%d\n",count);
   
    for(j=0; j<=frames; j++){ page[j] = -1;}
    for(j=0; j<=frames; j++){ uval[j] = 0;}

        for(i=0;i<count;i++){ 
            found=0;
            /*duplicate check with spaces in pages array*/
            if(i<= frames || checkFill <= frames){
                  for(j=0;j<=frames;j++){
                        if (page[j]==list[i]){
                            found=1;
                    uval[j] = 1;
                        }
                    }
                    if(found!=1){
                //printf("Pointer: %d\n", ptr);
                        page[checkFill]=list[i];
                        faults++;
                ptr++;
                checkFill++;
                if(ptr > frames){ptr = 0;}
                //printf("Fault: %d\n",faults);
                //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
                //for(j=0; j<=frames;j++){printf("UVAL: %d at count %d\n",uval[j],i);}
                    }
            }
       
              else{
                    for(j=0;j<=frames;j++){ //Duplicate Check
                        if (page[j]==list[i]){
                            found = 1;
                    uval[j] = 1;
                        }
                    }
           
                    if(found != 1){
                while(uval[ptr] != 0){
                    uval[ptr] = 0;
                    ptr++;
                    if(ptr > frames){ptr = 0;}
                }
               
                        page[ptr]=list[i];
                uval[ptr]=0;
                      faults++;
                      ptr++;
                if(ptr > frames){ptr = 0;}
                //printf("Pointer: %d\n",ptr);
                //printf("Fault: %d\n",faults);

            }
                    //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
                //for(j=0; j<=frames;j++){printf("UVAL: %d at count %d\n",uval[j],i);}
              }
     
    }
    fp = fopen("faults.txt", "a"); //Append to faults file
    if(fp != NULL){
        fprintf(fp, "Second Chance Replacement Algorithm: %d\n",faults);
        fclose(fp);
    }
    printf("Second Chance Replacement Algorithm: %d\n",faults);
}

void run_clock(int frames, int list[],int count){
    int size = frames + 1;
    int page[size];                /*creates an array size frames to use for storage*/
    int uval[size];            //Array to hold uval for each value in pages array
    int faults=0;                    /*counter for number of faults*/
    int i,j,indexToRemove,spot;  /* list of index*/
    int found;                        /* 0=new number, 1=duplicate in page array*/
    int checkFill = 0;        //Used to check if pages array is full
    int temppage;                    /*check variable*/
    int templist;
    int ptr=0;
    int changed = 0;

    //printf("\nEnter Clock function\n");
    //printf("Number of frames used:%d",size);
    //printf("\nSize of the array:%d\n",count);
   
    for(j=0; j<=frames; j++){ page[j] = -1;}
    for(j=0; j<=frames; j++){ uval[j] = 0;}

        for(i=0;i<count;i++){ 
            found=0;
            /*duplicate check with spaces in pages array*/
            if(i<= frames || checkFill <= frames){
                  for(j=0;j<=frames;j++){
                        if (page[j]==list[i]){
                            found=1;\
                    uval[j] = 1;
                        }
                    }
                    if(found!=1){
                //printf("Pointer: %d\n", ptr);
                        page[checkFill]=list[i];
                uval[checkFill] = 1;
                        faults++;
                ptr++;
                checkFill++;
                if(ptr > frames){ptr = 0;}
                //printf("Fault: %d\n",faults);
                        //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
                //for(j=0; j<=frames;j++){printf("UVAL: %d at count %d\n",uval[j],i);}
                    }
            }


              else{
                    for(j=0;j<=frames;j++){ //Duplicate Check
                        if (page[j]==list[i]){
                            found = 1;
                    uval[j] = 1;
                        }
                    }
           
                    if(found != 1){
                while(uval[ptr] != 0){
                    uval[ptr] = 0;
                    ptr++;
                    if(ptr > frames){ptr = 0;}
                }
               
                        page[ptr]=list[i];
                uval[ptr]=1;
                      faults++;
                      ptr++;
                if(ptr > frames){ptr = 0;}
                //printf("Pointer: %d\n",ptr);
                //printf("Fault: %d\n",faults);

            }
                //for(j=0; j<=frames;j++){printf("Page: %d at count %d\n",page[j],i);}
                //for(j=0; j<=frames;j++){printf("UVAL: %d at count %d\n",uval[j],i);}
              }
   

       
    }
    fp = fopen("faults.txt", "a"); //Append to faults file
    if(fp != NULL){
        fprintf(fp, "Clock Replacement Algorithm: %d\n",faults);
        fclose(fp);
    }
    printf("Clock Replacement Algorithm: %d\n",faults);
}
/*main program
opens pages.txt, put data into array
runs test, OPT, second chance, Clock
prints pages faults

*/

int main(int argc, char const *argv[]){
   
    fp=fopen("pages.txt","r");

    int frames;                                          /*first number in txt*/ 
    int list[25];                                        /*the rest of the doc*/
    int i = 0;
    int num;                                              /*index*/
    int count=0;                                        /*number of things in array*/

    if (fp == NULL){                                    /*checks for pages.txt*/
        printf("pages.txt not found\n");
        return 1;
    }
    else {                                              /*if found copy data into array*/
        printf("pages.txt found\n");
        fscanf(fp,"%d",&frames);    //Get number of frames in first line
        printf("Frames: %d\n", frames);
        frames = frames - 1;         //Get 0 based number of frames
        for(i=0; i<25; i++){
            fscanf(fp, "%d", &num);
            if(feof(fp)) break;
            list[i] = num;
            count++;
            //printf("\nlist[%d]=%d",i,list[i]);
        }           
    }
    printf("Faults: \n");
   
    fclose(fp);

    fp = fopen("faults.txt", "w+"); //Create file to write output
    if(fp!= NULL){
        fputs("Faults: \n", fp);
        fclose(fp);
    }   

    //Run Optimal Replacement Algorithm
    run_OPT(frames, list, count);
    //Run function Second Chance
    run_secondChance(frames, list, count);
    //Run function clock*/
    run_clock(frames, list, count);
   
    return 0;
}
