#pragma once

#include <stdint.h>
#include <string>

namespace params {
 // TODO SET PARAMETERS!
  // defines the field Z_p for secret sharing and homomorphic encryption
  static constexpr uint64_t PLAINTEXT_MOD = 65537ULL;
  // defines the mod degree for homomorphic encryption
  static constexpr uint64_t MOD_DEGREE = 16384;
  // defines the size of each block the images are split into
  static constexpr size_t NUM_BYTES = 2;
} // namespace params
