// Implementación de QuickSort con esquema de partición de Hoare
// (pivote = elemento medio), variante equivalente al pseudocódigo
// visto en clases de INF221 (diapositivas del curso, 2026-2).

#include <vector>
#include <utility>

int partition(int A[], int i, int d, int p) {
    int pivotValue = A[p];
    i--;
    d++;
    do {
        do {
            i++;
        } while (A[i] < pivotValue);

        do {
            d--;
        } while (A[d] > pivotValue);

        if (i < d) {
            std::swap(A[i], A[d]);
        }
    } while (i < d);

    return d;
}

void QuickSort(int A[], int i, int d) {
    if (i >= d) return;

    int mid = i + (d - i) / 2;
    int pivotValue = A[mid];

    int left = i;
    int right = d;

    while (left <= right) {
        while (A[left] < pivotValue) left++;
        while (A[right] > pivotValue) right--;

        if (left <= right) {
            std::swap(A[left], A[right]);
            left++;
            right--;
        }
    }

    if (i < right) {
        QuickSort(A, i, right);
    }
    if (left < d) {
        QuickSort(A, left, d);
    }
}

void quickSort(std::vector<int>& A) {
    if (A.size() <= 1) return;
    QuickSort(A.data(), 0, static_cast<int>(A.size()) - 1);
}