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
#include <cstring>
#define NUM_V 2
#define ws 8 //neurons for hidden layer

using namespace std;
//global variables
   	double weights[100][100]={{0.1, -0.2, 0.3, 0.1, -0.2, 0.3, 0.1, -0.2},
				{-0.4, 0.5, 0.6, -0.4, 0.5, 0.6, -0.4, 0.5}};
        double value[100]={0.1, 0.2}; //values atm 2
	
	int pipes[100][2];
	
	bool inWCheck = false;
	bool inDataCheck = false;
	bool hWCheck = false;
	bool oWCheck = false;
	int inNeuronNum = 0;  //number of neurons of input layer
	int inWNum = 1;       //number of weights in 1 neuron of input layer
	int hNeuronNum = 0;   //number of neurons in hidden layer
	int hWNum = 1;        //number of weights in hidden layer
	int hLayerNum = 0;    //number of hidden layers
	int oWNum = 1;        //number of weights in output layer
	int lIndex = 0;       
	int hIndex = -1;       
	string str = "";         
	int totalLayers;     //total number of layers
	int oNeuronNum=0;
	
		
	int subs;
   	pthread_mutex_t mutex1;
   	pid_t pid;
   	double output;
   	pthread_t p1,p2,p3,p4,p5,p6,p7,p8;
   	int count=0;
   	
//objects

struct inputLayer
{
	double* values;
	double** iWeights;
};

struct hiddenLayer
{
	double** hWeights;
};

struct outputLayer
{
	double** oWeights;
};
   	
struct threadArg
{
   int subscript;
   int write_end;
   double** mulArrayIn;

}; 

struct hidArg
{
	double** arr;//array which is read buffer
	double** hw; //weights
};
  	
   	
//global variables

//inputLayer iLayer;   
//hiddenLayer* hLayer;
//outputLayer oLayer;
	
int global_write=0;
int global_read=0;
int counter=0;

//thread functions
void* n1(void* smth)
{
  	pthread_mutex_lock(&mutex1);
	inputLayer* s = (inputLayer*) smth;
	
	double* oneDplaceholder=new double[inWNum*inNeuronNum];

	double** mulArray=new double*[inNeuronNum];
	for(int i=0; i<inNeuronNum; i++)
	{
	    mulArray[i]=new double[inWNum]{0.0};
	 
	}
	
	for(int i=0; i<inNeuronNum; i++)
	{
	   for(int j=0; j<inWNum; j++)
	   {
	   
	         mulArray[i][j]=s->iWeights[i][j]* s->values[i];
	   
	   }
	
	
	}
	
//	converting  2d array to 1d
    	for(int i=0; i<inNeuronNum; i++)
    	{
    	   for(int j=0; j<inWNum; j++)
    	   {
    	      oneDplaceholder[i*inWNum+j]=mulArray[i][j];
    	   
    	   }
    	
    	}
    
//    	write oned array to pipe
          if(count==0){
    	   if (write(global_write, oneDplaceholder, inWNum * inNeuronNum* sizeof(double)) == -1) 
    	   {
                 perror("write");
                 exit(EXIT_FAILURE);
    	   }
            count++;
        }
   	pthread_mutex_unlock(&mutex1);
  	pthread_exit(NULL);
   
}

void* n2(void* smth)
{
   pthread_mutex_lock(&mutex1);
  cout<<"Thread from hidden layer"<<endl;
  
        hidArg* h = (hidArg*) smth;
	
	//huid arg has hlayer object called hw and 2d array
	
	double* added=new double[inWNum];
	
	for(int j=0;j<inNeuronNum;j++)
	{
	   for(int z=0; z<inWNum; z++)
            {
                    added[z] += h->arr[j][z];
                    cout<<"Added "<<added[z]<<endl;
                    //cout<<"Hidden array "<<h->hw[j][z]<<endl;
            
//		global_var+= h->arr[j]*weights[j][hcounter];
	    }
	    cout<<endl;
	}
	
	//multiplied values of input have been added in added array
         //now we have to multiply added array with weights of hidden layer 1
         //added array size: inWNum,,, weightsHiddenLayer1 size: hNeuronNum* hWNum
         
         double** res=new double*[hNeuronNum];
         for(int i=0; i<hNeuronNum; i++)
         {
              res[i]=new double[hWNum]{0.0};
         
         }
         
         
         //result should be 8*8
         for(int i=0; i<hNeuronNum; i++)
         {
            for(int j=0; j<hWNum; j++)
            {
                 res[i][j]=added[i]*h->hw[i][j];
                  //cout<<res[i][j]<<" ";
            }
          //  cout<<endl;
         
         }
         
         double* oneDplaceholder=new double[hWNum*hNeuronNum];

         	
//	converting  2d array to 1d
    	for(int i=0; i<hNeuronNum; i++)
    	{
    	   for(int j=0; j<hWNum; j++)
    	   {
    	      oneDplaceholder[i*hWNum+j]=res[i][j];    	   
    	   }
    	
    	}
         
         
          //only 1 thread will write to pipe
           if(counter==0)
           {
    	   if (write(global_write, oneDplaceholder, hWNum * hNeuronNum* sizeof(double)) == -1) 
    	   {
                 perror("write");
                 exit(EXIT_FAILURE);
    	   }
            counter++;
            
           }
           
           

  pthread_mutex_unlock(&mutex1);
  pthread_exit(NULL);
}

