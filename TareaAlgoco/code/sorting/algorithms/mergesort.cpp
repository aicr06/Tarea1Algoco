// Implementación de MergeSort basada en el pseudocódigo visto en clases.
// diapositivas de INF221, Dividir y Conquistar (Parte 1).

#include <vector>

void Merge(int *T, int *A, int left, int mid, int right) {
    int i1 = left; 
    int i2 = mid;
    int i = left;
    while (i1 < mid && i2 <= right) {
        if (T[i1] <= T[i2])
            A[i++] = T[i1++];
        else
            A[i++] = T[i2++];
    }
    while (i1 < mid)
        A[i++] = T[i1++];
    while (i2 <= right)
        A[i++] = T[i2++];
}

void MergeSort(int *A, int *T, int left, int right) {
    int n = right - left + 1;
    if (n <= 1)
        return;
    int mid = left + n / 2;
    MergeSort(A, T, left, mid - 1);
    MergeSort(A, T, mid, right);
    for (int i = left; i <= right; i++)
        T[i] = A[i];
    Merge(T, A, left, mid, right);
}

void mergeSort(std::vector<int>& A) {
    if (A.size() <= 1) return;
    std::vector<int> T(A.size());
    MergeSort(A.data(), T.data(), 0, static_cast<int>(A.size()) - 1);
}