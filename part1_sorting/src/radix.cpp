#include "sorting.h"

namespace sorting {
	namespace {
		int getMax(const std::vector<int>& arr) {
			int mx = arr[0];
			for (size_t i = 1; i < arr.size(); ++i) {
				if (arr[i] > mx) {
					mx = arr[i];
				}
			}

			return mx;
		}

		std::vector<int> stableSort(const std::vector<int>& arr, int exp) {
			int n = arr.size();
			std::vector<int> freq(10, 0);
			std::vector<int> result(n);

			for (int i = 0; i < n; ++i) {
				int digit = (arr[i] / exp) % 10;
				freq[digit]++;
			}

			for (int i = 1; i < 10; ++i) freq[i] += freq[i-1];

			for (int i = n - 1; i >= 0; --i) {
				int digit = (arr[i] / exp) % 10;
				result[freq[digit] - 1] = arr[i];
				--freq[digit];
			}

			return result;
		}

	} // Anonymous namespace

	SortStats radixSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.comparisons =0; // Non-comparison sort

		if (arr.size() <= 1) {
			stats.data = arr;
			return stats;
		}

		int mx = getMax(arr);
		stats.data = arr;

		for (int exp = 1; (mx / exp) > 0; exp *= 10) {
			stats.data = stableSort(stats.data, exp);
		}

		return stats;
	}

} // namespace sorting
