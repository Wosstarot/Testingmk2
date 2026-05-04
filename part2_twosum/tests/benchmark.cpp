#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>
#include <functional>
#include <random>
#include "twosum.h"

using namespace std;
using namespace twosum;

// Generates n random integers in [0, maxValue] using a fixed seed for reproducibility
vector<int> generateRandomRange(size_t n, unsigned int seed, int maxValue) {
	vector<int> data(n);
	mt19937 gen(seed);
	uniform_int_distribution<int> dist(0, maxValue);

	for (size_t i = 0; i < n; ++i) {
		data[i] = dist(gen);
	}
	return data;
}

struct BenchmarkResult {
	double median_time;
};

// Run multiple trials and return median time
BenchmarkResult runTrials(
	function<TwoSumResult(const vector<int>&, int)> twosum_fn,
	function<pair<vector<int>, int>(size_t)> input_gen,
	size_t trials
) {
	vector<double> times;

	// Warm-up run
	{
		auto [arr, target] = input_gen(0);
		twosum_fn(arr, target);
	}

	// Actual trials
	for (size_t i = 0; i < trials; ++i) {
		auto [arr, target] = input_gen(i);

		auto start = chrono::high_resolution_clock::now();
		twosum_fn(arr, target);
		auto end = chrono::high_resolution_clock::now();

		chrono::duration<double> elapsed = end - start;
		times.push_back(elapsed.count());
	}

	// Use median (same as Part 1)
	sort(times.begin(), times.end());

	BenchmarkResult res;
	res.median_time = times[trials / 2];
	return res;
}

int main() {
	const size_t TRIALS = 10;

	// INT_MAX / 2 avoids overflow when summing two values for the target,
	// and keeps accidental collisions negligible across all n values tested
	const int MAX_VALUE = INT_MAX / 2;

	vector<size_t> sizes = {1000, 5000, 10000, 20000, 50000};

	ofstream csv("benchmark_results.csv");
	csv << "algorithm,n,case,time_seconds\n";

	cout << "Two-Sum Benchmarking" << endl;
	cout << "Trials per test: " << TRIALS << "\n" << endl;

	for (size_t n : sizes) {
		cout << "=== Testing n = " << n << " ===" << endl;

		// Case 1: No solution (target = -1, impossible for non-negative integers)
		// Fresh array each trial so hash table collision patterns vary naturally.
		cout << "  no_solution..." << flush;

		auto no_solution_gen = [n, MAX_VALUE](size_t trial) -> pair<vector<int>, int> {
			return { generateRandomRange(n, (unsigned int)trial, MAX_VALUE), -1 };
		};

		auto res = runTrials(bruteForceTwoSum, no_solution_gen, TRIALS);
		csv << "brute," << n << ",no_solution," << res.median_time << "\n";

		res = runTrials(hashTwoSum, no_solution_gen, TRIALS);
		csv << "hash," << n << ",no_solution," << res.median_time << "\n";

		cout << " done" << endl;

		// Case 2: Solution exists at a random position (average case).
		// Fresh array each trial, pair picked at two random distinct indices.
		// Index picker uses seed (trial + 1000) to avoid correlation with
		// the array generation seed. MAX_VALUE ensures arr[i] + arr[j]
		// cannot overflow int since values are capped at INT_MAX / 2.
		cout << "  solution_exists..." << flush;

		auto solution_gen = [n, MAX_VALUE](size_t trial) -> pair<vector<int>, int> {
			vector<int> arr = generateRandomRange(n, (unsigned int)trial, MAX_VALUE);

			mt19937 idx_gen(trial + 1000);
			uniform_int_distribution<size_t> dist(0, arr.size() - 1);

			size_t idx1 = dist(idx_gen);
			size_t idx2;
			do { idx2 = dist(idx_gen); } while (idx2 == idx1);

			return { arr, arr[idx1] + arr[idx2] };
		};

		res = runTrials(bruteForceTwoSum, solution_gen, TRIALS);
		csv << "brute," << n << ",solution_exists," << res.median_time << "\n";

		res = runTrials(hashTwoSum, solution_gen, TRIALS);
		csv << "hash," << n << ",solution_exists," << res.median_time << "\n";

		cout << " done\n" << endl;
	}

	csv.close();
	cout << "Results written to benchmark_results.csv" << endl;

	return 0;
}
