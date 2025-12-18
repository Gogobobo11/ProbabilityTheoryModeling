#include "BernoulliDistribution.hpp"

#include <cassert>
#include <random>

using namespace ptm;

BernoulliDistribution::BernoulliDistribution(double p) : p_(p) {
    assert(p <= 1 and p >= 0);
}

double BernoulliDistribution::Pdf(double x) const {
    if (x == 1.0) return p_;

    if (x == 0.0) return 1 - p_;

    return 0;
}

double BernoulliDistribution::Cdf(double x) const {
    if (x < 0) return 0;

    if (x < 1) return 1 - p_;

    return 1;
}

double BernoulliDistribution::Sample(std::mt19937& rnd) const {
    std::bernoulli_distribution dist(p_);

    return dist(rnd);
}

double BernoulliDistribution::TheoreticalMean() const {
    return p_;
}

double BernoulliDistribution::TheoreticalVariance() const {
    return p_ * (1 - p_);
}