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
#define NUM_I 2
#define NUM_H 8
#define NUM_O 1

#define NUM_V 2

using namespace std;
   
   
   pid_t pid;
   double output;
   
   
struct input
{
  double values[NUM_V];
  double weights;
  //double bias;
    
};

//double sigmoid (double x) {
//    return 1 / (1 + exp(-x));
//}


void* n1(void* smth)
{
  
//   input* a = (input*)smth;
//   double w = a->weights;
//   //double b = a->bias;
//   double v = a->values[0];
//   
//   //double z = w*v + b;  //weight * value = bias
//   //output = sigmoid(z);

//   double z = v*w;
//   //write to pipe
//   
   exit(0);

}


int main()
{
   int layers;
  //making layers dynamically
  cout<<"---The Neural Network---"<<endl;
  cout<<"How many hidden layers do you want in your network?"<<endl;
  cin>>layers;
  layers+=2;
  
  input* data;
  data->values[0]=0.1;
  data->values[1]=0.2;
  
  //input layer is main itself
  
//       for(int i=0;i<NUM_T;i++)
//       {
//          pthread_create(&neuron[i], NULL, inp, (void*) data);
//          cout<<"thread created "<<i+1<<endl;
//       }
//       
//          
  
  //pipe
  
//  char buffer [200]="\0";
//   int fd[2];
//   int status;
//  
//   //read(0, buffer,10);
//   //write(1, buffer, 10);
//   pipe(fd);
  
  
  //====================================================PROCESSES=====
  for (int i=0; i < layers; i++)
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
           
           //making neurons dynamically
        
         if(i==0)
         {      
      
          pthread_t neuron[NUM_I];
        
         for(int j=0; j<NUM_I; j++)
         {
       
          pthread_create(&neuron[j], NULL, n1, (void*) data);
          cout<<"thread created "<<j+1<<endl;
         }
       

         for(int j=0;j<NUM_I;j++)
         {
          
          //thread will write to pipe
           pthread_join(neuron[j],NULL);
           cout<<"Thread"<<j<<" joined\n";
          
         }
           
           
           
        }
        
        else if(i==layers-1)
        {
             pthread_t neuron[NUM_O];
        
            for(int j=0; j<NUM_O; j++)
            {
       
            pthread_create(&neuron[j], NULL, n1, (void*) data);
            cout<<"thread created "<<j+1<<endl;
            }
       

            for(int j=0;j<NUM_O;j++)
            {
          
             //thread will write to pipe
             pthread_join(neuron[j],NULL);
             cout<<"Thread"<<j<<" joined\n";
          
            }
           
           
        }
        
        else
           {
           
               pthread_t neuron[NUM_H];
        
              for(int j=0; j<NUM_H; j++)
              {
       
              pthread_create(&neuron[j], NULL, n1, (void*) data);
              cout<<"thread created "<<j+1<<endl;
              }
       

              for(int j=0;j<NUM_H;j++)
              {
          
               //thread will write to pipe
               pthread_join(neuron[j],NULL);
               cout<<"Thread"<<j<<" joined\n";
              }
           
         
            }//end of else 
       
            pthread_exit(0);
            exit(0); 
           
        }//end of child
     }//end of for

}//end of main




