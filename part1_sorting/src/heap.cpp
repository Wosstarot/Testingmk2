#include "sorting.h"
#include <algorithm>

namespace sorting {
    namespace {
        void heapify(std::vector<int>& a, size_t n, size_t i, size_t& comparisons) {
            while (true) {
                size_t largest = i;
                size_t left = 2 * i + 1;
                size_t right = 2 * i + 2;
                
                if (left < n) {
                    comparisons++;  // Update comparison count
                    if (a[left] > a[largest]) {
                        largest = left;
                    }
                }
                
                if (right < n) {
                    comparisons++;  // Update comparison count
                    if (a[right] > a[largest]) {
                        largest = right;
                    }
                }
                
                if (largest == i) {
                    break;
                }
                
                std::swap(a[i], a[largest]);
                i = largest;
            }
        }
    } // anonymous namespace

    SortStats heapSort(const std::vector<int>& arr) {
        SortStats stats;
        stats.data = arr;
        stats.comparisons = 0;
        
        if (arr.size() <= 1) return stats;
        
        const size_t n = stats.data.size();
        
        // Build heap
        for (size_t i = n / 2; i-- > 0; ) {
            heapify(stats.data, n, i, stats.comparisons);
        }
        
        // Extract elements from heap
        for (size_t end = n; end > 1; --end) {
            std::swap(stats.data[0], stats.data[end - 1]);
            heapify(stats.data, end - 1, 0, stats.comparisons);
        }
        
        return stats;
    }
} // namespace sorting
