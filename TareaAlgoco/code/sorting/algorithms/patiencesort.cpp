// Implementación de Patience Sort (variante eficiente, O(n log n)).
// Basada en:
// - Wikipedia: "Patience sorting", https://en.wikipedia.org/wiki/Patience_sorting
//   (reglas del juego / fase de distribución en pilas).
// - Wikibooks: "Algorithm Implementation/Sorting/Patience sort",
//   https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Patience_sort
//   (idea de usar búsqueda binaria para ubicar la pila destino en O(log p)).
// Fase de mezcla implementada como k-way merge con min-heap (std::priority_queue)
// sobre los topes de las pilas, para lograr O(n log n) total en vez del
// O(n^2) de un merge lineal ingenuo.

#include <vector>
#include <queue>
#include <algorithm>

struct HeapNode {
    int value;
    int pileIndex;

    bool operator>(const HeapNode& other) const {
        return value > other.value;
    }
};

void patienceSort(std::vector<int>& A) {
    if (A.size() <= 1) return;

    std::vector<std::vector<int>> piles;
    std::vector<int> topElements;

    for (int x : A) {
        auto it = std::lower_bound(topElements.begin(), topElements.end(), x);
        int idx = std::distance(topElements.begin(), it);

        if (it == topElements.end()) {
            piles.push_back({x});
            topElements.push_back(x);
        } else {
            piles[idx].push_back(x);
            topElements[idx] = x;
        }
    }

    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> minHeap;

    for (size_t i = 0; i < piles.size(); ++i) {
        if (!piles[i].empty()) {
            minHeap.push({piles[i].back(), static_cast<int>(i)});
            piles[i].pop_back();
        }
    }

    size_t writeIdx = 0;
    while (!minHeap.empty()) {
        HeapNode topNode = minHeap.top();
        minHeap.pop();

        A[writeIdx++] = topNode.value;

        int pIdx = topNode.pileIndex;
        if (!piles[pIdx].empty()) {
            minHeap.push({piles[pIdx].back(), pIdx});
            piles[pIdx].pop_back();
        }
    }
}