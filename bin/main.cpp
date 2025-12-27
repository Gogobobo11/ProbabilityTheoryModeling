#include <fstream>
#include <iostream>
#include <sstream>

#include "../lib/markov-chain/MarkovTextModel.hpp"

int main() {
  using namespace ptm;

  std::ifstream in("tests/war_and_peace.txt");

  std::stringstream buffer;
  buffer << in.rdbuf();
  std::string text = buffer.str();

  MarkovTextModel model(MarkovTextModel::TokenLevel::Word);
  model.TrainFromText(text);

  std::random_device rd;
  std::mt19937 rng(rd());

  std::string generated = model.GenerateText(50, rng, "and");
  std::cout << generated << std::endl;
}
