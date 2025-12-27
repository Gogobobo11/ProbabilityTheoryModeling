#include <ranges>
#include "MarkovTextModel.hpp"
#include "utility/SplitText.hpp"

namespace ptm {

MarkovTextModel::MarkovTextModel(TokenLevel level) : level_(level) {
}
void MarkovTextModel::TrainFromText(const std::string& text) {
  chain_.Train(Tokenize(text));
}
std::string MarkovTextModel::GenerateText(std::size_t num_tokens,
                                          std::mt19937& rng,
                                          const std::string& start_token) const {
  auto tokens = chain_.Generate(start_token, num_tokens, rng);
  return Detokenize(tokens);
}
const MarkovChain& MarkovTextModel::Chain() const noexcept {
  return chain_;
}
std::vector<std::string> MarkovTextModel::Tokenize(const std::string& text) const {
  switch (level_) {
    case TokenLevel::Character: {
      auto view = text | std::views::transform([](char c) { return std::string(1, c); });
      return std::vector<std::string>{view.begin(), view.end()};
    }
    case TokenLevel::Word: {
      return split(text, ' ');
    }
    default:
      throw std::invalid_argument("Invalid token level");
  }
}
std::string MarkovTextModel::Detokenize(const std::vector<std::string>& tokens) const {
  std::string ans;
  for (auto el : tokens) {
    ans += el;
    ans += ' ';
  }
  return ans;
}

} // namespace ptm

