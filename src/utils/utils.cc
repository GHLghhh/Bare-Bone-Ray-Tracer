#include "utils.h"

std::uint64_t MortonCode(std::vector<std::size_t> index)
{
  std::uint64_t res = 0;
  std::size_t shift = 0;
  std::size_t numIndex = index.size();
  for (std::size_t idx = 0; idx < 64; idx++) {
    res = res | (((index[idx % numIndex] & (1 << shift)) >> shift) << idx);
    if (((idx + 1) % numIndex) == 0) {
      shift++;
    }
  }
  return res;
}