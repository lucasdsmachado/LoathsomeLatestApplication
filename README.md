# HashBench - Hash Algorithm Performance Analyzer

A comprehensive benchmarking tool that implements and compares four different hashing collision resolution techniques. This program evaluates the performance of **Linear Probing**, **Double Hashing**, **Explicit Chaining**, and **Explicit Chaining without Joining** by measuring access counts and generating detailed performance reports.

## 🚀 Features

- **Linear Probing**: Resolves collisions by finding the next available slot in the hash table
- **Double Hashing**: Uses a secondary hash function to calculate step size for collision resolution
- **Explicit Chaining**: Implements linked lists to handle collisions with explicit pointer management
- **Explicit Chaining without Joining**: Modified chaining approach with specialized pointer handling
- **Performance Metrics**: Calculates average access counts for each hashing method
- **Automated Testing**: Runs tests across different load factors (10% to 90%) with multiple iterations

## 📁 File Structure

```
├── main.c                              # Main program implementation
├── record.h                            # Record structure definition
├── database.dat                        # Binary database file (generated)
├── file.txt                           # Input data file (generated)
├── README.md                          # This file
└── Output Files (generated):
    ├── linearProbing.txt              # Linear probing results
    ├── doubleHashing.txt              # Double hashing results
    ├── explicitChaining.txt           # Explicit chaining results
    └── explicitChainingNoJoining.txt  # Modified chaining results
```

## 🏗️ Record Structure

Each record in the hash table contains:

```c
typedef struct {
    bool occupied;    // Indicates if the slot is occupied
    int value;        // The stored integer value
    int pos;          // Position in the hash table
    int ptr;          // Pointer to next record (for chaining)
} Record;
```

## 🛠️ Compilation & Usage

### Prerequisites

- GCC compiler
- Standard C libraries

### Build

```bash
gcc -o hashbench main.c -lm
```

### Run

```bash
./hashbench
```

The program will automatically:

1. Generate test data with varying load factors (10% to 90%)
2. Apply each hashing method to the dataset
3. Calculate performance metrics
4. Generate detailed output files with results

## 📊 Performance Analysis

The program tests each algorithm across different load factors:

- **Load Factor Range**: 10% to 90% (in 5% increments)
- **Iterations per Load Factor**: 10 runs for statistical accuracy
- **Hash Table Size**: 997 slots (prime number for better distribution)
- **Metrics**: Average number of accesses per lookup operation

### Output Format

```
Current percentage: 50
Addr           Reg            Ptr
0              null           null
1              12345          null
2              67890          5
...
```

## 🔧 Core Functions

| Function                                        | Purpose                                       |
| ----------------------------------------------- | --------------------------------------------- |
| [`populateFile`](main.c)                        | Generates random test data                    |
| [`initializeFile`](main.c)                      | Initializes empty hash table                  |
| [`linearProbing`](main.c)                       | Implements linear probing algorithm           |
| [`doubleHashing`](main.c)                       | Implements double hashing algorithm           |
| [`explicitChaining`](main.c)                    | Implements explicit chaining                  |
| [`explicitChainingNoJoining`](main.c)           | Modified chaining implementation              |
| [`evaluateOpenAddressingTotalAccesses`](main.c) | Calculates access counts for open addressing  |
| [`evaluateChainedHashingTotalAccesses`](main.c) | Calculates access counts for chaining methods |

## 📈 Expected Results

The program provides insights into:

- **Linear Probing**: Performance degradation with clustering at high load factors
- **Double Hashing**: Better distribution but still subject to open addressing limitations
- **Explicit Chaining**: Consistent performance regardless of load factor
- **Modified Chaining**: Specialized behavior for specific use cases

## 🤝 Contributing

Feel free to contribute by:

- Adding new hashing algorithms
- Improving performance analysis
- Enhancing output formatting
- Adding visualization capabilities

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

**Note**: This tool is designed for educational and research purposes to understand the behavior and performance characteristics of different hashing collision resolution strategies.
