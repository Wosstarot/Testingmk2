#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include "../src/sorting.h"
#include "generator.h"

using namespace std;
using namespace sorting;

struct BenchmarkResult {
    double time_median;
    size_t comparisons_median;
};

BenchmarkResult runTrials(
    function<SortStats(const vector<int>&)> sort_fn,
    const vector<int>& input,
    size_t trials
) {
    vector<double> times;
    vector<size_t> comparisons;
    
    // Warm-up: Run once to get cache/branch predictor warm
    sort_fn(input);
    
    // Actual trials
    for (size_t i = 0; i < trials; ++i) {
        auto start = chrono::high_resolution_clock::now();
        SortStats stats = sort_fn(input);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
        comparisons.push_back(stats.comparisons);
    }

    // Compute Median
    sort(times.begin(), times.end());
    sort(comparisons.begin(), comparisons.end());
    
    BenchmarkResult result;
    result.time_median = times[trials / 2];
    result.comparisons_median = comparisons[trials / 2];
    
    return result;
}

void benchmarkAlgorithm(
    const string& name,
    function<SortStats(const vector<int>&)> sort_fn,
    const vector<size_t>& sizes,
    size_t trials,
    ofstream& csv
) {
    cout << "\nBenchmarking " << name << "..." << endl;
    
    for (size_t n : sizes) {
        cout << "  n = " << n << "..." << flush;
        
        // Test cases
        struct TestCase {
            string name;
            vector<int> data;
        };
        
        vector<TestCase> cases = {
            {"sorted", generateSorted(n)},
            {"reversed", generateReversed(n)},
            {"random", generateRandom(n)}
        };
        
        for (const auto& test : cases) {
            auto result = runTrials(sort_fn, test.data, trials);
            
            csv << name << "," 
                << n << "," 
                << test.name << "," 
                << result.time_median << "," 
                << result.comparisons_median << "\n";
            
            cout << "." << flush;
        }
        
        cout << " done" << endl;
    }
}

int main() {
    cout << "==================================" << endl;
    cout << "Sorting Algorithm Benchmark" << endl;
    cout << "==================================" << endl;
    
    const size_t TRIALS = 10;  // Back to reasonable number
    
    // Logarithmic spacing for smooth curves
    vector<size_t> sizes_n2 = {1000, 2000, 4000, 8000, 16000, 32000, 50000};
    vector<size_t> sizes_fast = {1000, 2000, 4000, 8000, 16000, 32000, 50000};
    
    cout << "Trials per test: " << TRIALS << endl;
    cout << "Input sizes: ";
    for (size_t s : sizes_n2) cout << s << " ";
    cout << "\n" << endl;
    
    cout << "Note: Close other applications for best results" << endl;
    cout << "==================================" << endl;
    
    ofstream csv("benchmark_results.csv");
    csv << "algorithm,n,case,time_seconds,comparisons\n";
    
    // O(n^2) algorithms
    benchmarkAlgorithm("insertion", insertionSort, sizes_n2, TRIALS, csv);
    benchmarkAlgorithm("selection", selectionSort, sizes_n2, TRIALS, csv);
    benchmarkAlgorithm("bubble", bubbleSort, sizes_n2, TRIALS, csv);
    
    // O(n log n) algorithms
    benchmarkAlgorithm("merge", mergeSort, sizes_fast, TRIALS, csv);
    benchmarkAlgorithm("quick", quickSort, sizes_fast, TRIALS, csv);
    benchmarkAlgorithm("heap", heapSort, sizes_fast, TRIALS, csv);
    
    // O(n) algorithms
    benchmarkAlgorithm("counting", countingSort, sizes_fast, TRIALS, csv);
    benchmarkAlgorithm("radix", radixSort, sizes_fast, TRIALS, csv);
    
    csv.close();
    
    cout << "\n==================================" << endl;
    cout << "Benchmark complete!" << endl;
    cout << "Results written to benchmark_results.csv" << endl;
    cout << "==================================" << endl;
    
    return 0;
}
