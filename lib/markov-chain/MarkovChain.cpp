#include "MarkovChain.hpp"

#include <algorithm>
#include <cstddef>

namespace ptm {
void MarkovChain::Train(const std::vector<State>& sequence) {
  for (auto state : sequence) {
    if (!state_to_index_.contains(state)) {
      index_to_state_.emplace_back(state);
      state_to_index_[state] = index_to_state_.size() - 1;
    }
  }
  counts_.Resize(index_to_state_.size());
  for (size_t c = 0; c < sequence.size() - 1; c++) {
    size_t i = state_to_index_[sequence[c]];
    size_t j = state_to_index_[sequence[c + 1]];
    counts_.AddAt(i, j, 1);
  }
}
std::unordered_map<MarkovChain::State, double> MarkovChain::NextDistribution(const State& from) const {
  if (!state_to_index_.contains(from)) {
    return {};
  }
  size_t i = state_to_index_.at(from);
  double row_sum = counts_.GetRowSum(i);
  if (row_sum == 0) {
    return {};
  }
  std::unordered_map<MarkovChain::State, double> ans = {};
  for (size_t j = 0; j < index_to_state_.size(); ++j) {
    double count = counts_.GetAt(i, j);
    if (count > 0) {
      ans[index_to_state_[j]] = count / row_sum;
    }
  }
  return ans;
}
double MarkovChain::TransitionProbability(const State& from, const State& to) const {
  if (!state_to_index_.contains(from) || !state_to_index_.contains(to)) {
    return 0.0;
  }
  size_t i = state_to_index_.at(from);
  size_t j = state_to_index_.at(to);
  double row_sum = counts_.GetRowSum(i);
  if (row_sum == 0) {
    return 0.0;
  }
  return static_cast<double>(counts_.GetAt(i, j)) / row_sum;
}
std::optional<MarkovChain::State> MarkovChain::SampleNext(const State& current, std::mt19937& rng) const {
  if (!state_to_index_.contains(current)) {
    return std::nullopt;
  }
  size_t i = state_to_index_.at(current);
  size_t row_sum = counts_.GetRowSum(i);
  if (row_sum == 0) {
    return std::nullopt;
  }
  auto cum_sum = counts_.CumulativeRowSum(i);
  std::uniform_int_distribution<size_t> dist(1, row_sum);
  size_t random_value = dist(rng);
  auto found = std::lower_bound(cum_sum.begin(), cum_sum.end(), random_value);

  if (found != cum_sum.end()) {
    size_t next_idx = std::distance(cum_sum.begin(), found);
    auto next_s_ind = counts_.GetKeys(i)[next_idx - 1];
    return index_to_state_[next_s_ind];
  }
  return std::nullopt;
}
std::vector<MarkovChain::State> MarkovChain::Generate(const State& start, size_t length, std::mt19937& rng) const {
  std::vector<State> sequence;
  sequence.reserve(length);
  State currentState = start;
  for (size_t i = 0; i < length; ++i) {
    auto nextStateOpt = SampleNext(currentState, rng);
    if (!nextStateOpt) {
      break;
    }
    currentState = *nextStateOpt;
    sequence.push_back(currentState);
  }
  return sequence;
}
std::vector<MarkovChain::State> MarkovChain::States() const {
  return index_to_state_;
}

} // namespace ptm
