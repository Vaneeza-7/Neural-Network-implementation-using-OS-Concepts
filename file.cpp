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
#define NUM_V 2
#define ws 8

using namespace std;
   
   double weights1[ws]={0.1, -0.2, 0.3, 0.1, -0.2, 0.3, 0.1, -0.2};
   double weights2[ws]={-0.4, 0.5, 0.6, -0.4, 0.5, 0.6, -0.4, 0.5};
   
   
   int subs;
   pthread_mutex_t mutex1;
   pid_t pid;
   double output;
   pthread_t p1,p2,p3,p4,p5,p6,p7,p8;
   int count;
   
struct input
{
  double values[NUM_V];
  //double weights;
    
};


void* n1(void* smth)
{
  pthread_mutex_lock(&mutex1);
  cout<<"Hello"<<endl;
  
   double* a = (double*)smth;
   double v = a;
   if(subs==0){
   for(int i=0; i<ws; i++){
   
   double w1 = weights1[i];
   double z1 = v*w1;
   cout<<"The first value written to pipe is"<<z1<<endl;
    }
   }
   if(subs==1){
   for(int i=0; i<ws; i++){
   double w2 = weights2[i];
   double z2=v2*w2;  
   
   cout<<"The second value written to pipe is "<<z2<<endl;
   }
  }
   count++;
   //write to pipe
   
//   int fd[2];
//   pipe(fd);
//   close(fd[0]);
//   write(fd[1], &z1, sizeof(double));
//   write(fd[1], &z2, sizeof(double));
//   close(fd[1]);
//   
//   
   
   pthread_mutex_unlock(&mutex1);
   exit(0);

}

void* n2(void* smth)
{
  cout<<"Thread from hidden layer"<<endl;
  return NULL;
}

int main()
{
  pthread_mutex_init(&mutex1,NULL);
   int layers, NUM_I, NUM_H, NUM_O;
  //making layers dynamically
  cout<<"---The Neural Network---"<<endl;
  cout<<"How many hidden layers do you want in your network?"<<endl;
  cin>>layers;
  cout<<"How many neurons do you want in your input layer?"<<endl;
  cin>>NUM_I;
  
  double* data = new double [NUM_I];
  
  cout<<"How many neurons do you want in your hidden layer?"<<endl;
  cin>>NUM_H;
  cout<<"How many neurons do you want in your output layer?"<<endl;
  cin>>NUM_O;
  for(int i=0; i<NUM_I; i++)
  {
     cout<<"Enter "<<i+1<<" value";
     cin>>data[i];
  }

  
  
  layers+=2;
  
//  double v=0.1;
//  input data;
//  for(int i=0; i<NUM_V; i++)
//  {
//     data.values[i]=v;
//     v=v+0.1; 
//  }
//  

  
  
  count=0;
     
  pid_t* pid = new pid_t[layers];
  
  
  //====================================================PROCESSES=====
  for (int i=0; i < layers; i++)
     {
        //fork our process
           pid[i] = fork();
        if (pid[i] > 0)
          {  
             int id = getpid();
            //cout<<"Process id"<<id<<endl;
            //parent wait for child processes to end
            int status;
            wait(&status);
            
            
          }
           
        //child process runs following
     if(pid[i] == 0)
     {
           int id = getpid();
           cout<<"Process id"<<id<<endl;
           
           //making neurons dynamically
        
         if(i==0)
         {      
      
           pthread_t* neuronI= new pthread_t[NUM_I];
        
         for(subs=0; subs<NUM_I; subs++)
         {
       
          pthread_create(&neuronI[subs], NULL, n1, (void*) data[subs]);
          cout<<"thread created "<<subs+1<<endl;
         }
       

         for(int j=0;j<NUM_I;j++)
         {
          
          //thread will write to pipe
           pthread_join(neuronI[j],NULL);
           cout<<"Thread"<<j<<" joined\n";
         }
           
           
           
        }
        
        else if(i==layers-1)
        {
             pthread_t* neuronO= new pthread_t[NUM_O];
        
            for(int j=0; j<NUM_O; j++)
            {
       
            pthread_create(&neuronO[j], NULL, n2, (void*) data);
            cout<<"thread created "<<j+1<<endl;
            }
       

            for(int j=0;j<NUM_O;j++)
            {
          
             //thread will write to pipe
             pthread_join(neuronO[j],NULL);
             cout<<"Thread"<<j<<" joined\n";
          
            }
           
           
        }
        
        else if(i>0 && i<layers-1)
           {
           
           pthread_t* neuronH= new pthread_t[NUM_H];
        
           
           pthread_create(&p1, NULL, n2, (void*) data);
           pthread_create(&p2, NULL, n2, (void*) data);
           pthread_create(&p3, NULL, n2, (void*) data);
           pthread_create(&p4, NULL, n2, (void*) data);
           pthread_create(&p5, NULL, n2, (void*) data);
           pthread_create(&p6, NULL, n2, (void*) data);
           pthread_create(&p7, NULL, n2, (void*) data);
           pthread_create(&p8, NULL, n2, (void*) data);
            
//              for(int j=0; j<NUM_H; j++)
//              {
//       
//              pthread_create(&neuronH[j], NULL, n1, (void*) data);
//              cout<<"thread created "<<j+1<<endl;
//              }
//       

//              for(int j=0;j<NUM_H;j++)
//              {
//          
//               //thread will write to pipe
//               pthread_join(neuronH[j],NULL);
//               cout<<"Thread"<<j<<" joined\n";
//              }
//           
         
            }//end of else 
            
                  
            pthread_exit(0);
            exit(0); 
           
        }//end of child
     }//end of for

}//end of main




