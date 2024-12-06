#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <chrono>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Различные размеры векторов
    std::vector<int> vector_sizes = { 1000, 10000, 100000, 1000000, 10000000 };

    if (rank == 0) {
        // Вывод заголовка таблицы
        std::cout << "Результат ; Размер вектора ; Время\n";
        std::cout << "-------------------------------------------------------------------------------------------\n";
    }

    for (int VECTOR_SIZE : vector_sizes) {
        std::vector<int> data;

        if (rank == 0) {
            // Инициализация случайного вектора на процессе с рангом 0
            data.resize(VECTOR_SIZE);
            std::srand(static_cast<unsigned>(std::time(0)));
            for (int i = 0; i < VECTOR_SIZE; ++i) {
                data[i] = std::rand() % 10000; // Значения от 0 до 9999
            }
        }

        // Вычисление размера блока для каждого процесса
        int block_size = VECTOR_SIZE / size;
        std::vector<int> local_data(block_size);

        // Распределение данных вектора между процессами
        MPI_Scatter(data.data(), block_size, MPI_INT, local_data.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);

        // Локальный минимум и максимум
        int local_min = std::numeric_limits<int>::max();
        int local_max = std::numeric_limits<int>::min();
        for (int value : local_data) {
            if (value < local_min) local_min = value;
            if (value > local_max) local_max = value;
        }

        // Глобальный минимум и максимум
        int global_min, global_max;
        double start_time = MPI_Wtime();
        MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;

        // Результаты на процессе 0
        if (rank == 0) {
            std::cout << global_min << "/" << global_max << ";"
                << VECTOR_SIZE << ";"
                << elapsed_time << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}