void* n3(void* arg)
{
  cout<<"In the else of hidden\n";
  pthread_exit(NULL);

}

int main()
{
	pthread_mutex_init(&mutex1,NULL); //mutex initialize
	
	//file reading 
	
	string line = "";
	string fileName = "Configuration.txt";
	fstream file;
	
	file.open(fileName.c_str(), ios :: in);						//Opening the file---------------------
	
	if (file.is_open())
	{
	
	//-------------------------------------- Reading from the file for checking number of neurons and layers etc. -----------------------------------------//
	
		while (getline(file,line))
		{
			//cout<<line<<endl;
			
			if (line[0] == 'I' || line[0] == 'i')					//Checking for input weights-----------
			{
				for (int i = 0; i < line.length(); i++)
				{
					if ((line[i] == 'w' || line[i] == 'W') && (line[i+1] == 'e' || line[i+1] == 'E'))
					{
						inWCheck = true;
						break;
					}
				}
			}
			
			if (inWCheck == true && (line[0] != 'I' && line[0] != 'i'))		//Counting the number of neurons of input layer----------
			{
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
					inNeuronNum++;
					if (inWNum == 1)
					{
						for (int i = 0; i < line.length(); i++)	//Counting the number of weights of input layer----------
						{
							if (line[i] == ',')
							{
								inWNum++;
							}
						}
					}
				}
				else
				{
					inWCheck = false;
				}
			}
			
			if (line[0] == 'h' || line[0] == 'H')
			{
				hLayerNum++;
				hWCheck = true;
			}
			
			if (hWCheck == true && (line[0] != 'h' && line[0] != 'H'))		//Counting the number of neurons of hidden layer ---------
			{
		
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
					hNeuronNum++;
					if (hWNum == 1)
					{
						for (int i = 0; i < line.length(); i++)	//Counting the number of weights of hidden layer----------
						{
							if (line[i] == ',')
							{
								hWNum++;
							}
						}
					}
				}
				else
				{
					hWCheck = false;
				}
			}
			
			if (line[0] == 'o' || line[0] == 'O')
			{
				oWCheck = true;
			}
			
			if (oWCheck == true && (line[0] != 'o' && line[0] != 'O'))		//Counting the number of neurons of output layer----------
			{
		
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
					
					oNeuronNum++;
					if (oWNum == 1)
					{
					
						for (int i = 0; i < line.length(); i++)	//Counting the number of weights of input layer----------
						{
							if (line[i] == ',')
							{
								oWNum++;
							}
						}
					}
				}
				else
				{
					oWCheck = false;
				}
			}
			
		}	// End While File Reading------
	}
	else
	{
		cout<<"File not open!"<<endl;
	}
	file.close();
		
	
	//------------------------------------------------ Making structures for storing weights and values --------------------------------------------------//
		
