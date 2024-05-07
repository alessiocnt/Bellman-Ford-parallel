#!/bin/bash
#SBATCH --job-name=nomejob
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alessio.conti3@unibo.it
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --output=projectoutput
#SBATCH --gres=gpu:1

make clean
make

nvcc cuda/src/main.cu -o cuda.exe

# Check if the compilation was successful
if [ $? -eq 0 ]; then
	# Run the compiled CUDA program 
	./omp_base.exe graph_XS.txt
	./omp_frontier.exe graph_XS.txt
	./cuda.exe graph_XS.txt
else
	echo "Compilation failed. Exiting."
fi

