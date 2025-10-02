#pragma once
#include <random>
#include <cstdint>
#include <initializer_list>

class RNG {
public:
    explicit RNG(uint64_t seed = 0xC0FFEEULL) : eng_(seed) {}
    double uniform01() { return std::generate_canonical<double, 53>(eng_); }
    int choose_by_cdf(std::initializer_list<double> cdf) {
        double r = uniform01(); double sum = 0.0; int idx = 0;
        for (double p : cdf) { sum += p; if (r < sum) return idx; ++idx; }
        return static_cast<int>(cdf.size()) - 1;
    }
    void reseed(uint64_t seed) { eng_.seed(seed); }
private:
    std::mt19937_64 eng_;
};