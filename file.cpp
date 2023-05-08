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
#define ws 8 //neurons for hidden layer

using namespace std;
//global variables
   	double weights[100][100]={{0.1, -0.2, 0.3, 0.1, -0.2, 0.3, 0.1, -0.2},
				{-0.4, 0.5, 0.6, -0.4, 0.5, 0.6, -0.4, 0.5}};
        double value[100]={0.1, 0.2}; //values atm 2
	
	int pipes[100][2];
	
//	 int** pipes = new int*[n];
//              for(int i=0; i<n; i++) {
//                     pipes[i] = new int[2];
//                      if(pipe(pipes[i]) < 0) {
//                             cout << "Failed to create pipe" << std::endl;
//                              exit(1);
//                                          }
//	}
	
	
	int subs;
   	pthread_mutex_t mutex1;
   	pid_t pid;
   	double output;
   	pthread_t p1,p2,p3,p4,p5,p6,p7,p8;
   	int count;


void* n1(void* smth)
{
  	pthread_mutex_lock(&mutex1);
   	int* a = (int*)smth;
	for(int i=0; i < ws; i++)
   	{
   	      
   		//cout << "Weights: " << weights[*a][i] << endl;
	   	double w1 = weights[*a][i];
	   	double z1 = value[*a]*w1;
	   	//cout<<"The values are " << z1 << endl;
	   	
	   	
	   	 int pipe_fd = pipes[i][1];
	   	 close(pipes[i][0]);
                  if (write(pipe_fd, &z1, sizeof(z1)) == -1) {
                          perror("write");
                     }
                    
                    close(pipes[i][1]);
                   
                   double buffer;
                  read(pipes[i][0], &buffer, sizeof(buffer));
                  cout << "Value read from pipe " << i << ": " << buffer <<endl;
                  close(pipes[i][0]);
                    
    	}
    	delete a;
    	
   
    	
   	pthread_mutex_unlock(&mutex1);
   	pthread_exit(NULL);
}

void* n2(void* smth)
{
  cout<<"Thread from hidden layer"<<endl;
  pthread_exit(NULL);
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
     		cout<<"Enter "<<i+1<<" value: ";
     		cin>>data[i];
  	}

  	layers+=2;
  
  	count=0;
  	
  	for (int i = 0; i < 100; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        } 
  	
     
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
		    	wait(&status);  //a wait loop??
		}
           
        	//child process runs following
     		if(pid[i] == 0)
     		{
           		int id = getpid();
           		cout<<"Process id"<<id<<endl;
           
           		//making neurons dynamically
           		
        		if(i==0)//input layer
         		{      
      
           			pthread_t* neuronI= new pthread_t[NUM_I];
        
                                 for(int j=0;j<NUM_I;j++)
         			{
		  			pthread_create(&neuronI[j], NULL, n1, (void*) new int(j));
		  			cout<<"thread created "<<j+1<<endl;
         			}

				for(int j=0;j<NUM_I;j++)
				{
				  
					//thread will write to pipe
					pthread_join(neuronI[j],NULL);
					cout<<"Thread"<<j<<" joined\n";
				} 
			}
		         else if(i==layers-1)//input layer
         		{      
      
           			pthread_t* neuronO= new pthread_t[NUM_O];
        
         			for(subs=0; subs<NUM_O; subs++)
         			{
       
		  			pthread_create(&neuronO[subs], NULL, n1, (void*) &data[subs]);
		  			cout<<"thread created "<<subs+1<<endl;
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
			    
			      	for(int j=0; j<NUM_H; j++)
			      	{
			      		pthread_create(&neuronH[j], NULL, n2, (void*) data);
			      		//cout<<"thread created "<<j+1<<endl;
              			}
     

			      	for(int j=0;j<NUM_H;j++)
			      	{
			  
				       //thread will write to pipe
				       pthread_join(neuronH[j],NULL);
				       cout<<"Thread"<<j<<" joined\n";
			      	}
         
         		}//end of else 
			pthread_exit(0);
			exit(0); 
		}//end of child
     }//end of for

}//end of main
