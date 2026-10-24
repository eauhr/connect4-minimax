#pragma once
#include <random>

constexpr unsigned kDefaultSeed = 12345;

inline std::mt19937& rng() {
    static std::mt19937 gen(kDefaultSeed);
    return gen;
}

inline void seedRng(unsigned seed) { rng().seed(seed); }

inline int randInt(int n) {
    return static_cast<int>(rng()() % static_cast<unsigned>(n));
}