inputLayer iLayer;   
hiddenLayer* hLayer;
outputLayer oLayer;
		
		
		iLayer.values = new double[inNeuronNum];
		iLayer.iWeights = new double*[inNeuronNum];
		for (int i = 0; i < inNeuronNum; i++)
		{
			iLayer.iWeights[i] = new double[inWNum]{ 0.0 };
		}
		
		hNeuronNum/=hLayerNum;
		//hiddenLayer object array*
		 hLayer = new hiddenLayer[hLayerNum];
		for (int i = 0; i < hLayerNum;i++)
		{
			hLayer[i].hWeights = new double*[hNeuronNum];
			for (int j = 0; j < hWNum; j++)
			{
				hLayer[i].hWeights[j] = new double[hWNum]{ 0.0 };
			}
		}
		
		
		
		//outputLayer oLayer;
		oLayer.oWeights = new double*[oNeuronNum];
		for(int i=0; i<oNeuronNum; i++)
		{
		     oLayer.oWeights[i] = new double[oWNum];
		}
		
		
		//mulArrayInput
		threadArg arg;
		arg.mulArrayIn = new double* [inNeuronNum];
		for(int i=0; i<inNeuronNum; i++)
		{
		     arg.mulArrayIn[i]=new double [inWNum]{0.0};
		}
		
		arg.subscript=0;
		arg.write_end=0;
		
		double** mulArrayRead = new double* [inNeuronNum];
		for(int i=0; i<inNeuronNum; i++)
		{
		     mulArrayRead[i]=new double [inWNum]{0.0};
		}
		
		hidArg hObject;  //making object for hidden neuron
		
		hObject.arr = new double* [inNeuronNum];
		for(int i=0; i<inNeuronNum; i++)
		{
		     hObject.arr[i]=new double [inWNum]{0.0};
		}
		
		hObject.hw = new double* [hNeuronNum];
		for(int i=0; i<hNeuronNum; i++)
		{
		     hObject.hw[i]=new double [hWNum]{0.0};
		}
		
	//-------------------------------------- Reading from the file for storing weights and values of neurons -----------------------------------------------//
	file.open(fileName.c_str(), ios :: in);						//Opening the file---------------------
	
	if (file.is_open())
	{
	
		while (getline(file,line))
		{
			if (line[0] == 'I' || line[0] == 'i')					//Checking for input weights-----------
			{
				for (int i = 0; i < line.length(); i++)
				{
					if ((line[i] == 'w' || line[i] == 'W') && (line[i+1] == 'e' || line[i+1] == 'E'))
					{
						inWCheck = true;
						break;
					}
				}
			}
			
			if (inWCheck == true && (line[0] != 'I' && line[0] != 'i'))		//Counting the number of neurons of input layer----------
			{
			
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
				
					int j = 0;
					for (int i = 0; i < inWNum; i++)
					{
						str = "";
						
						while (line[j] != ',')
						{
						
							if ((line[j] >='0' && line[j] <= '9') || line[j] == '.' || line[j] == '-') 
							{
								str+=line[j];
							}
							j++;
						}
						str+="\0";
						j++;
						iLayer.iWeights[lIndex][i] = stod(str);
					}
					lIndex++;
				}
				else
				{
					lIndex = 0;
					inWCheck = false;
				}
			}
			
			if (inWCheck == false && (line[0] == 'I' || line[0] == 'i'))
			{
				inDataCheck = true;
			}
			
			if (inDataCheck == true && (line[0] != 'I' && line[0] != 'i'))
			{
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
					int j = 0;
					for (int i = 0; i < inNeuronNum; i++)
					{
						str = "";
						while (line[j] != ',')
						{
							if ((line[j] >='0' && line[j] <= '9') || line[j] == '.' || line[j] == '-') 
							{
								str+=line[j];
							}
							j++;
						}
						str+="\0";
						j++;
						iLayer.values[i] = stod(str);
					}
				}
			}
			
			if (line[0] == 'h' || line[0] == 'H')
			{
				hIndex++;
				lIndex = 0;
				hWCheck = true;
			}
			
			
			
			if (hWCheck == true && (line[0] != 'h' && line[0] != 'H'))		//Counting the number of neurons of hidden layer ---------
			{
			
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
				
					line+="\0";
					int k = 0;
					
					for (int i = 0; i < hWNum; i++)
					{
						
						str = "";
							
						for ( ;k<line.length();k++)
						{
							if (line[k]==',')
							{
								k++;
								break;
							}
							else
							{
								if ((line[k] >='0' && line[k] <= '9') || line[k] == '.' || line[k] == '-') 
								{
									str+=line[k];
									
								}
							}
						}
							
						str+="\0";	
						hLayer[hIndex].hWeights[lIndex][i] = stod(str);	
					}
						
					lIndex++;
					
				}
				else
				{
				
					lIndex = 0;
					hWCheck = false;
				}
			}
			
			if (line[0] == 'o' || line[0] == 'O')
			{
				oWCheck = true;
			}
			
		if (oWCheck == true && (line[0] != 'o' && line[0] != 'O'))		//Counting the number of neurons of output layer----------
			{
		
				if (!(line[0] >= 'A' && line[0] <= 'Z') && !(line[0] >= 'a' && line[0] <= 'z') && !(line.empty()))
				{
				
					int j = 0;
					for (int i = 0; i < oWNum; i++)
					{
						str = "";
						
						while (line[j] != ',')
						{
						
							if ((line[j] >='0' && line[j] <= '9') || line[j] == '.' || line[j] == '-') 
							{
								str+=line[j];
							}
							j++;
						}
						str+="\0";
						j++;
						oLayer.oWeights[lIndex][i] = stod(str);
					}
					lIndex++;
				
				}
				else
				{
					lIndex = 0;
					oWCheck = false;
				}
			}
			
		}
		
	}
	else
	{
		cout<<"File not open!"<<endl;
	}
	file.close();
	
	cout<<inNeuronNum<<endl;
	cout<<inWNum<<endl;
	cout<<hLayerNum<<endl;
	cout<<hNeuronNum<<endl;
	cout<<hWNum<<endl;
	cout<<oNeuronNum<<endl;
	cout<<oWNum<<endl;
	
	totalLayers=hLayerNum+2;
	//making layers dynamically
  	count=0;
  	
