#include "GeometricDistribution.hpp"

#include <cassert>
#include <cmath>
#include <random>

using namespace ptm;

GeometricDistribution::GeometricDistribution(double p) : p_(p) {}

double GeometricDistribution::Pdf(double x) const {
    if (x < 1) return 0;

    x = std::floor(x);

    double tmp = 1.0;

    for (int i = 0; i < x - 1; ++i) {
        tmp *= 1 - p_;
    }

    return p_ * tmp;
}

double GeometricDistribution::Cdf(double x) const {
    if (x < 1) return 0;

    x = std::floor(x);

    double tmp = 1.0;

    for (int i = 0; i < x; ++i) {
        tmp *= 1 - p_;
    }

    return 1 - tmp;
}

double GeometricDistribution::Sample(std::mt19937& rnd) const {
    std::geometric_distribution<> dist(p_);

    return dist(rnd) + 1.;
}

double GeometricDistribution::TheoreticalMean() const {
    return 1. / p_;
}

double GeometricDistribution::TheoreticalVariance() const {
    return (1.0 - p_) / (p_ * p_);
}