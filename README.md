# CSCI115 Algorithm Analysis Project

## Team Information
**Team Name:** Big-O Brigade

**Members:**
- Joseph G.
- Kenneth C.
- Kevin T.

---

## Environment

**Language:** C++17  
**Compiler:** g++ 11.4.0 or later  
**Python:** 3.8+ (for graph generation)  
**Operating System:** Linux, macOS, or Windows (with MinGW/MSYS2)

**Dependencies:**
- Standard C++ library (no external dependencies for C++ code)
- Python packages: `matplotlib`, `pandas`, `numpy` (for graphing only)

**Hardware:** Tested on modern x86-64 systems (Intel/AMD)

**Note:** Benchmark results are timing-sensitive and will vary based on CPU speed, system load, and compiler optimizations.

---

## Project Structure

```
CSCI115_TeamName_Project/
├── README.md                  # This file
├── .gitignore
├── part1_sorting/
│   ├── src/                   # Sorting algorithm implementations
│   │   ├── sorting.h          # Common header
│   │   ├── bubble.cpp
│   │   ├── selection.cpp
│   │   ├── insertion.cpp
│   │   ├── merge.cpp
│   │   ├── quick.cpp
│   │   ├── heap.cpp
│   │   ├── counting.cpp
│   │   └── radix.cpp
│   ├── tests/                 # Test suite and benchmarking
│   │   ├── test_sorting.cpp   # Correctness tests
│   │   ├── benchmark.cpp      # Performance benchmarking
│   │   ├── generator.h
│   │   └── generator.cpp      # Input generation
│   ├── Makefile               # Build system
│   ├── plot_results.py        # Graph generation script
│   └── (graphs/)              # Generated plots (created by script)
└── part2_twosum/
    ├── src/                   # Two-Sum implementations
    │   ├── twosum.h
    │   └── twosum.cpp
    ├── tests/                 # Test suite and benchmarking
    │   ├── test_twosum.cpp
    │   └── benchmark.cpp
    ├── Makefile
    ├── plot_results.py
    └── (graphs/)              # Generated plots
```

---

## Part 1: Sorting Algorithms

### Implemented Algorithms

**O(n²) Comparison Sorts:**
- Bubble Sort (unoptimized)
- Selection Sort
- Insertion Sort

**O(n log n) Comparison Sorts:**
- Merge Sort (with auxiliary buffer)
- Quicksort (median-of-three pivot)
- Heapsort

**O(n) Non-Comparison Sorts:**
- Counting Sort
- Radix Sort

All algorithms return sorted data and comparison count (0 for non-comparison sorts).

### Build Instructions

**Linux / macOS:**
```bash
cd part1_sorting
make              # Build benchmark (release mode)
make test         # Build and run correctness tests
make clean        # Remove build artifacts
```

**Windows (MinGW/MSYS2):**
```bash
cd part1_sorting
mingw32-make      # Build benchmark
mingw32-make test # Run tests
```

**Alternative (Windows):** Use WSL (Windows Subsystem for Linux) and follow Linux instructions.

### Run Correctness Tests

```bash
cd part1_sorting
make test
```

**Expected Output:**
```
Testing Insertion Sort...
  ✓ All tests PASSED
Testing Selection Sort...
  ✓ All tests PASSED
...
✓ ALL TESTS PASSED
```

**Total:** 88 tests (11 per algorithm)

**Tests validate:**
- Edge cases (empty arrays, single elements)
- Input patterns (sorted, reversed, random, duplicates)
- Correctness of sorting
- Element preservation

### Run Benchmark

```bash
cd part1_sorting
./build/release/benchmark
```

**Runtime:** ~5-7 minutes  
**Output:** `benchmark_results.csv`

**CSV Format:**
```
algorithm,n,case,time_seconds,comparisons
insertion,1000,sorted,0.000006,999
insertion,1000,reversed,0.000086,499500
insertion,1000,random,0.000063,243974
...
```

**Input sizes tested:** 1000, 2000, 4000, 8000, 16000, 32000, 50000  
**Test cases per size:** sorted, reversed, random  
**Trials per test:** 10 (median reported)

### Generate Graphs

**Requirements:**
```bash
pip install matplotlib pandas numpy
```

**Generate:**
```bash
cd part1_sorting
python3 plot_results.py
```

**Output:** 7 graphs in `graphs/` directory

**Generated plots:**
- `compare_n2.png` - O(n²) algorithms comparison
- `compare_nlogn.png` - O(n log n) algorithms comparison
- `compare_n.png` - O(n) algorithms comparison
- `compare_all_comparison_based.png` - All comparison sorts (log-log scale)
- `algo_*.png` - Per-algorithm breakdown (sorted/random/reversed cases)

### Notes

- **Bubble sort:** Unoptimized version (no early termination)
- **Input range:** [0, n-1] for all generators
- **Comparison counting:** Incremented before each comparison operation
- **Median statistic:** Robust to outliers from OS interrupts
- **O(n²) cap:** Limited to n=50,000 to avoid excessive runtime

---

## Part 2: Two-Sum Problem

### Problem Statement

Given an array of integers and a target sum, determine if two distinct elements sum to the target value.

### Implemented Solutions

