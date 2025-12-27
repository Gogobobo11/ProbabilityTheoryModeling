#include <gtest/gtest.h>

#include <cmath>
#include <numbers>
#include <random>
#include <ranges>

#include "distributions/ExponentialDistribution.hpp"
#include "distributions/MaxwellDistribution.hpp"
#include "lib/distributions/BernoulliDistribution.hpp"
#include "lib/distributions/BinomialDistribution.hpp"
#include "lib/distributions/CauchyDistribution.hpp"
#include "lib/distributions/DistributionExperiment.hpp"
#include "lib/distributions/GeometricDistribution.hpp"
#include "lib/distributions/LaplaceDistribution.hpp"
#include "lib/distributions/LogisticDistribution.hpp"
#include "lib/distributions/NormalDistribution.hpp"
#include "lib/distributions/PoissonDistribution.hpp"
#include "lib/distributions/UniformDistribution.hpp"

TEST(DistributionTest, NormalDistributionBasicProperties) {
  using namespace ptm;

  NormalDistribution nd(0.0, 1.0);
  double pdf0 = nd.Pdf(0.0);
  double cdf0 = nd.Cdf(0.0);

  EXPECT_NEAR(pdf0, 0.3989, 1e-3);

  EXPECT_NEAR(cdf0, 0.5, 1e-3);
}

TEST(DistributionExperimentTest, EmpiricalMeanCloseToTheoretical) {
  using namespace ptm;

  std::mt19937 rng(123);

  auto dist = std::make_shared<NormalDistribution>(5.0, 2.0);
  DistributionExperiment experiment(dist, 20000);

  auto stats = experiment.Run(rng);

  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.1);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.3);
}

TEST(DistributionTest, UniformDistributionBasicProperties) {
  using namespace ptm;

  UniformDistribution ud(0.0, 2.0);
  EXPECT_NEAR(ud.Pdf(1.0), 0.5, 1e-9);
  EXPECT_NEAR(ud.Cdf(0.0), 0.0, 1e-9);
  EXPECT_NEAR(ud.Cdf(2.0), 1.0, 1e-9);

  EXPECT_NEAR(ud.TheoreticalMean(), 1.0, 1e-9);
  EXPECT_NEAR(ud.TheoreticalVariance(), 1.0 / 3.0, 1e-9);
}

TEST(DistributionTest, BernoulliDistributionBasic) {
  using namespace ptm;

  BernoulliDistribution bd(0.3);
  EXPECT_NEAR(bd.Pdf(0.0), 0.7, 1e-9);
  EXPECT_NEAR(bd.Pdf(1.0), 0.3, 1e-9);
  EXPECT_NEAR(bd.Cdf(0.5), 0.7, 1e-9);
  EXPECT_NEAR(bd.TheoreticalMean(), 0.3, 1e-9);
  EXPECT_NEAR(bd.TheoreticalVariance(), 0.21, 1e-9);
}

TEST(DistributionTest, BinomialDistributionBasic) {
  using namespace ptm;

  BinomialDistribution bd(10, 0.5);
  double p5 = bd.Pdf(5.0);
  EXPECT_NEAR(p5, 0.246, 1e-2);

  EXPECT_NEAR(bd.TheoreticalMean(), 5.0, 1e-9);
  EXPECT_NEAR(bd.TheoreticalVariance(), 2.5, 1e-9);
}

TEST(DistributionTest, GeometricDistributionBasic) {
  using namespace ptm;

  double p = 0.4;
  GeometricDistribution gd(p);

  EXPECT_NEAR(gd.Pdf(1.0), p, 1e-9);
  EXPECT_NEAR(gd.Cdf(3.0), 1.0 - std::pow(1.0 - p, 3), 1e-9);

  EXPECT_NEAR(gd.TheoreticalMean(), 1.0 / p, 1e-9);
  EXPECT_NEAR(gd.TheoreticalVariance(), (1.0 - p) / (p * p), 1e-9);
}

TEST(DistributionTest, PoissonDistributionBasic) {
  using namespace ptm;

  double lambda = 3.0;
  PoissonDistribution pd(lambda);

  EXPECT_NEAR(pd.Pdf(0.0), std::exp(-lambda), 1e-9);

  EXPECT_NEAR(pd.TheoreticalMean(), lambda, 1e-9);
  EXPECT_NEAR(pd.TheoreticalVariance(), lambda, 1e-9);

  double cdf1 = pd.Cdf(1.0);
  double p0 = pd.Pdf(0.0);
  double p1 = pd.Pdf(1.0);
  EXPECT_NEAR(cdf1, p0 + p1, 1e-6);
}

TEST(DistributionTest, CauchyDistributionBasic) {
  using namespace ptm;

  CauchyDistribution cd(0.0, 1.0);
  EXPECT_NEAR(cd.Pdf(0.0), 1.0 / (std::numbers::pi * 1.0), 1e-9);
  EXPECT_NEAR(cd.Cdf(0.0), 0.5, 1e-9);
}

