#pragma once

namespace mystl::algo {

template <typename T>
T const &max(T const &a, T const &b) {
  return a > b ? a : b;
}

template <typename T>
T const &min(T const &a, T const &b) {
  return a < b ? a : b;
}

template <typename Iter_t, typename T>
constexpr void fill(Iter_t begin, Iter_t end, T value) {
  while (begin != end) {
    *begin = value;
    ++begin;
  }
}

template <typename FromIter_t, typename ToIter_t>
constexpr void copy(FromIter_t fromBegin, FromIter_t fromEnd,
                    ToIter_t toBegin) {
  while (fromBegin != fromEnd) {
    *toBegin = *fromBegin;
    ++fromBegin;
    ++toBegin;
  }
}

} // namespace mystl::algo
