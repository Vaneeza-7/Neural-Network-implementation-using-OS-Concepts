#include<iostream>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>
#include<fstream>
#define NUM_T 5

using namespace std;
   
   
   pid_t pid;
   pthread_t  neuron1, neuron2, neuron3, neuron4;         //datatype for thread
   
struct input
{
  float value;
  float weights;
  float bias;
    
};


int main()
{
   int layers;
  //making layers dynamically
  cout<<"---The Neural Network---"<<endl;
  cout<<"How many layers do you want in your netowrk?"<<endl;
  cin>>layers;
  
  
  for (int i=0; i< layers; i++)
     {
        //fork our process
           
        if (pid > 0)
          {  pid = fork();
             //parent wait for child processes to end
            int status;
            wait(&status);
            
            
            
          }
           
        //child process runs following
        if(pid == 0)
        {
           //create an array of neurons in this layer
           pthread_t neuron[NUM_T];
       
       for(int i=0;i<NUM_T;i++)
       {
          pthread_create(&neuron[i], NULL, n1, (void*) input);
          cout<<"thread created "<<i+1<<endl;
       }

       
       for(int i=0;i<NUM_T;i++)
       {
          
          
          
          pthread_join(neuron[i],NULL);
            
          
       }
           
           pthread_exit(0);
       
       
            exit(0); 
           
        }
     }




}
