#include "BinomialDistribution.hpp"

#include <cassert>
#include <cmath>
#include <random>
#include <vector>

using namespace ptm;

double C(long long n, long long k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    std::vector<std::vector<long long>> C(n + 1, std::vector<long long>(k + 1));

    for (long long i = 0; i <= n; ++i) {
        for (long long j = 0; j <= std::min(i, k); ++j) {
            if (j == 0 || j == i) {
                C[i][j] = 1;
                continue;
            }

            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }

    return (double)C[n][k];
}

BinomialDistribution::BinomialDistribution(long long n, double p) : n_(n), p_(p) {
    assert(n >= 1 and p <= 1 and p >= 0);
}

double BinomialDistribution::Pdf(double x) const {
    return C(n_, (long long)x) * std::pow(p_, x) * std::pow(1 - p_, x);
}

double BinomialDistribution::Cdf(double x) const {
    double tmp = 0;

    for (int i = 0; i <= x; ++i) {
        tmp += C(n_, i) * std::pow(p_, i) * std::pow(1 - p_, n_ - i);
    }

    return tmp;
}

double BinomialDistribution::Sample(std::mt19937& rnd) const {
    std::binomial_distribution<long long> dist(n_, p_);

    return (double)dist(rnd);
}

double BinomialDistribution::TheoreticalMean() const {
    return p_ * (double)n_;
}

double BinomialDistribution::TheoreticalVariance() const {
    return p_ * (1 - p_) * (double)n_;
}