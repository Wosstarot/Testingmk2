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

	} // Anonymous namespace

	SortStats countingSort(const std::vector<int>& arr) {
		SortStats stats;
		stats.comparisons = 0;  // Non-comparison sort

		if (arr.size() <= 1) {
			stats.data = arr;
			return stats;
		}

		int mx = getMax(arr);
		int n = arr.size();
		std::vector<int> freq(mx + 1, 0);
		stats.data.resize(n);

		for (int i = 0; i < n; ++i) {
			++freq[arr[i]];
		}

		for (int i = 1; i <= mx; ++i) {
			freq[i] += freq[i - 1];
		}

		for (int i = n - 1; i >= 0; --i) {
			int x = arr[i];
			stats.data[freq[x] - 1] = x;
			--freq[x];
		}

		return stats;
	}
} // namespace sorting