//  	for(int i=0; i<hLayerNum; i++)
//		{
//		  for(int j=0; j<hNeuronNum; j++)
//		  {
//		      for(int z=0; z<hWNum; z++)
//		      {
//		      
//		        cout<<hLayer[i].hWeights[j][z]<<" ";
//		      
//		      }
//		    cout<<endl;
//		  }
//		cout<<endl;
//		cout<<endl;
//		}
  	
  	
  	double* data = new double [inNeuronNum];
  	for(int i=0; i<inNeuronNum; i++)
  	{
     		cout<<"Enter "<<i+1<<" value: ";
     		cin>>data[i];
  	}

  	 
  	pid_t* pid = new pid_t[totalLayers];
  	
  	           			//creating pipe for process 1
           			int fd[2];
           			if (pipe(fd) == -1) {
                                     perror("pipe");
                                     exit(EXIT_FAILURE);
                                     }
                                     
                                     global_write=fd[1];
                                     global_read=fd[0];
  
  //====================================================PROCESSES=====//
	for (int i=0; i < totalLayers; i++)
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
      
           			pthread_t* neuronI= new pthread_t[inNeuronNum];
           			
//           			//creating pipe for process 1
//           			int fd[2];
//           			if (pipe(fd) == -1) {
//                                     perror("pipe");
//                                     exit(EXIT_FAILURE);
//                                     }
           			//char buffer[sizeof(inputLayer)];
           			
           			
           			//pass the wite end to thread argument
           			//arg.write_end=fd[1];
           			//arg.subscript=0;
           			
           			
           			
//                                  memcpy(buffer, &iLayer, sizeof(inputLayer));

//                                  int bytes_written = write(fd[1], buffer, sizeof(inputLayer));
//                                   if (bytes_written == -1) 
//                                   {
//                                      std::cerr << "Failed to write to pipe" << std::endl;
//                                       return 1;
//                                   }

