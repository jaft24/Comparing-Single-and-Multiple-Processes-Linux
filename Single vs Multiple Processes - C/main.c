/*
Author: Jaleta Tesgera
CSC 474: Operating Systems Project 1
Date: March 28, 2023


Citations:
---------
How to section sums and have a practical application of Fork an Pipe: https://www.youtube.com/watch?v=6u_iPGVkfZ4
Undrstanding multiple children in fork declaration: https://www.youtube.com/watch?v=94URLRsjqMQ&list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY&index=6
How to read files in C: https://www.youtube.com/watch?v=8nIilb2kiSU
Using the Get time of the Day Function: https://www.techiedelight.com/find-execution-time-c-program/


General Program Explanation(Requirments):
----------------------------------------
This code will ask the user for two input. The first input will ask for the type of data from the number 1 to 3, and the second input will
ask for the number of child processes to run the sum. The choices for the child processes are 1,2 or 4. The code is generally going to add
a sum from a list of numbers using 1, 2, or 4 child process, along with the time it took to return the sum.


Design Detail Explanation:
--------------------------
The code starts by declargin a generall array called data_arr[]. Upon the user's choice of the data then the code gives this array a size
of either a 1000, 10000, 100000 lines as provided in the instruction. Then the  numbers will be processed into an array to make reading and
sum easier. Once we are done with reading the data, now we move to three big if statements that is based on the number of children.
For all the chosen processes the time starts counting as soon as we enter into the chosen number of processes. Initially all the variables
that will have the partial sums in the child process, the variables that will take these sums in the parent processes, and pipes are already
declared. My concept of creating children was to keep injecting new children into the parent proccess meaning once we are done with the first
child and move to the parent process with an else statemnt, I keep declaring a new fork in there so I can control all the children and the only
parent process they have. Some basic math will be used to add the numbers in the loop by manupilating a start and end line integer variables
declared at start. After we obtaina the partial sums, a read pipe will be declared that will hold the data and to be written at the respective
parent process. The number of pipes I declared and wait statements I called are the same as the child processes needed. In the parent process
I take all the partial sums from the children and print their sum result. The time function stops counting the time right there and returns the
time it took to report the final sum.


NB: I did not do error handling for pipe and fork creation (if -1), as it would just be a reddundancy of my code, and we have covered most of in class.






*/




// Start by including all the necessary libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h> 
#include <time.h>


