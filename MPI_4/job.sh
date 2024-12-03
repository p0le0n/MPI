#!/bin/bash
#SBATCH --job-name=matrix_multiplication
#SBATCH --output=matrix_output.txt
#SBATCH --ntasks=16
#SBATCH --nodes=4
#SBATCH --time=00:30:00
#SBATCH --partition=standard

module load openmpi

# Генерация матриц A и B
echo "Генерация матриц..."
./task4_canonmatrixgen

# Выполнение ленточного алгоритма умножения
echo "Запуск ленточного алгоритма умножения матриц..."
mpirun ./task4

# Выполнение алгоритма Кэннона
echo "Запуск алгоритма Кэннона для умножения матриц..."
mpirun ./task4_canon
