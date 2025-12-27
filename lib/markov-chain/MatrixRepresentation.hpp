#pragma once
#include <cstddef>
#include <iostream>
#include <map>
#include <ostream>
#include <vector>

template<class T>
struct MatrixRepresentation {
  void Resize(size_t new_size) {
    values_.resize(new_size);
    row_sums_.resize(new_size);
  }
  void AddAt(size_t i, size_t j, const T& value) {
    row_sums_[i] += value;
    values_[i][j] += value;
  }

  T GetAt(size_t i, size_t j) const {
    if (values_[i].find(j) == values_[i].end()) {
      return T(0);
    }
    return values_[i].at(j);
  }

  std::vector<T> CumulativeRowSum(size_t ind) const {
    std::vector<T> cumulativeRowSum;
    cumulativeRowSum.resize(values_[ind].size() + 1, T(0));
    size_t j = 0;
    for (auto it = values_[ind].begin(); it != values_[ind].end(); ++it) {
      cumulativeRowSum[j + 1] = cumulativeRowSum[j] + it->second;
      j++;
    }
    return cumulativeRowSum;
  }

  std::vector<size_t> GetKeys(size_t row) const {
    std::vector<size_t> keys;
    for (auto it = values_[row].begin(); it != values_[row].end();++it) {
      keys.push_back(it->first);
    }
    return keys;
  }

  T GetRowSum(size_t i) const {
    return row_sums_[i];
  }

private:
  std::vector<std::map<size_t, T>> values_;
  std::vector<T> row_sums_;
};