int main(int argc, char *argv[])
{


 // Taking User Input
 int data_index, num_proccess;


 // Take the data number the user wants to use
 printf("Hello World! \nWelcome to Sum Processor Generator \n");
 printf("- - - - - - - - - - - - - - - - - \n \n");
 printf("There are three datas available with 1,000 lines, 10,000 lines, and 100,000 lines. \n");
 printf("Please choose one data with input 1, 2, or 3 respectively. \nIndex: ");
 scanf("\n%d", &data_index);


 // Take the number of processes the user wants to un the sums with
 printf("\nPlease choose the numbers of child processes to add your data \n");
 printf("The choices are 1, 2, or 4 processes \nNumber of child processes: ");
 scanf("\n%d", &num_proccess);


 // Report the users choice
 printf("\nThank you very much you have selected data number %d to run the sum with %d child processes \n", data_index, num_proccess);


 // Create a sequence of if statement to pick out the line size of the array since we need that to read the lines into the array
 int arr_size;
 if (data_index == 1)
 {
   arr_size = 1000; // the first data
 }
 else if (data_index == 2)
 {
   arr_size = 10000; // the second data
 }
 else if (data_index == 3)
 {
   arr_size = 100000; // the third data
 }


 // After taking the number of lines declare the array, the starting line, and the num variable that will momentarily take each lines value to make the array
 int data_arr[arr_size];
 int num;
 int data_line = 0;


 // Processing the requested data into the static array


 // File 1
 if (data_index == 1)
 {


   FILE *fp = fopen("src/file1.dat", "r");
   while (fscanf(fp, "%d", &num) == 1)
   {
     data_arr[data_line] = num;
     data_line++;
   }
   fclose(fp);
 }


 // File 2
 else if (data_index == 2)
 {


   FILE *fp = fopen("src/file2.dat", "r");
   while (fscanf(fp, "%d", &num) == 1)
   {
     data_arr[data_line] = num;
     data_line++;
   }
   fclose(fp);
 }


 // File 3
 else if (data_index == 3)
 {


   FILE *fp = fopen("src/file3.dat", "r");
   while (fscanf(fp, "%d", &num) == 1)
   {
     data_arr[data_line] = num;
     data_line++;
   }
   fclose(fp);
 }


 // Display this error message that if a wrong number is chosen and exit out of the program.
 /*
 Future Modifications for this section could be that instead of ending the code with an error this error handling could be
  moved to where the user will insert the data and execute a while loop that will keep asking for the appropritae number.
 */
 else
 {
   printf("Please enter the index only from the options provided \nData Index entry Error \n\n");
   return 1;
 }
  // End of processing data (by now we have the data in the array or we have exited the program due to an incorrect input)




 // Declare another static variable that will be used in all the processes to report the final sum
 int total_sum_report = 0;


// - - - - - - - - - - - - - - - - - - -  ONE CHILD PROCESS-- - - - - - - - - - - - - - - - - - - //


 if (num_proccess == 1)
 {
   // Sart measuring the time
    struct timeval start, end;
    gettimeofday(&start, NULL);


   // One child proccess needs one sum from child, and a partial sum both set to 0 for start
   int partial_sum1 = 0;
   int sum_from_child = 0;


   // Declare these variables to help with reading through the data
   int start_line, end_line;


  // One child process needs one pipe
   int pipe1[2];
   pipe(pipe1);


   // Start the forking process
   int id = fork();
   if (id == 0)
   {
     // Child Task executes here.
     // Since this is one child start at the begining and end at the end of the array
     start_line = 0;
     end_line = arr_size;


    // This loops through the array and adds all the quantities to partial_sum1
     for (int i = start_line; i < arr_size; i++){
         partial_sum1 += data_arr[i];
     }


     // Report the partial sum with one child process
     printf("\nThe Partial sum with one child is: %d", partial_sum1);


     // Write the partial sum to the pipe
     close(pipe1[0]);
     write(pipe1[1], &partial_sum1, sizeof(partial_sum1));
     close(pipe1[1]);


   }


   else
   {
     // Parent Task executes here.


      wait(NULL); // wait for one child process to finish so the parent can start




     // Read the vale from the pipe
     close(pipe1[1]);
     read(pipe1[0], &sum_from_child, sizeof(sum_from_child));
     close(pipe1[0]);


     total_sum_report = sum_from_child;


     // Report the final sum to the user
     printf("The Total sum from the parent process is: %d \n", total_sum_report);
    
   
    // End the time function and return the number of seconds taken by this process using the following simple math
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    double micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    double time_taken = micros / 1000000.00;


    printf("\nThe time taken to complete a task with one child process is %f seconds", time_taken);
    printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    printf("\n- - - - - - - - - - - - - - - - END OF CODE - - - - - - - - - - - - - - - -\n");


   }
 }




 // - - - - - - - - - - - - - - - - - - -  TWO CHILD PROCESS-- - - - - - - - - - - - - - - - - - - //


 else if (num_proccess == 2){
  // Two child proccess


  // Sart measuring the time
    struct timeval start, end;
    gettimeofday(&start, NULL);


  // Two child proccess needs two sum from childs, and two partial sums all set to 0 for start
  int partial_sum1 = 0; int partial_sum2 = 0;
  int sum_from_child1 = 0; int sum_from_child2 = 0;


  // Declare these variables to help with reading through the data
  int start_line, end_line;
   // Two child process need two pipes
   int pipe1[2];
   pipe(pipe1);


   int pipe2[2];
   pipe(pipe2);


   // Start the forking process
   int id = fork();
   if (id == 0)
   {
      // First child task executes here.
      // Since this is the  first child process start from the beginig and end half way
      start_line = 0;
      end_line = arr_size/2;


      // This loops through the array and adds the first half of the the quantities to partial_sum1
      for (int i = start_line; i < end_line; i++){
         partial_sum1 += data_arr[i];
     }
     // Report the partial sum to the user
     printf("\nThe Partial sum with the first child is:  %d", partial_sum1);
      
     // write the partail sum to the pipe 
     close(pipe1[0]);
     write(pipe1[1], &partial_sum1, sizeof(partial_sum1));
     close(pipe1[1]);






   }
   else
   {
       // Decalre the second fork
       int id2 = fork();
       if (id2 == 0)
       {
       // Second child task executes here.
       // Since this is the  second child process start half way and end at the last line.
      start_line = arr_size/2;
      end_line = arr_size;


      // This loops through the array and adds the second half of the the quantities to partial_sum2
      for (int i = start_line; i < end_line; i++){
         partial_sum2 += data_arr[i];
     }
     // Report the partial sum to the user
     printf("The Partial sum with the second child is: %d", partial_sum2);
    
     // write the partail sum to the pipe
     close(pipe2[0]);
     write(pipe2[1], &partial_sum2, sizeof(partial_sum2));
     close(pipe2[1]);


       }
       else {


     // Parent Task executes here.
     // call the wait function twice since there are two child processes the parent should wait to finish
     wait(NULL);
     wait(NULL);
     
     // Read the value from the first pipe
     close(pipe1[1]);
     read(pipe1[0], &sum_from_child1, sizeof(sum_from_child1));
     close(pipe1[0]);


      // Read the value from the second pipe
     close(pipe2[1]);
     read(pipe2[0], &sum_from_child2, sizeof(sum_from_child2));
     close(pipe2[0]);




     // add to the two partial sums to the static variable total sum
     total_sum_report = sum_from_child1 + sum_from_child2;


     // Report the final sum to the user
     printf("The Total sum from the parent process is: %d\n", total_sum_report);


  // End the time function and return the number of seconds taken by this process using the following simple math
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    double micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    double time_taken = micros / 1000000.00;


    printf("\nThe time taken to complete a task with one child process is %f seconds", time_taken);
    printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    printf("\n- - - - - - - - - - - - - - - - END OF CODE - - - - - - - - - - - - - - - -\n");




       }
   }


 }




// - - - - - - - - - - - - - - - - - - -  FOUR CHILD PROCESS-- - - - - - - - - - - - - - - - - - - //


 else if (num_proccess == 4)
 {
   // Four child Process


   // Sart measuring the time
    struct timeval start, end;
    gettimeofday(&start, NULL);


   // Four child proccess needs four sums from childs, and four partial sums all set to 0 for start
  int partial_sum1 = 0; int partial_sum2 = 0; int partial_sum3 = 0; int partial_sum4 = 0;
  int sum_from_child1 = 0; int sum_from_child2 = 0; int sum_from_child3 = 0; int sum_from_child4 = 0;


  // Declare these variables to help with reading through the data
  int start_line, end_line;




    // Four child process need four pipes
   int pipe1[2];
   pipe(pipe1);


   int pipe2[2];
   pipe(pipe2);


   int pipe3[2];
   pipe(pipe3);


   int pipe4[2];
   pipe(pipe4);




  // Start by calling the first fork
   int id = fork();
   if (id == 0)
   {
    // First child task executes here.
    // Since this is the  first child process start from the beginig and ends at the quarter.
      start_line = 0;
      end_line = arr_size/4;


      // This loops through the array and adds the first quarter of the the quantities to partial_sum1
      for (int i = start_line; i < end_line; i++){
         partial_sum1 += data_arr[i];
     }
     // report the partial sum to the user
     printf("\nThe Partial sum with the first child is:  %d", partial_sum1);
      
     // write the partail sum to the pipe 
     close(pipe1[0]);
     write(pipe1[1], &partial_sum1, sizeof(partial_sum1));
     close(pipe1[1]);


   }
   else
   {
       // Declare another fork for the second child
       int id2 = fork();
       if (id2 == 0)
       {
     // Second child task executes here.
     // Since this is the second child process start from the quarter and ends at half of the line.
      start_line = arr_size/4;
      end_line = arr_size/2;


     // This loops through the array and adds the second quarter of the the quantities to partial_sum2
      for (int i = start_line; i < end_line; i++){
         partial_sum2 += data_arr[i];
     }


     // Report the partial sum to user
     printf("The Partial sum with the second child is: %d", partial_sum2);


     // write the partial sum to the pipe
     close(pipe2[0]);
     write(pipe2[1], &partial_sum2, sizeof(partial_sum2));
     close(pipe2[1]);


       }
       else
       {
           // Declare another fork for the third child
           int id3 = fork();
           if (id3 == 0)
           {
     // Third child task executes here.
     // Since this is the third child process start from the half and ends at 3/4th of the line.
      start_line = arr_size/2;
      end_line = (3*arr_size)/4;


     // This loops through the array and adds the third quarter of the the quantities to partial_sum3
      for (int i = start_line; i < end_line; i++){
         partial_sum3 += data_arr[i];
     }


     // report the partial sum to user
     printf("The Partial sum with the third child is:  %d", partial_sum3);


     // write the partial sum to the pipe
     close(pipe3[0]);
     write(pipe3[1], &partial_sum3, sizeof(partial_sum3));
     close(pipe3[1]);        


           }
           else
           {
               // Declare the last fork for the fourth child
               int id4 = fork();
               if (id4 == 0)
               {


      // Fourth child task executes here.
      // Since this is the fourth child process start from the 3/4th line and ends at the last line.            
      start_line = (3*arr_size)/4;
      end_line = arr_size;


      // This loops through the array and adds the fourth quarter of the the quantities to partial_sum4
      for (int i = start_line; i < end_line; i++){
         partial_sum4 += data_arr[i];
     }
     // report the partial sum to the user
     printf("The Partial sum with the fourth child is: %d", partial_sum4);


     // write the partail sum to the pipe
     close(pipe4[0]);
     write(pipe4[1], &partial_sum4, sizeof(partial_sum4));
     close(pipe4[1]);  


   }
               else
               {
                        // Parent Task executes here.
                        // call the wait function four times since there are four child processes the parent should wait to finish
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);


     // read the values from the first pipe
     close(pipe1[1]);
     read(pipe1[0], &sum_from_child1, sizeof(sum_from_child1));
     close(pipe1[0]);


     // read the values from the second pipe
     close(pipe2[1]);
     read(pipe2[0], &sum_from_child2, sizeof(sum_from_child2));
     close(pipe2[0]);
    
     // read the values from the third pipe
     close(pipe3[1]);
     read(pipe3[0], &sum_from_child3, sizeof(sum_from_child3));
     close(pipe3[0]);


     // read the values from the fourth pipe
     close(pipe4[1]);
     read(pipe4[0], &sum_from_child4, sizeof(sum_from_child4));
     close(pipe4[0]);




     // add all the sums from the partial sums to the static variable total sum
     total_sum_report = sum_from_child1 + sum_from_child2 + sum_from_child3 + sum_from_child4;


     // Finally, report the total sum to the user
     printf("The Total sum from the parent process is: %d\n", total_sum_report);


   // End the time function and return the number of seconds taken by this process using the following simple math
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    double micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    double time_taken = micros / 1000000.00;


    printf("\nThe time taken to complete a task with one child process is %f seconds", time_taken);
    printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    printf("\n- - - - - - - - - - - - - - - - END OF CODE - - - - - - - - - - - - - - - -\n");




                }
           }
       }
   }


 }
 // Display this error message that if a wrong number is chosen for the process and exit out of the program.
 /*
 Future Modifications for this section could be that instead of ending the code with an error this error handling could be
  moved to where the user will insert the number of process and execute a while loop that will keep asking for the appropritae number.
 */
 else {
    printf("Please enter the process only from the options provided \nChild process entry Error \n\n");
    return 2;
 }
  // add final spacing (just a personal perference)
 printf("\n \n");
 return 0; 


}




 // - - - - - - - - - - - - - - - - - - -  END OF CODE- - - - - - - - - - - - - - - - - - - //



