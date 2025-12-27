#pragma once

#include <random>
#include <string>

#include "MarkovChain.hpp"

namespace ptm {

class MarkovTextModel {
public:
  enum class TokenLevel { Character, Word }; // NOLINT

  explicit MarkovTextModel(TokenLevel level = TokenLevel::Word);

  // Первичное обучение / дообучение на тексте (одинаково, TrainFromText можно вызывать сколько угодно)
  void TrainFromText(const std::string& text);

  // Генерация текста:
  // - num_tokens: количество токенов (символов или слов в зависимости от уровня)
  // - start_token: опциональный стартовый токен; если не задан или не встречался,
  //   берётся первый известный токен модели
  std::string GenerateText(std::size_t num_tokens, std::mt19937& rng, const std::string& start_token = "") const;

  const MarkovChain& Chain() const noexcept;

private:
  TokenLevel level_;
  MarkovChain chain_;

  std::vector<std::string> Tokenize(const std::string& text) const;
  std::string Detokenize(const std::vector<std::string>& tokens) const;
};

} // namespace ptm
