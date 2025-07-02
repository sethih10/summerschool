#!/bin/bash
# #SBATCH --ntasks=4 # For proesses MPI
# #SBATCH --cpus-per-task=1  # For threads OpenMP
# 
# export OMP_NUM_THREADS=10
# 
# 
# # This would led the threads to be serialized because one cpu might run max 2 threads at a time so it would be actually serialized. 



#SBATCH --job-name=example
#SBATCH --account=project_462000956
#SBATCH --partition=small
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --output=slurm%x_%j.out

# Set the number of threads based on --cpus-per-task
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
srun ./skeleton