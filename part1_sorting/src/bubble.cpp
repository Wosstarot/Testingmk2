#include "sorting.h"
#include <algorithm>

namespace sorting {
	SortStats bubbleSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.data = arr;
		stats.comparisons = 0;

		if (arr.size() <= 1) return stats;

		for (size_t i = 0; i < stats.data.size(); ++i) {
			for (size_t j = 0; j < stats.data.size() - 1 - i; ++j) {
				stats.comparisons++;  // Update comparison count
				if (stats.data[j] > stats.data[j + 1]) {
					std::swap(stats.data[j], stats.data[j + 1]);
				}
			}
		}

		return stats;
	}
} // namespace sorting
