#include <sstream>

#include <gtest/gtest.h>
#include "lib/sigma-algebra/DiscreteRandomVariable.hpp"
#include "lib/sigma-algebra/Event.hpp"
#include "lib/sigma-algebra/OutcomeSpace.hpp"
#include "lib/sigma-algebra/ProbabilityMeasure.hpp"
#include "lib/sigma-algebra/SigmaAlgebra.hpp"

TEST(SigmaAlgebraTest, ProbabilityMeasureAndExpectation) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");
  auto w2 = omega.AddOutcome("3");

  ProbabilityMeasure P(omega);
  P.SetAtomicProbability(w0, 0.2);
  P.SetAtomicProbability(w1, 0.3);
  P.SetAtomicProbability(w2, 0.5);

  EXPECT_TRUE(P.IsValid(1e-9));

  // событие A = {1,3}
  std::vector<bool> mask(omega.GetSize(), false);
  mask[w0] = true;
  mask[w2] = true;
  Event A(mask);

  double pA = P.Probability(A);
  EXPECT_NEAR(pA, 0.7, 1e-9);

  // X(1)=1, X(2)=2, X(3)=3
  std::vector<double> X_values = {1.0, 2.0, 3.0};
  DiscreteRandomVariable X(omega, P, X_values);

  double EX = X.ExpectedValue();
  // E[X] = 1*0.2 + 2*0.3 + 3*0.5 = 2.3
  EXPECT_NEAR(EX, 2.3, 1e-9);
}

TEST(SigmaAlgebraTest, EventOperations) {
  using namespace ptm;

  OutcomeSpace omega;
  auto a = omega.AddOutcome("a");
  auto b = omega.AddOutcome("b");
  auto c = omega.AddOutcome("c");

  auto E1 = ptm::Event::Empty(omega.GetSize());
  auto E2 = ptm::Event::Full(omega.GetSize());

  EXPECT_FALSE(E1.Contains(a));
  EXPECT_TRUE(E2.Contains(a));
  EXPECT_TRUE(E2.Contains(b));
  EXPECT_TRUE(E2.Contains(c));

  auto E3 = Event::Complement(E2);
  EXPECT_FALSE(E3.Contains(a));
  EXPECT_FALSE(E3.Contains(b));
  EXPECT_FALSE(E3.Contains(c));
}

TEST(ProbSpaceTest, EventLogic) {
  using namespace ptm;
  OutcomeSpace omega;
  omega.AddOutcome("Head");
  omega.AddOutcome("Tail");

  auto E_empty = Event::Empty(2);
  auto E_full = Event::Full(2);

  auto E_full_calc = Event::Complement(E_empty);
  EXPECT_EQ(E_full_calc.GetMask(), E_full.GetMask());

  std::vector<bool> m1 = {true, false};
  std::vector<bool> m2 = {false, true};
  Event e1(m1), e2(m2);

  auto e_union = Event::Unite(e1, e2);
  EXPECT_EQ(e_union.GetMask(), E_full.GetMask());

  auto e_inter = Event::Intersect(e1, e2);
  EXPECT_EQ(e_inter.GetMask(), E_empty.GetMask());
}

TEST(SigmaAlgebraTest, Validation) {
  using namespace ptm;
  OutcomeSpace omega;
  omega.AddOutcome("1");
  omega.AddOutcome("2");
  omega.AddOutcome("3");

  std::vector<Event> trivial = {Event::Empty(3), Event::Full(3)};
  SigmaAlgebra sa1(omega, trivial);
  EXPECT_TRUE(sa1.IsSigmaAlgebra());

  std::vector<bool> m1 = {true, false, false};
  std::vector<Event> broken = trivial;
  broken.emplace_back(m1);

  SigmaAlgebra sa2(omega, broken);
  EXPECT_FALSE(sa2.IsSigmaAlgebra());

  std::vector<bool> m1_comp = {false, true, true};
  broken.emplace_back(m1_comp);
  SigmaAlgebra sa3(omega, broken);
  EXPECT_TRUE(sa3.IsSigmaAlgebra());
}

TEST(SigmaAlgebraTest, Generation) {
  using namespace ptm;
  OutcomeSpace omega;
  omega.AddOutcome("1");
  omega.AddOutcome("2");
  omega.AddOutcome("3");
  omega.AddOutcome("4");

  std::vector<bool> ma = {true, false, false, false};
  std::vector<Event> gens;
  gens.emplace_back(ma);

  SigmaAlgebra sa = SigmaAlgebra::Generate(omega, gens);
  EXPECT_TRUE(sa.IsSigmaAlgebra());
  EXPECT_EQ(sa.GetEvents().size(), 4);

  std::vector<bool> mb = {false, true, false, false};
  gens.emplace_back(mb);

  SigmaAlgebra sa2 = SigmaAlgebra::Generate(omega, gens);
  EXPECT_TRUE(sa2.IsSigmaAlgebra());
  EXPECT_EQ(sa2.GetEvents().size(), 8);
}
