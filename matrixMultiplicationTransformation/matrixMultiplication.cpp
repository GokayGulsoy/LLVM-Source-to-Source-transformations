#include <iostream>
#include <vector>
#include <exception>

// Matrix multiplication function
std::vector<std::vector<double>> matrixMultiplicationFunction(const std::vector<std::vector<double>> &matrix1,
                                                              const std::vector<std::vector<double>> &matrix2) {
    int rows1 = 2;
    int cols1 = 2;
    int rows2 = 2;
    int cols2 = 2;

    std::vector<std::vector<double>> result(rows1, std::vector<double>(cols2, 0.0));

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

int main() {
    // Example matrices
    std::vector<std::vector<double>> matrix1 = {{1.0, 2.0}, {3.0, 4.0}};
    std::vector<std::vector<double>> matrix2 = {{5.0, 6.0}, {7.0, 8.0}};

    // Perform matrix multiplication
    std::vector<std::vector<double>> result = matrixMultiplicationFunction(matrix1, matrix2);

    // Display the result
    for (const auto &row : result) {
        for (double value : row) {
            std::cout << "" << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
