#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <chrono>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Массив размеров векторов для тестирования
    std::vector<int> vector_sizes = { 1000, 10000, 100000, 1000000, 10000000 };

    if (rank == 0) {
        // Вывод заголовка таблицы
        std::cout << "Результат ; Размер вектора ; Время\n";
        std::cout << "-------------------------------------------------------------------------------------------\n";
    }

    // Размерность вектора и генерация случайных данных
    for (int VECTOR_SIZE : vector_sizes) {
        std::vector<int> data(VECTOR_SIZE);
        srand(rank + 1);
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            data[i] = rand();
        }

        // Нахождение локального минимума
        int local_min = data[0];
        for (int i = 1; i < VECTOR_SIZE; ++i) {
            if (data[i] < local_min) {
                local_min = data[i];
            }
        }

        // Нахождение глобального минимума
        int global_min;
        auto start = std::chrono::high_resolution_clock::now();
        MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Запись результатов
        if (rank == 0) {
            std::ofstream output_file("task1_output.txt", std::ios::app);
            if (output_file.is_open()) {
                output_file << global_min << ";" << VECTOR_SIZE << ";" << elapsed.count() << "\n";
                output_file.close();
            }
            else {
                std::cerr << "Не удалось открыть файл для записи\n";
            }
        }
    }
    MPI_Finalize();
    return 0;
}