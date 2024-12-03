#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <numeric>
#include <cmath>

// Порог для сравнения чисел с плавающей точкой
constexpr double EPSILON = 1e-6;

// Функция проверки корректности
bool verify_results(const std::vector<int>& result1, const std::vector<int>& result2, int N) {
    for (int i = 0; i < N * N; ++i) {
        if (std::abs(result1[i] - result2[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

// Параллельный ленточный алгоритм умножения матриц
void matrix_multiply_parallel(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, int N, int rank, int size) {
    int block_size = N / size;
    std::vector<int> local_A(block_size * N);
    std::vector<int> local_C(block_size * N, 0);

    MPI_Scatter(A.data(), block_size * N, MPI_INT, local_A.data(), block_size * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<int*>(B.data()), N * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < block_size; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    MPI_Gather(local_C.data(), block_size * N, MPI_INT, C.data(), block_size * N, MPI_INT, 0, MPI_COMM_WORLD);
}


// Последовательное умножение матриц (для сравнения)
void matrix_multiply_simple(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i * N + j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> matrix_sizes = {192, 384, 768};

    if (rank == 0) {
        std::cout << "Размер матрицы ; Кол-во процессов ; Ленточный (сек)  ; Последовательный (сек) ; Ленточный (корректность) \n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";
    }

    for (int N : matrix_sizes) {
        std::vector<int> A(N * N), B(N * N), C_seq(N * N, 0), C_parallel(N * N, 0);

        if (rank == 0) {
            std::srand(static_cast<unsigned>(std::time(0)));
            for (int i = 0; i < N * N; ++i) {
                A[i] = std::rand() % 10;
                B[i] = std::rand() % 10;
            }
        }

        double seq_start_time = MPI_Wtime();
        matrix_multiply_simple(A, B, C_seq, N);
        double seq_end_time = MPI_Wtime();
        double seq_time = seq_end_time - seq_start_time;

        double start_time = MPI_Wtime();
        matrix_multiply_parallel(A, B, C_parallel, N, rank, size);
        double end_time = MPI_Wtime();
        double parallel_time = end_time - start_time;


        bool parallel_correct = false;

        if (rank == 0) {
            parallel_correct = verify_results(C_seq, C_parallel, N);

            std::cout << N << ";" << size << ";"
                      << parallel_time  << ";"
                      << seq_time << ";" << (parallel_correct ? "Да" : "Нет") << ";"
                     << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
