#!/bin/sh
#SBATCH --job-name=task3_job
#SBATCH --ntasks=16
#SBATCH --nodes=2
#SBATCH --time=00:05:00
#SBATCH --partition=standard

module load openmpi
mpirun ./task3 >> task3_output.txt
rm slurm*