#include <gtest/gtest.h>
#include <random>

#include "lib/distributions/BernoulliDistribution.hpp"
#include "lib/distributions/CauchyDistribution.hpp"
#include "lib/distributions/ExponentialDistribution.hpp"
#include "lib/distributions/LaplaceDistribution.hpp"
#include "lib/distributions/UniformDistribution.hpp"
#include "lib/law-of-large-numbers/LawOfLargeNumbersSimulator.hpp"

TEST(LawOfLargeNumbersTest, BernoulliMeanConverges) {
  using namespace ptm;

  std::mt19937 rng(123);

  auto dist = std::make_shared<BernoulliDistribution>(0.3);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 100000;
  size_t step = 5000;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  const double theoretical_mean = dist->TheoreticalMean();

  double first_error = result.entries.front().abs_error;
  double last_error = result.entries.back().abs_error;
  double last_mean = result.entries.back().sample_mean;

  EXPECT_GT(first_error, last_error);

  EXPECT_NEAR(last_mean, theoretical_mean, 0.05);

  EXPECT_LT(last_error, 0.05);

  for (std::size_t i = 1; i < result.entries.size(); ++i) {
    EXPECT_EQ(result.entries[i].n, result.entries[i - 1].n + step);
  }
}

TEST(LawOfLargeNumbersTest, UniformMeanConverges) {
  using namespace ptm;

  std::mt19937 rng(123);

  auto dist = std::make_shared<UniformDistribution>(0.0, 10.0);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 100000;
  size_t step = 5000;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  const double theoretical_mean = dist->TheoreticalMean();
  double last_error = result.entries.back().abs_error;

  EXPECT_NEAR(result.entries.back().sample_mean, theoretical_mean, 0.1);
  EXPECT_LT(last_error, 0.1);
}

TEST(LawOfLargeNumbersTest, CauchyDoesNotConverge) {
  using namespace ptm;

  std::mt19937 rng(123);

  auto dist = std::make_shared<CauchyDistribution>(0.0, 1.0);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 1000;
  size_t step = 100;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  for (const auto& entry : result.entries) {
    EXPECT_TRUE(std::isnan(entry.abs_error));
  }
}

TEST(LawOfLargeNumbersTest, ExponentialMeanConverges) {
  using namespace ptm;

  std::mt19937 rng(123);

  double lambda = 2.0;
  auto dist = std::make_shared<ExponentialDistribution>(lambda);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 100000;
  size_t step = 5000;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  const double theoretical_mean = dist->TheoreticalMean();

  EXPECT_NEAR(result.entries.back().sample_mean, theoretical_mean, 0.05);
  EXPECT_LT(result.entries.back().abs_error, 0.05);
}

TEST(LawOfLargeNumbersTest, LaplaceMeanConverges) {
  using namespace ptm;

  std::mt19937 rng(123);

  double mu = 5.0;
  double b = 2.0;
  auto dist = std::make_shared<LaplaceDistribution>(mu, b);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 100000;
  size_t step = 5000;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  const double theoretical_mean = dist->TheoreticalMean();

  EXPECT_NEAR(result.entries.back().sample_mean, theoretical_mean, 0.05);
  EXPECT_LT(result.entries.back().abs_error, 0.05);
}

TEST(LawOfLargeNumbersTest, InvalidParameters) {
  using namespace ptm;

  std::mt19937 rng(123);
  auto dist = std::make_shared<BernoulliDistribution>(0.5);
  LawOfLargeNumbersSimulator sim(dist);

  EXPECT_THROW(LawOfLargeNumbersSimulator(nullptr), std::logic_error);

  EXPECT_THROW(sim.Simulate(rng, 100, 0), std::logic_error);

  EXPECT_THROW(sim.Simulate(rng, 50, 100), std::logic_error);
}

TEST(LawOfLargeNumbersTest, NonMultipleStep) {
  using namespace ptm;

  std::mt19937 rng(123);
  auto dist = std::make_shared<BernoulliDistribution>(0.5);
  LawOfLargeNumbersSimulator sim(dist);

  LLNPathResult result = sim.Simulate(rng, 10, 3);

  ASSERT_EQ(result.entries.size(), 3);
  EXPECT_EQ(result.entries[0].n, 3);
  EXPECT_EQ(result.entries[1].n, 6);
  EXPECT_EQ(result.entries[2].n, 9);
}
