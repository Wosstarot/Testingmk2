#include "sorting.h"

namespace sorting {
	SortStats insertionSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.data = arr;
		stats.comparisons = 0;

		if (arr.size() <= 1) return stats;

		for (size_t i = 1; i < stats.data.size(); ++i) {
			int key = stats.data[i];
			int j = i - 1;

			// Count every comparison in the while condition
			while (j >= 0) {
				stats.comparisons++;  // Update comparison count
				if (stats.data[j] > key) {
					stats.data[j + 1] = stats.data[j];
					--j;
				} else {
					break;
				}
			}

			stats.data[j + 1] = key;
		}
		return stats;
	}

} // namespace sorting
