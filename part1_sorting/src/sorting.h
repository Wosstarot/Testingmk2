#ifndef SORTING_H
#define SORTING_H
#include <vector>

namespace sorting {
    struct SortStats {
        std::vector<int> data;
        size_t comparisons = 0;  // 0 for non-comparison sorts
    };

    SortStats insertionSort(const std::vector<int>& arr);
    SortStats selectionSort(const std::vector<int>& arr);
    SortStats bubbleSort(const std::vector<int>& arr);
    SortStats mergeSort(const std::vector<int>& arr);
    SortStats quickSort(const std::vector<int>& arr);
    SortStats heapSort(const std::vector<int>& arr);
    SortStats countingSort(const std::vector<int>& arr);
    SortStats radixSort(const std::vector<int>& arr);
}

#endif
