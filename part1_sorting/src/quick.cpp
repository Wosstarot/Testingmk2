#include "sorting.h"
#include <algorithm>

namespace sorting {
	namespace {

		int medianOfThree(std::vector<int>& arr, int low, int high, size_t& comparisons) {
			int mid = (low + high) / 2;
			
			comparisons++; // Update comparison count
			if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);

			comparisons++; // Update comparison count
			if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);

			comparisons++; // Update comparison count
			if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);

			std::swap(arr[mid], arr[high]);
			return arr[high];
		}


		int partition(std::vector<int>& arr, int low, int high, size_t& comparisons) {
			int pivot = medianOfThree(arr, low, high, comparisons);

			int i = low - 1;

			for (int j = low; j < high; ++j) {
			comparisons++; // Update comparison count
			if (arr[j] <= pivot) {
					++i;
					std::swap(arr[i], arr[j]);
				}
			}

			std::swap(arr[i + 1], arr[high]);
			return i + 1;
		}


		// Helper function so quickSort's public API creates a copy and doesn't sort in place
		void quickSortImpl(std::vector<int>& arr, int low, int high, size_t& comparisons) {
			if (low < high) {
				int pivotIndex = partition(arr, low, high, comparisons);
				quickSortImpl(arr, low, pivotIndex - 1, comparisons);
				quickSortImpl(arr, pivotIndex + 1, high, comparisons);
			}
		}

	} // Anonymous namespace


	SortStats quickSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.data = arr;
		stats.comparisons = 0;
		
		if (arr.size() <=1) return stats;

		quickSortImpl(stats.data, 0, (int)stats.data.size() - 1, stats.comparisons);
		return stats;

	}

} // namespace sorting
