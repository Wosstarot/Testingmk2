#include <iostream>
#include <vector>
#include <functional>
#include "../src/twosum.h"

using namespace std;
using namespace twosum;

int testAlgorithm(
    const string& name,
    function<TwoSumResult(const vector<int>&, int)> twosum_fn
) {
    cout << "Testing " << name << "..." << endl;
    int failed = 0;
    
    // Test 1: Empty array
    {
        vector<int> arr = {};
        auto result = twosum_fn(arr, 10);
        if (result.found) {
            cout << "  FAILED: Empty array - should not find solution" << endl;
            failed++;
        }
    }
    
    // Test 2: Single element
    {
        vector<int> arr = {5};
        auto result = twosum_fn(arr, 10);
        if (result.found) {
            cout << "  FAILED: Single element - should not find solution" << endl;
            failed++;
        }
    }
    
    // Test 3: Two elements - no solution
    {
        vector<int> arr = {3, 5};
        auto result = twosum_fn(arr, 10);
        if (result.found) {
            cout << "  FAILED: Two elements no solution - found=" << result.found << endl;
            failed++;
        }
    }
    
    // Test 4: Two elements - solution exists
    {
        vector<int> arr = {3, 7};
        auto result = twosum_fn(arr, 10);
        if (!result.found) {
            cout << "  FAILED: Two elements with solution - not found" << endl;
            failed++;
        }
        if (result.found && (result.value1 + result.value2 != 10)) {
            cout << "  FAILED: Two elements - wrong sum" << endl;
            failed++;
        }
    }
    
    // Test 5: Multiple elements - solution exists
    {
        vector<int> arr = {1, 5, 3, 8, 2};
        auto result = twosum_fn(arr, 11);
        if (!result.found) {
            cout << "  FAILED: Multiple elements - solution exists but not found" << endl;
            failed++;
        }
        if (result.found && (result.value1 + result.value2 != 11)) {
            cout << "  FAILED: Multiple elements - wrong sum: " 
                 << result.value1 << " + " << result.value2 << endl;
            failed++;
        }
    }
    
    // Test 6: No solution
    {
        vector<int> arr = {1, 2, 3, 4};
        auto result = twosum_fn(arr, 100);
        if (result.found) {
            cout << "  FAILED: No solution case - found false positive" << endl;
            failed++;
        }
    }
    
    // Test 7: Solution at beginning
    {
        vector<int> arr = {5, 10, 20, 30};
        auto result = twosum_fn(arr, 15);  // 5 + 10
        if (!result.found) {
            cout << "  FAILED: Solution at beginning - not found" << endl;
            failed++;
        }
    }
    
    // Test 8: Solution at end
    {
        vector<int> arr = {1, 2, 3, 7, 8};
        auto result = twosum_fn(arr, 15);  // 7 + 8
        if (!result.found) {
            cout << "  FAILED: Solution at end - not found" << endl;
            failed++;
        }
    }
    
    // Test 9: Duplicates - same value appears twice
    {
        vector<int> arr = {3, 3};
        auto result = twosum_fn(arr, 6);
        if (!result.found) {
            cout << "  FAILED: Duplicates - should find 3 + 3 = 6" << endl;
            failed++;
        }
    }
    
    // Test 10: Large array
    {
        vector<int> arr(1000);
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = i;
        }
        auto result = twosum_fn(arr, 1997);  // 998 + 999
        if (!result.found) {
            cout << "  FAILED: Large array - solution not found" << endl;
            failed++;
        }
    }
    
    if (failed == 0) {
        cout << "  ✓ All tests PASSED" << endl;
    } else {
        cout << "  ✗ " << failed << " tests FAILED" << endl;
    }
    
    return failed;
}

int main() {
    cout << "Running Two-Sum correctness tests...\n" << endl;
    
    int total_failures = 0;
    
    total_failures += testAlgorithm("Brute Force", bruteForceTwoSum);
    total_failures += testAlgorithm("Hash Table", hashTwoSum);
    
    cout << "\n========================================" << endl;
    if (total_failures == 0) {
        cout << "✓ ALL TESTS PASSED" << endl;
    } else {
        cout << "✗ TOTAL FAILURES: " << total_failures << endl;
    }
    cout << "========================================" << endl;
    
    return (total_failures == 0) ? 0 : 1;
}
