#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cstdint>

extern "C" {
#include "../keccak/KeccakHash.h"
}

TEST_CASE("SHAKE128 Known-Answer-Test (empty message)", "[keccak]") {
  std::array<uint8_t, 16> digest;
  std::array<uint8_t, 16> known_answer = {
      0x7f, 0x9c, 0x2b, 0xa4, 0xe8, 0x8f, 0x82, 0x7d,
      0x61, 0x60, 0x45, 0x50, 0x76, 0x05, 0x85, 0x3e,
  };
  Keccak_HashInstance ctx;
  Keccak_HashInitialize_SHAKE128(&ctx);

  Keccak_HashUpdate(&ctx, (const unsigned char *)"", 0);
  Keccak_HashFinal(&ctx, NULL);
  Keccak_HashSqueeze(&ctx, digest.data(), digest.size() * 8);

  REQUIRE(digest == known_answer);
}
TEST_CASE("SHAKE128 Known-Answer-Test (small message)", "[keccak]") {
  std::array<uint8_t, 16> input = {
      0xd4, 0xd6, 0x7b, 0x00, 0xca, 0x51, 0x39, 0x77,
      0x91, 0xb8, 0x12, 0x05, 0xd5, 0x58, 0x2c, 0x0a,
  };
  std::array<uint8_t, 16> digest;
  std::array<uint8_t, 16> known_answer = {
      0xd0, 0xac, 0xfb, 0x2a, 0x14, 0x92, 0x8c, 0xaf,
      0x8c, 0x16, 0x8a, 0xe5, 0x14, 0x92, 0x5e, 0x4e,
  };
  Keccak_HashInstance ctx;
  Keccak_HashInitialize_SHAKE128(&ctx);

  Keccak_HashUpdate(&ctx, input.data(), input.size() * 8);
  Keccak_HashFinal(&ctx, NULL);
  Keccak_HashSqueeze(&ctx, digest.data(), digest.size() * 8);

  REQUIRE(digest == known_answer);
}
