#include "MarkovChain.hpp"

#include <algorithm>
#include <sys/stat.h>

namespace ptm {
void MarkovChain::Train(const std::vector<State>& sequence) {
  for (auto state : sequence) {
    if (!state_to_index_.contains(state)) {
      index_to_state_.emplace_back(state);
      state_to_index_[state] = index_to_state_.size() - 1;
      counts_.OnAddition();
    }
  }
  for (int c = 0; c < sequence.size()-1; c++) {
    size_t i = state_to_index_[sequence[c]];
    size_t j = state_to_index_[sequence[c+1]];
    counts_.AddAt(i, j, 1);
  }
}
std::unordered_map<MarkovChain::State, double> MarkovChain::NextDistribution(const State& from) const {
  size_t i = state_to_index_.at(from);
  std::unordered_map<MarkovChain::State, double> ans = {};
  for (int j = 0; j < state_to_index_.size(); ++j) {
    State state = index_to_state_.at(j);
    ans[state] += counts_.GetAt(i, j) / counts_.GetRowSum(i);
  }
  return ans;
}
double MarkovChain::TransitionProbability(const State& from, const State& to) const {
  size_t i = state_to_index_.at(from);
  size_t j = state_to_index_.at(to);
  double count = counts_.GetAt(i, j);
  if (counts_.GetRowSum(i) == 0) {
    return 0;
  }
  return count / counts_.GetRowSum(i);
}
std::optional<MarkovChain::State> MarkovChain::SampleNext(const State& current, std::mt19937& rng) const {
  if (!state_to_index_.contains(current)) {
    return std::nullopt;
  }
  size_t i = state_to_index_.at(current);
  auto cum_sum = counts_.CumulativeRowSum(i);
  std::uniform_int_distribution<size_t> dist(0, counts_.GetRowSum(i));
  size_t random_value = dist(rng);
  auto found = std::lower_bound(cum_sum.begin(), cum_sum.end(), random_value);

  if (found != cum_sum.end()) {
    size_t next_idx = std::distance(cum_sum.begin(), found);
    return index_to_state_[next_idx-1];
  }
  return std::nullopt;
}
std::vector<MarkovChain::State> MarkovChain::Generate(const State& start, size_t length, std::mt19937& rng) const {
  std::vector<State> sequence(length);
  State currentState = start;
  for (int i = 1; i <= length; ++i) {
    auto nextStateOpt = SampleNext(currentState, rng);
    if (!nextStateOpt) {
      break;
    }
    currentState = *nextStateOpt;
    sequence[i] = currentState;
  }
  return sequence;
}
std::vector<MarkovChain::State> MarkovChain::States() const {
  return index_to_state_;
}
// size_t MarkovChain::ensureState(const State& s) {
//   return
// }

} // namespace ptm