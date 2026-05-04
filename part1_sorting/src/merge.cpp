#include "sorting.h"

namespace sorting {
	namespace {
		void merge(std::vector<int>& arr, std::vector<int>& aux,
		           size_t lo, size_t mid, size_t hi, size_t& comparisons) {
			// Copy to auxiliary buffer
			for (size_t k = lo; k <= hi; ++k)
				aux[k] = arr[k];

			size_t i = lo, j = mid + 1;
			for (size_t k = lo; k <= hi; ++k) {
				if (i > mid) {
					arr[k] = aux[j++];
				} else if (j > hi) {
					arr[k] = aux[i++];
				} else {
					comparisons++;
					if (aux[i] <= aux[j])
						arr[k] = aux[i++];
					else
						arr[k] = aux[j++];
				}
			}
		}

		void sort(std::vector<int>& arr, std::vector<int>& aux,
		          size_t lo, size_t hi, size_t& comparisons) {
			if (lo >= hi) return;
			size_t mid = lo + (hi - lo) / 2;
			sort(arr, aux, lo,      mid, comparisons);
			sort(arr, aux, mid + 1, hi,  comparisons);
			merge(arr, aux, lo, mid, hi, comparisons);
		}
	}

	SortStats mergeSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.comparisons = 0;
		stats.data = arr;

		if (stats.data.size() <= 1) return stats;

		std::vector<int> aux(stats.data.size());
		sort(stats.data, aux, 0, stats.data.size() - 1, stats.comparisons);

		return stats;
	}
} // namespace sorting
