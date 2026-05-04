#ifndef TWOSUM_H
#define TWOSUM_H

#include <vector>
#include <optional>

namespace twosum {

struct TwoSumResult {
    bool found;
    int index1;  // First index (or -1 if not found)
    int index2;  // Second index (or -1 if not found)
    int value1;  // First value (or 0 if not found)
    int value2;  // Second value (or 0 if not found)
};

// Brute force: O(n²) time, O(1) space
TwoSumResult bruteForceTwoSum(const std::vector<int>& arr, int target);

// Hash-based: O(n) average time, O(n) space
TwoSumResult hashTwoSum(const std::vector<int>& arr, int target);

} // namespace twosum

#endif
