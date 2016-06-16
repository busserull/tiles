#include "utilities.hpp"
#include <cstdlib>

namespace ut{
  int randInclusive(int lower, int upper){
    int num = (rand() % (upper - lower + 1));
    return (num + lower);
  }

  void shuffleArray(int *arr, int length){
    int *i = (arr + length);
    while((i - arr) > 1){
      int index = randInclusive(0, (i - arr - 1));
      int temp = *(i - 1);
      *(i - 1) = *(arr + index);
      *(arr + index) = temp;
      i--;
    }
  }
}
