# Bellman-Ford-parallel :rocket:
Implementation of Bellman-Ford algorithm - parallel version for the course in:
Architectures and Platform for Artificial Intelligence (2024) @ _University of Bologna_. :mortar_board:

The algorithm are tested on four different graphs automatically generated. 

The project is built for _Slurm architectures_, to run it simply run:
```
sabtch project.sbatch
```
## Running info
The sbatch script will automatically run all the experiments with _graph_XS_. <br>
To test other graphs changhe the file _project.sbatch_ accordingly with the desired graph name: <br>
1. "graph_XS.txt" nodes: 250, edges: 1500
2. "graph_S.txt" nodes: 1500, edges: 10k
3. "graph_M.txt" nodes: 5k, edges: 200k
4. "graph_L.txt" nodes: 10k, edges: 500k

If no input name is defined, all four input graph will be tested (it will take a while).

**Extract** the _benchmark.zip_ folder. <br>
Project file structure: <br>
```
project
│   README.md
│   makefile
|   project.sbatch
|   project.pdf 
|
└───benchmark
│
└───openmp_base
│   └───libs
│   └───src
|
└───openmp_frontier
│   └───libs
│   └───src
|
└───cuda
│   └───src
```

The complete report of the experiment can be found [here!](https://github.com/alessiocnt/Bellman-Ford-parallel/blob/main/project.pdf)
