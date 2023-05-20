# Operating-system-that-implements-a-neural-network-architecture

This repository contains a C++ program that implements a neural network process. It allows the user to specify the number of hidden layers, neurons in each layer, and input/output values. The program utilizes multi-threading and inter-process communication using pipes.

## Prerequisites

Make sure you have the following installed on your system:

- C++ compiler (supporting C++11)
- POSIX threads (pthread) library

## Getting Started

1. Clone the repository to your local machine:
```
git clone <https://github.com/Vaneeza-7/Operating-system-that-implements-a-neural-network-architecture.git>
```

2. Compile the source code using the C++ compiler:

```
g++ neural_network.cpp -o neural_network -lpthread
```

3. Run the program:

```
./neural_network
```

## Usage

Follow the on-screen instructions to input the desired configuration of your neural network. You will be prompted to specify the number of hidden layers, the number of neurons in each layer, and the values for the input layer.

The program will create multiple processes and threads to simulate the neural network. Each process represents a layer, and each thread represents a neuron. The calculations for each neuron are performed concurrently using threads. The output of each neuron is printed to the console.

## Implementation Details

- The program uses a `pthread_mutex_t` mutex to synchronize access to shared data.
- The weights and input values are stored in global arrays: `weights` and `value`.
- The `n1` function represents a neuron in the input and output layers. It performs calculations based on the weights and input values.
- The `n2` function represents a neuron in the hidden layers. Currently, it does not contain any implementation and can be customized as per your requirements.
- Processes are created using `fork()`, and threads are created using `pthread_create()`.
- Pipes are used for inter-process communication. Each process/thread reads data from the previous process/thread and writes its output to the next process/thread.
- The program prints the process IDs to the console for reference.

## Contributing

Contributions to this repository are welcome. Feel free to open issues or submit pull requests to suggest improvements or fix any bugs.

## License

This project is licensed under the [MIT License](LICENSE).



