#include "twosum.h"
#include <unordered_set>
#include <unordered_map>

namespace twosum {

TwoSumResult bruteForceTwoSum(const std::vector<int>& arr, int target) {
    // Check all pairs (i, j) where i < j
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[i] + arr[j] == target) {
                return {true, static_cast<int>(i), static_cast<int>(j), arr[i], arr[j]};
            }
        }
    }
    
    // No solution found
    return {false, -1, -1, 0, 0};
}

TwoSumResult hashTwoSum(const std::vector<int>& arr, int target) {
    // Hash set to store values we've seen
    std::unordered_map<int, size_t> seen;  // value -> index
    
    for (size_t i = 0; i < arr.size(); ++i) {
        int complement = target - arr[i];
        
        // Check if complement exists in hash set
        auto it = seen.find(complement);
        if (it != seen.end()) {
            size_t j = it->second;
            return {true, static_cast<int>(j), static_cast<int>(i), arr[j], arr[i]};
        }
        
        // Add current element to hash set
        seen[arr[i]] = i;
    }
    
    // No solution found
    return {false, -1, -1, 0, 0};
}

} // namespace twosum
