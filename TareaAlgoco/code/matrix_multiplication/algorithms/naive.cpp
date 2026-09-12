// Implementación de multiplicación de matrices (naive/clásica), O(n^3).
// Basada en el pseudocódigo de matrixMult visto en clases de INF221
// (diapositivas del curso, 2026-2), adaptada de int**/new a
// std::vector<std::vector<int>> para manejo seguro de memoria (RAII).

#include <vector>

using Matrix = std::vector<std::vector<int>>;

// Multiplicación clásica de matrices (Naive): O(n^3) en tiempo, O(n^2) en espacio
Matrix matrixMultNaive(const Matrix& M1, const Matrix& M2, int n) {
    Matrix M(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                M[i][j] += M1[i][k] * M2[k][j];
            }
        }
    }
    return M;
}