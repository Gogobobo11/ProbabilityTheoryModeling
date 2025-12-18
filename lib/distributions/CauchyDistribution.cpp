#include "CauchyDistribution.hpp"

#include <cassert>
#include <cmath>
#include <numbers>
#include <random>

using namespace ptm;

CauchyDistribution::CauchyDistribution(double x0, double gamma) : x0_(x0), gamma_(gamma) {
    assert(gamma > 0);
}

double CauchyDistribution::Pdf(double x) const {
    return 1 / (
            std::numbers::pi * 
            gamma_ * 
            (1 + std::pow((x - x0_)/gamma_, 2))
    );
}

double CauchyDistribution::Cdf(double x) const {
    return std::numbers::inv_pi * std::atan((x - x0_) / gamma_) + 0.5;
}

double CauchyDistribution::Sample(std::mt19937& rnd) const {
    std::cauchy_distribution<> dist(x0_, gamma_);

    return dist(rnd);
}

double CauchyDistribution::TheoreticalMean() const {
    return std::nan("");
}

double CauchyDistribution::TheoreticalVariance() const {
    return std::nan("");
}