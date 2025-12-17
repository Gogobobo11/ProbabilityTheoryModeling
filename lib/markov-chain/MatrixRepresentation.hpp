#pragma once
#include <cstddef>
#include <ostream>
#include <vector>


template <class T>
struct MatrixRepresentation {
  void OnAddition() {
    size_t cur_size = values_.size();
    values_.resize(cur_size + 1);
    for (size_t i = 0; i <= cur_size; ++i) {
      values_[i].resize(cur_size + 1);
    }
    row_sums_.resize(cur_size + 1);
  }
  void AddAt(size_t i, size_t j, const T& value) {
    row_sums_[i] += value;
    values_[i][j] += value;
  }

  T GetAt(size_t i, size_t j) const {
    return values_[i][j];
  }

  std::vector<T> CumulativeRowSum(size_t ind) const {
    std::vector<T> cumulativeRowSum;
    cumulativeRowSum.resize(values_[ind].size() + 1, T(0));
    for (size_t j = 0; j < values_[ind].size(); ++j) {
      cumulativeRowSum[j + 1] = cumulativeRowSum[j] + values_[ind][j];
    }
    return cumulativeRowSum;
  }

  T GetRowSum(size_t i) const {
    return row_sums_[i];
  }
private:
  std::vector<std::vector<T>> values_;
  std::vector<T> row_sums_;
};