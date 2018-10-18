#include "utils.h"

uint64_t MortonCode(std::vector<size_t> index)
{
  uint64_t res = 0;
  size_t shift = 0;
  size_t numIndex = index.size();
  for (size_t idx = 0; idx < 64; idx++) {
    res = res | (((index[idx % numIndex] & (1 << shift)) >> shift) << idx);
    if (((idx + 1) % numIndex) == 0) {
      shift++;
    }
  }
  return res;
}