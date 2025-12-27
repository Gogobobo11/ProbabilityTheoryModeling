#pragma once
#include <functional>
#include <string>
#include <vector>

std::string filtered(std::string_view text, std::function<bool(char)> is_good) {
  std::string result{};
  for (char c : text) {
    if (is_good(c)) {
      result += c;
    }
  }
  return result;
}

std::vector<std::string> SplitWithFilter(const std::string& str, std::string delimiters, std::function<bool(char)> is_good) {
  std::vector<std::string> tokens;
  size_t start = 0;
  size_t end = str.find_first_of(delimiters);

  std::string_view view = str;
  while (end != std::string::npos) {
    std::string_view token = view.substr(start, end - start);

    tokens.push_back(filtered(token, is_good));
    start = end + 1;
    end = str.find_first_of(delimiters, start);
  }

  tokens.push_back(str.substr(start));
  return tokens;
}
