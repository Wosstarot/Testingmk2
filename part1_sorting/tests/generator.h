#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

std::vector<int> generateSorted(size_t n);
std::vector<int> generateReversed(size_t n);
std::vector<int> generateRandom(size_t n, unsigned int seed = 42);
std::vector<int> generateNearlySorted(size_t n, unsigned int seed = 42);
std::vector<int> generateFewUnique(size_t n, unsigned int seed = 42);

#endif
