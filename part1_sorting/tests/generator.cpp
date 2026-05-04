#include "generator.h"
#include <algorithm>
#include <random>
#include <numeric>

std::vector<int> generateSorted(size_t n) {
    std::vector<int> data(n);
    std::iota(data.begin(), data.end(), 0);
    return data;
}

std::vector<int> generateReversed(size_t n) {
    std::vector<int> data(n);
    std::iota(data.begin(), data.end(), 0);
    std::reverse(data.begin(), data.end());
    return data;
}

std::vector<int> generateRandom(size_t n, unsigned int seed) {
    std::vector<int> data(n);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, n - 1);
    
    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(gen);
    }
    return data;
}

std::vector<int> generateNearlySorted(size_t n, unsigned int seed) {
    std::vector<int> data = generateSorted(n);
    std::mt19937 gen(seed);
    
    size_t swaps = std::max(size_t(1), n / 20);
    std::uniform_int_distribution<size_t> dist(0, n - 2);
    
    for (size_t i = 0; i < swaps; ++i) {
        size_t pos = dist(gen);
        std::swap(data[pos], data[pos + 1]);
    }
    
    return data;
}

std::vector<int> generateFewUnique(size_t n, unsigned int seed) {
    std::vector<int> data(n);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 4);
    
    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(gen);
    }
    return data;
}
