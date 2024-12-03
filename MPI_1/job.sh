#!/bin/sh
#SBATCH --job-name=task1_job
#SBATCH --ntasks=16
#SBATCH --nodes=2
#SBATCH --time=00:05:00
#SBATCH --partition=standard

module load openmpi
mpirun ./task1 >> task1_output.txt
rm slurm*