# Shortest Path & Traveling Salesman Problem (CO1007)

A C++ implementation of graph theory algorithms including the **Bellman-Ford Algorithm** and the **Traveling Salesman Problem (TSP)** using the **Bellman-Held-Karp Dynamic Programming** approach and optimization heuristics. Developed for the **Discrete Structures (CO1007)** course at Ho Chi Minh City University of Technology (HCMUT - VNU-HCM), Semester 242.

## Overview

This project implements classical graph algorithms to solve single-source shortest path problems and the NP-hard Traveling Salesperson Problem on weighted directed graphs:

### 1. Bellman-Ford Algorithm (`bellman.h`, `bellman.cpp`)
- **Step-by-step Execution (`BF`)**: Performs a single relaxation pass over sorted edge lists, updating distance (`BFvalue`) and predecessor (`BFprev`) arrays.
- **Shortest Path Reconstruction (`BF_Path`)**: Executes full Bellman-Ford relaxations ($|V| - 1$ iterations) and reconstructs the optimal path from source to target vertex using predecessor backtracking.

### 2. Traveling Salesman Problem (`tsm.h`, `tsm.cpp`)
- **Exact Solution (Bellman-Held-Karp algorithm)**: Dynamic programming with bitmasking ($O(n^2 2^n)$ time complexity) for graphs with $N \le 20$ vertices to find the exact minimum-cost Hamiltonian circuit.
- **Heuristic Approximation**: Greedy nearest neighbor and local search optimization strategies for larger graph instances where exact exponential search is computationally intractable.

### 3. Technical Report (`report/tsm.pdf`)
- In-depth theoretical analysis, mathematical formulation, complexity breakdown, and empirical evaluation of the algorithms.

## Project Structure

```text
.
├── include/
│   ├── bellman.h                           # Header for Bellman-Ford functions
│   └── tsm.h                               # Header for TSP solver functions
├── src/
│   ├── bellman.cpp                         # Implementation of Bellman-Ford
│   ├── tsm.cpp                             # Implementation of Bellman-Held-Karp & heuristics
│   └── main.cpp                            # Test runner & demonstration suite
├── specs/
│   └── DS__CO1007__SEM_242___Assignment.pdf # Assignment specification document
├── report/
│   └── tsm.pdf                             # Technical report on TSP solution
├── .gitignore                              # Git ignore rules
├── LICENSE                                 # MIT License
├── README.md                               # Project documentation
└── run.sh                                  # Build and run script
```

## Getting Started

### Prerequisites

- A C++ compiler supporting **C++11** (e.g., `g++` or `clang++`).

### Build & Run

Compile and execute the test suite using `g++`:

```bash
g++ -o main src/main.cpp -std=c++11
./main
```

Or execute the provided shell script:

```bash
bash run.sh
```

## License

This project is licensed under the [MIT License](LICENSE).
