#include "MySTL/Vector.h"
#include <iostream>

void test_array();

int main() {
  auto vs = mystl::Vector<float>{1, 2, 3, 4, 5, 6};
  for (auto v : vs) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
  std::cout << "run test \n";
  test_array();
}
