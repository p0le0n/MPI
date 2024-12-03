#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void generateMatrix(int** matrix, int rows, int cols) {
    // Генерация случайных чисел для матрицы
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 11;  // Случайное число от 0 до 10
        }
    }
}

void printMatrixToFile(int** matrix, int rows, int cols, const std::string& filename) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
        return;
    }

    // Запись матрицы в файл
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file << matrix[i][j];
            if (j < cols - 1) {
                file << " "; // Пробел между числами, но не в конце строки
            }
        }
        file << "\n";
    }

    file.close();
}

int main() {
    int rows = 768;  // Количество строк
    int cols = 768;  // Количество столбцов

    // Инициализация случайного генератора
    srand(time(0));

    // Выделение памяти для матриц A и B
    int** A = new int*[rows];
    int** B = new int*[rows];
    for (int i = 0; i < rows; i++) {
        A[i] = new int[cols];
        B[i] = new int[cols];
    }

    // Генерация случайных матриц A и B
    generateMatrix(A, rows, cols);
    generateMatrix(B, rows, cols);

    // Запись матриц в файлы
    printMatrixToFile(A, rows, cols, "A.txt");
    printMatrixToFile(B, rows, cols, "B.txt");

    // Освобождение памяти
    for (int i = 0; i < rows; i++) {
        delete[] A[i];
        delete[] B[i];
    }
    delete[] A;
    delete[] B;

    std::cout << "Матрицы A и B успешно сгенерированы и записаны в файлы A.txt и B.txt!" << std::endl;

    return 0;
}
