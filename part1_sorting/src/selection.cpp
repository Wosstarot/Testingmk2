#include "sorting.h"

namespace sorting {
	SortStats selectionSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.data = arr;
		stats.comparisons = 0;

		if (arr.size() <= 1) return stats;

		for (size_t i = 0; i < stats.data.size() - 1; ++i) {
			size_t min_pos = i;

			for (size_t j = i + 1; j < stats.data.size(); ++j) {
				stats.comparisons++;  // Update comparison count
				if (stats.data[j] < stats.data[min_pos]) {
					min_pos = j;
				}
			}

			std::swap(stats.data[i], stats.data[min_pos]);
		}

		return stats;
	}
}
