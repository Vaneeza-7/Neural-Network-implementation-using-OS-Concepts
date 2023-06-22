//---this file is not being actively used in the main program, but it contains a seperate implementation of file reading for reference.---//


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

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

int main()
{
	bool inWCheck = false;
	bool inDataCheck = false;
	bool hWCheck = false;
	bool oWCheck = false;
	int inNeuronNum = 0;
	int inWNum = 1;
	int hNeuronNum = 0;
	int hWNum = 1;
	int hLayerNum = 0;
	int oWNum = 0;
	int lIndex = 0;
	int hIndex = -1;
	string str = "";
	
	inputLayer iLayer;
	
	
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
					
					oWNum++;
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
		
		iLayer.values = new double[inNeuronNum];
		iLayer.iWeights = new double*[inNeuronNum];
		for (int i = 0; i < inNeuronNum; i++)
		{
			iLayer.iWeights[i] = new double[inWNum]{ 0.0 };
		}
		
		hNeuronNum/=hLayerNum;
		hiddenLayer* hLayer = new hiddenLayer[hLayerNum];
		for (int i = 0; i < hLayerNum;i++)
		{
			hLayer[i].hWeights = new double*[hNeuronNum];
			for (int j = 0; j < hWNum; j++)
			{
				hLayer[i].hWeights[j] = new double[hWNum]{ 0.0 };
			}
		}
		
		outputLayer oLayer;
		oLayer.oWeights = new double*[1];
		oLayer.oWeights[0] = new double[oWNum];
		
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
					str= "";
					int j = 0;
					while (j<line.length())
					{
						if ((line[j] >='0' && line[j] <= '9') || line[j] == '.' || line[j] == '-') 
						{
								str+=line[j];
						}
						j++;
					}
					str+="\0";
					j++;
					oLayer.oWeights[0][lIndex] = stod(str);
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
	cout<<oWNum<<endl;
	
	
	for(int i=0; i<5; i++)
	{
	   for(int j=0; j<8; j++)
	   {
	      for(int z=0; z<8; z++)
	      {
	          cout<<hLayer[i].hWeights[j][z]<<" ";
	      }
	        cout<<endl;
	   }
	      cout<<endl;
	}
	
	
	
	
	
	return 0;
}
