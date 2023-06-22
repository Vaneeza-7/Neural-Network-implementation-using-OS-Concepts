# Neural-Network-implementation-using-OS-Concepts

This repository contains a C++ program that implements a neural network process. The program reads weights from a configuration file, performs forward propagation using those weights, and computes the output based on the provided weights and data.
The program utilizes multi-threading and inter-process communication using pipes.

## Prerequisites

Make sure you have the following installed on your system:

- C++ compiler (supporting C++11)
- POSIX threads (pthread) library

## Getting Started

1. Clone the repository to your local machine:
```
git clone <https://github.com/Vaneeza-7/Neural-Network-implementation-using-OS-Concepts.git>
```

2. Compile the source code using the C++ compiler:

```
g++ new.cpp -o neural_network -lpthread
```

3. Run the program:

```
./neural_network
```

## Usage

Just give the path to the configuration file and the prgram reads the file for weights and input. For a different input, just change the configuration file and the code will run accordingly.
The program will create multiple processes and threads to simulate the neural network. Each process represents a layer, and each thread represents a neuron. The calculations for each neuron are performed concurrently using threads. The output of each neuron is printed to the console.

## Implementation Details

- The program uses a `pthread_mutex_t` mutex to synchronize access to shared data.
- It reads the weights from the `Configuration.txt` file and stores them in appropriate data structures.
- It also reads the input data from the `Configuration.txt` file.
- Performs forward propagation using the provided weights and input data.
- Computes the output of the neural network.
- The `n1` function represents a neuron in the input layer. It performs calculations based on the weights and input values.
- The `n2` function represents a neuron in the hidden layers. It performs calculations using the output of input layer and its weights.
- The `n3` function represents a neuron in the outer layer. It gives the output using the output of hidden layers and reads the final result from a pipe. 
- Processes are created using `fork()`, and threads are created using `pthread_create()`.
- Pipes are used for inter-process communication. Each process/thread reads data from the previous process/thread and writes its output to the next process/thread.
- The program prints the process IDs to the console for reference.


## Authors
- [Vaneeza](https://github.com/Vaneeza-7)
- [Rabail](https://github.com/Rabail-RN)


## License

This project is licensed under the [MIT License](LICENSE).




