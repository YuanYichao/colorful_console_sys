#include <iostream>
#include <utility>
#include "Darray.hpp"
#include "winsock.h"

class a {
 public:
  using A = int;
};
int main() {
  ccs::Darray<int, 3, 3, 3> arr = {1, 2,  3,  4,  5,  6,  7, 8,
                                   9, 10, 11, 12, 13, 14, 15};
  ccs::Darray<int, 3, 3, 3> arr2 = {1, 2,  3,  4,  5,  6,  7,  8,
                                    9, 10, 11, 12, 13, 14, 15, 16};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        std::cout << arr.at(k, j, i) << std::endl;
      }
    }
  }
  for (int i = 0; i < 15; i++) std::cout << arr[i] << std::endl;
  swap(arr, arr2);
  arr = arr2;
  arr = arr;
  auto t = arr.sbegin();
  auto tt = arr.send();
  auto ttt = arr.csend();
  auto tttt = arr.csbegin();
  return 0;
}