**1. Brute Force**
- **Algorithm:** Check all pairs using nested loops
- **Time Complexity:** O(n²)
- **Space Complexity:** O(1)

**2. Hash Table**
- **Algorithm:** Single pass with hash map lookup
- **Time Complexity:** O(n) average case, O(n²) worst case
- **Space Complexity:** O(n)

Both return `TwoSumResult` struct containing:
- `found` - Boolean indicating if solution exists
- `index1, index2` - Indices of the pair (or -1 if not found)
- `value1, value2` - Values of the pair (or 0 if not found)

### Build Instructions

**Linux / macOS:**
```bash
cd part2_twosum
make              # Build benchmark
make test         # Run correctness tests
make clean        # Remove build artifacts
```

**Windows:**
```bash
cd part2_twosum
mingw32-make
mingw32-make test
```

### Run Correctness Tests

```bash
cd part2_twosum
make test
```

**Expected Output:**
```
Testing Brute Force...
  ✓ All tests PASSED
Testing Hash Table...
  ✓ All tests PASSED
✓ ALL TESTS PASSED
```

**Total:** 20 tests (10 per algorithm)

**Tests cover:**
- Empty arrays
- Single element
- No solution exists
- Solution exists (various positions)
- Duplicate values
- Large arrays

### Run Benchmark

```bash
cd part2_twosum
./build/release/benchmark
```

**Runtime:** ~2-3 minutes  
**Output:** `benchmark_results.csv`

**CSV Format:**
```
algorithm,n,case,time_seconds
brute,1000,no_solution,0.00033
hash,1000,no_solution,0.00024
brute,1000,solution_exists,0.00005
hash,1000,solution_exists,0.00010
...
```

**Input sizes tested:** 1000, 5000, 10000, 20000, 50000  
**Test cases:**
- `no_solution` - Target = -1 (worst case, full traversal)
- `solution_exists` - Random pair positions (average case)

**Methodology:**
- Fresh random array generated per trial
- Value range: [0, INT_MAX/2] to prevent overflow
- Trials per test: 10 (median reported)

### Generate Graphs

**Requirements:** Same as Part 1

**Generate:**
```bash
cd part2_twosum
python3 plot_results.py
```

**Output:** 2 graphs in `graphs/` directory

**Generated plots:**
- `twosum_no_solution.png` - Worst case comparison
- `twosum_solution_exists.png` - Best/average case comparison

### Notes

- **Value range:** [0, INT_MAX/2] eliminates hash collisions and prevents integer overflow
- **Solution generation:** Two random distinct indices per trial
- **Hash implementation:** Uses `std::unordered_map` with O(1) average lookup
- **Performance observation:** Hash overhead visible at small n (brute force faster at n=1000)

---

## Building Everything

**From project root:**

```bash
# Build both parts
cd part1_sorting && make && cd ..
cd part2_twosum && make && cd ..

# Run all tests
cd part1_sorting && make test && cd ..
cd part2_twosum && make test && cd ..
```

---

## Reproducing Results

### Full Workflow (Linux/macOS)

```bash
# 1. Clean build
cd part1_sorting && make clean && cd ..
cd part2_twosum && make clean && cd ..

# 2. Build executables
cd part1_sorting && make && cd ..
cd part2_twosum && make && cd ..

# 3. Run tests (verify correctness)
cd part1_sorting && make test && cd ..
cd part2_twosum && make test && cd ..

# 4. Run benchmarks (generate CSVs)
cd part1_sorting && ./build/release/benchmark && cd ..
cd part2_twosum && ./build/release/benchmark && cd ..

# 5. Generate graphs (requires Python)
cd part1_sorting && python3 plot_results.py && cd ..
cd part2_twosum && python3 plot_results.py && cd ..
```

**Total time:** ~10-15 minutes (depending on CPU speed)

---

## Compiler Flags

**Release mode (default):**
- `-O3` - Aggressive optimization
- `-march=native` - CPU-specific optimizations
- `-DNDEBUG` - Disable assertions
- `-std=c++17` - C++17 standard
- `-Wall -Wextra` - Enable warnings

**Debug mode:**
- `-O0` - No optimization
- `-g` - Debug symbols
- `-std=c++17`

**To build in debug mode:**
```bash
make clean
make BUILD=debug
```

---

## Known Issues & Limitations

### Part 1
- Benchmarks take 5-7 minutes to complete (can be longer on slower systems)
- O(n²) algorithms capped at n=50,000 (prohibitive runtime at larger sizes)
- Graph generation requires Python with matplotlib/pandas installed
- `-march=native` flag may not work on all systems (remove if compilation fails)

### Part 2
- Benchmarks take 2-3 minutes to complete
- Hash table performance depends on std::unordered_map implementation
- Results show hash overhead at small n where brute force is competitive

### Platform-Specific
- **Windows:** Requires MinGW/MSYS2 or WSL for make/g++
- **macOS:** May require XCode command-line tools for g++
- **All platforms:** Graph generation requires Python 3.8+ with packages

---

## Assumptions

- **Input data:** Non-negative integers in specified ranges
- **Memory:** Sufficient RAM for largest test size (n=50,000)
- **Timing:** System is relatively idle during benchmarks for consistent results
- **Compiler:** Modern g++ with C++17 support (version 11.x or later recommended)

