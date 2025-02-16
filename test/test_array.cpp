#include "MySTL/Array.h"

#include <type_traits>

using Array10 = mystl::Array<int, 10>;

void pass_as_ref(Array10 &arr) {
  static_assert(std::is_same_v<decltype(arr[0]), int &>);
  static_assert(std::is_same_v<decltype(arr.front()), int &>);
  static_assert(std::is_same_v<decltype(arr.back()), int &>);
  static_assert(std::is_same_v<decltype(arr.data()), int(&)[10]>);

  assert(arr.front() == 1);
  assert(arr.back() == 10);
}

void pass_as_const_ref(Array10 const &arr) {
  static_assert(std::is_same_v<decltype(arr[0]), int const &>);
  static_assert(std::is_same_v<decltype(arr.front()), int const &>);
  static_assert(std::is_same_v<decltype(arr.back()), int const &>);
  static_assert(std::is_same_v<decltype(arr.data()), int const(&)[10]>);

  assert(arr.front() == 1);
  assert(arr.back() == 10);
}

void pass_as_rvalue_ref(Array10 &&arr) {
  // static_assert(std::is_same_v<decltype(arr[0]), int &&>);
  static_assert(std::is_same_v<decltype(arr[0]), int &>);
}

void test_array() {
  auto arr = mystl::Array<int, 10>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (auto v : arr) {
    (void)v;
    //
  }

  assert(arr.size() == 10);
  assert(arr[0] == 1);
  assert(arr[1] == 2);
  assert(arr[2] == 3);
  assert(arr[3] == 4);
  assert(arr[4] == 5);
  assert(arr[5] == 6);
  assert(arr[6] == 7);
  assert(arr[7] == 8);
  assert(arr[8] == 9);
  assert(arr[9] == 10);
}