TEST(DistributionTest, LaplaceDistributionBasic) {
  using namespace ptm;

  LaplaceDistribution ld(0.0, 1.0);
  EXPECT_NEAR(ld.Pdf(0.0), 0.5, 1e-9);
  EXPECT_NEAR(ld.Cdf(0.0), 0.5, 1e-9);

  EXPECT_NEAR(ld.TheoreticalMean(), 0.0, 1e-9);
  EXPECT_NEAR(ld.TheoreticalVariance(), 2.0, 1e-9);
}

TEST(DistributionTest, MaxwellDistributionBasic) {
  using namespace ptm;
  using namespace std::numbers;

  double a = 2;
  double const kErf1 = 0.8427008;

  MaxwellDistribution md(a);
  EXPECT_NEAR(md.Pdf(2. * sqrt2), sqrt2 * inv_sqrtpi / e, 1e-7);
  EXPECT_NEAR(md.Cdf(2 * sqrt2), kErf1 - 2 * inv_sqrtpi / e, 1e-7);

  EXPECT_NEAR(md.TheoreticalMean(), 4 * sqrt2 * inv_sqrtpi, 1e-7);
  EXPECT_NEAR(md.TheoreticalVariance(), (12 * pi - 32) * inv_pi, 1e-7);
}

TEST(DistributionTest, LogisticDistributionBasic) {
  using namespace ptm;

  double mu = 3;
  double s = std::numbers::inv_pi;

  LogisticDistribution ld(mu, s);
  EXPECT_NEAR(ld.Pdf(mu), 1 / (4 * s), 1e-9);
  EXPECT_NEAR(ld.Cdf(mu), 0.5, 1e-9);

  EXPECT_NEAR(ld.TheoreticalMean(), mu, 1e-9);
  EXPECT_NEAR(ld.TheoreticalVariance(), 1. / 3, 1e-9);
}

TEST(DistributionExperimentTest, BinomialEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<BinomialDistribution>(20, 0.3);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, BernoulliEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<BernoulliDistribution>(0.4);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, GeometricEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<GeometricDistribution>(0.3);
  DistributionExperiment experiment(dist, 10000000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, LaplaceEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<LaplaceDistribution>(0.0, 1.0);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, NormalEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<NormalDistribution>(0.0, 1.0);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, PoissonEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<PoissonDistribution>(4.5);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, UniformEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<UniformDistribution>(2.0, 5.0);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, MaxwellEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<MaxwellDistribution>(1.0);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);
  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, LogisticEmpiricalMean) {
  using namespace ptm;

  std::mt19937 rng(777);
  auto dist = std::make_shared<LogisticDistribution>(0.0, 1.0);
  DistributionExperiment experiment(dist, 100000);

  auto stats = experiment.Run(rng);
  EXPECT_NEAR(stats.empirical_mean, dist->TheoreticalMean(), 0.05);

  EXPECT_NEAR(stats.empirical_variance, dist->TheoreticalVariance(), 0.05);
}

TEST(DistributionExperimentTest, EmpiricalCdfAndKolmogorovDistance) {
  using namespace ptm;

  auto tmp = std::views::iota(1, 20);

  std::vector<double> grid(tmp.begin(), tmp.end());

  std::mt19937 rng(777);
  size_t sample_size = 40000;

  auto dist_maxwell = std::make_shared<MaxwellDistribution>(5.);
  auto dist_laplace = std::make_shared<LaplaceDistribution>(5, 4);
  auto dist_exponential = std::make_shared<ExponentialDistribution>(0.01);
  auto dist_logistic = std::make_shared<LogisticDistribution>(9, 3);

  DistributionExperiment experiment_maxwell(dist_maxwell, sample_size);
  DistributionExperiment experiment_laplace(dist_laplace, sample_size);
  DistributionExperiment experiment_exponential(dist_exponential, sample_size);
  DistributionExperiment experiment_logistic(dist_logistic, sample_size);

  auto emp_cdf_maxwell = experiment_maxwell.EmpiricalCdf(grid, rng, sample_size);
  auto emp_cdf_laplace = experiment_laplace.EmpiricalCdf(grid, rng, sample_size);
  auto emp_cdf_exponential = experiment_exponential.EmpiricalCdf(grid, rng, sample_size);
  auto emp_cdf_logistic = experiment_logistic.EmpiricalCdf(grid, rng, sample_size);

  EXPECT_NEAR(experiment_maxwell.KolmogorovDistance(grid, emp_cdf_maxwell), 0, 0.01);
  EXPECT_NEAR(experiment_laplace.KolmogorovDistance(grid, emp_cdf_laplace), 0, 0.01);
  EXPECT_NEAR(experiment_exponential.KolmogorovDistance(grid, emp_cdf_exponential), 0, 0.01);
  EXPECT_NEAR(experiment_logistic.KolmogorovDistance(grid, emp_cdf_logistic), 0, 0.01);
}