//                                  close(fd[1]);
           			
           			
                                 for(int j=0;j<inNeuronNum;j++)
         			 {
         			        //pthread_create(&neuronI[j], NULL, n1, (void*) &arg); //thread subscript
		  		       if (pthread_create(&neuronI[j], NULL, n1, (void*) &iLayer) != 0) {
                                              fprintf(stderr, "Error creating thread %d.\n", i);
                                                         exit(1);
                                          }	
//		  			
         			 }
         			 
                                          
				for(int j=0;j<inNeuronNum;j++)
			        {
					pthread_join(neuronI[j],NULL);
					cout<<"Thread"<<j<<" joined\n";
				} 
				//close(fd[1]);
                                          
				 
				
			}//end of 1st layer/process
			
		         else if(i==totalLayers-1) //output layer
         		{      
                                
           			pthread_t* neuronO= new pthread_t[oNeuronNum];
        
         			for(subs=0; subs<oNeuronNum; subs++)
         			{
       
		  			pthread_create(&neuronO[subs], NULL, n1, (void*) &data[subs]);
		  			cout<<"thread created "<<subs+1<<endl;
         			}
       

				for(int j=0;j<oNeuronNum;j++)
				{
				  
					//thread will write to pipe
					pthread_join(neuronO[j],NULL);
					cout<<"Thread"<<j<<" joined\n";
				} 
			}	
			else if(i>0 && i<totalLayers-1)
        		{
        		        //read from pipe
//        		        if(i==1) //the first layer of hidden layer reads input data
//        		        {
//        		        if (read(fd[0], &iLayer, sizeof(inputLayer)) == -1) 
//		  			 {
//                                                perror("read");
//                                                exit(EXIT_FAILURE);
//                                           }
//                                          close(fd[0]);
//                                          
//                                          for(int j=0; j<inNeuronNum; j++)
//                                          {
//                                               cout<<"Value recieved is "<<iLayer.values[j]<<endl;
//                                               for(int z=0; z<inWNum; z++){
//                                                cout<<"Input data recieved" <<iLayer.iWeights[j][z]<<endl;
//                                              
//                                           }
//                                            
//                                            cout<<endl<<endl;
//                                         }
//        		        }
//        		        
        		        		 //read array from pipe to buffer
		      if(i==1) //if first hidden layer
		             { 
			         double buffer[inNeuronNum][inWNum];
			 		//the hidden layer is reading the input layer output	
	  			 if (read(fd[0], buffer, inWNum * inNeuronNum* sizeof(double)) == -1) 
	  			 {
                                        perror("read");
                                        exit(EXIT_FAILURE);
                                   }
                                  //close(fd[0]);
                                  //close(fd[1]);
                                  
                                  //printing the output of input layer i.e. buffer
                                 for(int j=0;j<inNeuronNum; j++)
         			{
         			      
         			   for(int z=0; z<inWNum; z++)
         			   { 
         			        
                                         cout<<"Buffer "<<buffer[j][z]<<endl;
         			   }
         			        cout<<endl;
         			}      
         		        //cout<<"Thread will be created soon \n";	
         			//creating hidden neurons i.e. 08
				
				
				
				for(int j=0; j<inNeuronNum; j++)
				{
				  for(int z=0; z<inWNum; z++)
				  {
				     
				       hObject.arr[j][z]=buffer[j][z];
				  }
				}
				
				
				cout<<"Value of HNeuronNUm before loop"<<hNeuronNum<<endl;
				
				for(int j=0; j<hNeuronNum; j++)
				{
				  for(int z=0; z<hWNum; z++)
				  {
				     hObject.hw[j][z]=hLayer[0].hWeights[j][z];
				     cout<<hObject.hw[j][z]<<" ";
				     //cout<<hLayer[0].hWeights[j][z]<<" ";
			          }
			          cout<<endl;
				}
				
				//hlayer is array of hiddenLayer objects
				//its size is hLayerNum
				
				
				cout<<"Value of HNeuronNUm "<<hNeuronNum<<endl;
				
				
				pthread_t* neuronH1= new pthread_t[hNeuronNum];
				
				for(int j=0; j<hNeuronNum; j++)
			      	{
			      		pthread_create(&neuronH1[j], NULL, n2, (void*) &hObject);
			      		cout<<"HIdden layer1 thread created "<<j+1<<endl;
              			}
     

			      	for(int j=0;j<hNeuronNum;j++)
			      	{
			  
				       //thread will write to pipe
				       pthread_join(neuronH1[j],NULL);
				       cout<<"Thread"<<j<<" joined\n";
			      	}
				
				
         			   
         	               }
         	           else
         	             {
         	                  
         	                  cout<<"Hi im process "<<i<<endl;
         	                  double buffer2[hNeuronNum][hWNum];
			 		//the hidden layer is reading the hidden layer output	
	  			  if (read(fd[0], buffer2, hWNum * hNeuronNum* sizeof(double)) == -1) 
	  			  {
                                        perror("read");
                                        exit(EXIT_FAILURE);
                                   }
                                   
                                   //close(fd[0]);
                                   //close(fd[1]);
                                  
                                   
                                     //printing the output of hidden layer i.e. buffer
                                 for(int j=0;j<hNeuronNum; j++)
         			{
         			      
         			   for(int z=0; z<hWNum; z++)
         			   { 
         			        
                                         cout<<"Buffer 2 :"<<buffer2[j][z]<<endl;
         			   }
         			        cout<<endl;
         			}
                                   
      			
    			         //creating hidden neurons i.e. 08
				pthread_t* neuronH= new pthread_t[hNeuronNum];
				
			      	for(int j=0; j<hNeuronNum; j++)
			      	{
			      	
			      		pthread_create(&neuronH[j], NULL, n3, (void*) &hObject);	
			      		cout<<"Thread created\n";
//			      		close(fd[0]); 
//					write(fd[1], global_var.c_str(), global_var.length());
//					close(fd[1]);
//					
//					read(fd[0], &array[j], sizeof(double));
//					close(fd[0]);
	      			}
	      			
			      	for(int j=0;j<hNeuronNum;j++)
			      	{
			  
				       //thread will write to pipe
				       pthread_join(neuronH[j],NULL);
				       cout<<"Thread"<<j<<" joined\n";
			      	}
		      	}    
         	                
			    
			      	
         
         		}//end of else 
			pthread_exit(0);
			exit(0); 
		}//end of child
     }//end of for

}//end of main
