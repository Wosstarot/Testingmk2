#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "sorting.h"
#include "generator.h"

using namespace std;
using namespace sorting;

int testAlgorithm(
    const string& name,
    function<SortStats(const vector<int>&)> sort_fn
) {
    cout << "Testing " << name << "..." << endl;
    int failed = 0;
    
    // Test 1: Empty array
    {
        vector<int> arr = {};
        auto result = sort_fn(arr);
        if (!result.data.empty()) {
            cout << "  FAILED: Empty array - output not empty" << endl;
            failed++;
        }
    }
    
    // Test 2: Single element
    {
        vector<int> arr = {42};
        auto result = sort_fn(arr);
        if (result.data.size() != 1 || result.data[0] != 42) {
            cout << "  FAILED: Single element - got [" << result.data[0] << "], expected [42]" << endl;
            failed++;
        }
    }
    
    // Test 3: Two elements (sorted)
    {
        vector<int> arr = {1, 2};
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Two elements sorted - output not sorted" << endl;
            failed++;
        }
    }
    
    // Test 4: Two elements (unsorted)
    {
        vector<int> arr = {2, 1};
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Two elements unsorted - output {" << result.data[0] << ", " 
                 << result.data[1] << "} not sorted" << endl;
            failed++;
        }
        if (result.data[0] != 1 || result.data[1] != 2) {
            cout << "  FAILED: Two elements unsorted - wrong values" << endl;
            failed++;
        }
    }
    
    // Test 5: Already sorted
    {
        vector<int> arr = generateSorted(100);
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Sorted array (n=100) - output not sorted" << endl;
            failed++;
        }
    }
    
    // Test 6: Reverse sorted
    {
        vector<int> arr = generateReversed(100);
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Reversed array (n=100) - output not sorted" << endl;
            failed++;
        }
        if (result.data[0] != 0 || result.data[99] != 99) {
            cout << "  FAILED: Reversed array - wrong boundary values" << endl;
            failed++;
        }
    }
    
    // Test 7: Random array + element preservation
    {
        vector<int> arr = generateRandom(100);
        vector<int> original = arr;
        auto result = sort_fn(arr);
        
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Random array (n=100) - output not sorted" << endl;
            failed++;
        }
        
        sort(original.begin(), original.end());
        if (result.data != original) {
            cout << "  FAILED: Random array - elements not preserved" << endl;
            failed++;
        }
    }
    
    // Test 8: All duplicates
    {
        vector<int> arr(50, 7);
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: All duplicates (n=50) - output not sorted" << endl;
            failed++;
        }
        bool all_sevens = true;
        for (int x : result.data) {
            if (x != 7) {
                all_sevens = false;
                break;
            }
        }
        if (!all_sevens) {
            cout << "  FAILED: All duplicates - values changed" << endl;
            failed++;
        }
    }
    
    // Test 9: Some duplicates
    {
        vector<int> arr = generateFewUnique(100);
        vector<int> original = arr;
        auto result = sort_fn(arr);
        
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Few unique (n=100) - output not sorted" << endl;
            failed++;
        }
        
        sort(original.begin(), original.end());
        if (result.data != original) {
            cout << "  FAILED: Few unique - elements not preserved" << endl;
            failed++;
        }
    }
    
    // Test 10: Larger random test
    {
        vector<int> arr = generateRandom(1000);
        vector<int> original = arr;
        auto result = sort_fn(arr);
        
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Large random (n=1000) - output not sorted" << endl;
            failed++;
        }
        
        sort(original.begin(), original.end());
        if (result.data != original) {
            cout << "  FAILED: Large random - elements not preserved" << endl;
            failed++;
        }
    }
    
    // Test 11: Contains zero
    {
        vector<int> arr = {5, 0, 3, 0, 1};
        auto result = sort_fn(arr);
        if (!is_sorted(result.data.begin(), result.data.end())) {
            cout << "  FAILED: Contains zeros - output not sorted" << endl;
            failed++;
        }
        if (result.data[0] != 0 || result.data[1] != 0) {
            cout << "  FAILED: Contains zeros - zeros not at start" << endl;
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
    cout << "Running correctness tests...\n" << endl;
    
    int total_failures = 0;
    
    total_failures += testAlgorithm("Insertion Sort", insertionSort);
    total_failures += testAlgorithm("Selection Sort", selectionSort);
    total_failures += testAlgorithm("Bubble Sort", bubbleSort);
    total_failures += testAlgorithm("Merge Sort", mergeSort);
    total_failures += testAlgorithm("Quicksort", quickSort);
    total_failures += testAlgorithm("Heapsort", heapSort);
    total_failures += testAlgorithm("Counting Sort", countingSort);
    total_failures += testAlgorithm("Radix Sort", radixSort);
    
    cout << "\n========================================" << endl;
    if (total_failures == 0) {
        cout << "✓ ALL TESTS PASSED" << endl;
    } else {
        cout << "✗ TOTAL FAILURES: " << total_failures << endl;
    }
    cout << "========================================" << endl;
    
    return (total_failures == 0) ? 0 : 1;
}
