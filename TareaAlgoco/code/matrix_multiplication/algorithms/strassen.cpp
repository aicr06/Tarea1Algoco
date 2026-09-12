// Implementación del algoritmo de Strassen para multiplicación de matrices.
// Basada en:
// - Wikipedia: "Strassen algorithm", https://en.wikipedia.org/wiki/Strassen_algorithm
//   (fórmulas de los 7 productos P1..P7 y combinación en los 4 cuadrantes).
// - GeeksforGeeks: "Strassen's Matrix Multiplication Algorithm | Implementation",
//   https://www.geeksforgeeks.org/dsa/strassens-matrix-multiplication-algorithm-implementation/
//   (estructura recursiva general: caso base, división y combinación).
// Asume n = potencia de 2 (según los tamaños definidos en el enunciado de
// la tarea: 2^4, 2^6, 2^8, 2^10); no maneja padding para n arbitrario.

#include <vector>

using Matrix = std::vector<std::vector<int>>;

Matrix addMatrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

Matrix subMatrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

Matrix matrixMultStrassen(const Matrix& M1, const Matrix& M2, int n) {
    if (n == 1) {
        return {{M1[0][0] * M2[0][0]}};
    }

    int k = n / 2;

    Matrix A(k, std::vector<int>(k)), B(k, std::vector<int>(k)),
           C(k, std::vector<int>(k)), D(k, std::vector<int>(k)),
           E(k, std::vector<int>(k)), F(k, std::vector<int>(k)),
           G(k, std::vector<int>(k)), H(k, std::vector<int>(k));

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            A[i][j] = M1[i][j];
            B[i][j] = M1[i][j + k];
            C[i][j] = M1[i + k][j];
            D[i][j] = M1[i + k][j + k];

            E[i][j] = M2[i][j];
            F[i][j] = M2[i][j + k];
            G[i][j] = M2[i + k][j];
            H[i][j] = M2[i + k][j + k];
        }
    }

    Matrix P1 = matrixMultStrassen(A, subMatrix(F, H), k);
    Matrix P2 = matrixMultStrassen(addMatrix(A, B), H, k);
    Matrix P3 = matrixMultStrassen(addMatrix(C, D), E, k);
    Matrix P4 = matrixMultStrassen(D, subMatrix(G, E), k);
    Matrix P5 = matrixMultStrassen(addMatrix(A, D), addMatrix(E, H), k);
    Matrix P6 = matrixMultStrassen(subMatrix(B, D), addMatrix(G, H), k);
    Matrix P7 = matrixMultStrassen(subMatrix(A, C), addMatrix(E, F), k);

    Matrix C11 = addMatrix(subMatrix(addMatrix(P5, P4), P2), P6);
    Matrix C12 = addMatrix(P1, P2);
    Matrix C21 = addMatrix(P3, P4);
    Matrix C22 = subMatrix(subMatrix(addMatrix(P1, P5), P3), P7);

    Matrix M(n, std::vector<int>(n));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            M[i][j]         = C11[i][j];
            M[i][j + k]     = C12[i][j];
            M[i + k][j]     = C21[i][j];
            M[i + k][j + k] = C22[i][j];
        }
    }

    return M;
}