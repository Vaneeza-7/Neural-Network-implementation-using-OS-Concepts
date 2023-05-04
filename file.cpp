#include<iostream>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>
#include<fstream>
#include<cmath>
#define NUM_T 5
using namespace std;
   
   
   pid_t pid;
   pthread_t  neuron1, neuron2, neuron3, neuron4;         //datatype for thread
   double output;
   
   
struct input
{
  double value;
  double weights;
  double bias;
    
};

double sigmoid (double x) {
    return 1 / (1 + exp(-x));
}


void* n1(void* smth)
{
  
   intput* a = (input*)smth;
   double w = a->weights;
   double b = a->bias;
   double v = a->value;
   
   double z = w*v + b;  //weight * value = bias
   output = sigmoid(z);
   
   
   

}


int main()
{
   int layers;
  //making layers dynamically
  cout<<"---The Neural Network---"<<endl;
  cout<<"How many hidden layers do you want in your network?"<<endl;
  cin>>layers;
 
  
  input* data;
  data->value=3;
  
  //input layer is main itself
  
       for(int i=0;i<NUM_T;i++)
       {
          pthread_create(&neuron[i], NULL, inp, (void*) data);
          cout<<"thread created "<<i+1<<endl;
       }
       
          
  
  //pipe
  
  char buffer [200]="\0";
   int fd[2];
   int status;
  
   //read(0, buffer,10);
   //write(1, buffer, 10);
   pipe(fd);
  
  for (int i=0; i< layers; i++)
     {
        //fork our process
           pid = fork();
        if (pid > 0)
          {  
             int id = getpid();
            //cout<<"Process id"<<id<<endl;
            //parent wait for child processes to end
            int status;
            wait(&status);
            
            
          }
           
        //child process runs following
        if(pid == 0)
        {
           int id = getpid();
           cout<<"Process id"<<id<<endl;
           //create an array of neurons in this layer
           pthread_t neuron[NUM_T];
           
       
       for(int i=0;i<NUM_T;i++)
       {
          pthread_create(&neuron[i], NULL, n1, (void*) data);
          cout<<"thread created "<<i+1<<endl;
       }

       
       for(int i=0;i<NUM_T;i++)
       {
          
          //thread will write to pipe
          pthread_join(neuron[i],NULL);
          
          cout<<"Thread"<<i<<" joined\n";
          
       }
           
           pthread_exit(0);
       
       
            exit(0); 
           
        }
     }

}




