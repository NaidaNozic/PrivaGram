#pragma once

#include <cstdint>
#include <map>
#include <string>

#if defined(__SIZEOF_INT128__)
typedef __uint128_t uint128_t;
#else
#include <boost/multiprecision/cpp_int.hpp>
typedef boost::multiprecision::uint128_t uint128_t;
#endif

extern "C" {
#include "keccak/KeccakHash.h"
}

class Share {
 private:
  Keccak_HashInstance shake128_;
  uint64_t mod_bitsize;
  bool seeded;

 protected:
  uint64_t mod;

  // Field operations
  uint64_t get_random_field_element();  // Use for creating random shares!
  uint64_t mod_inverse(uint64_t val) const;
  static uint64_t convert(const uint128_t& in);

 public:
  uint64_t field_add(uint64_t a1, uint64_t a2) const;
  uint64_t field_sub(uint64_t s1, uint64_t s2) const;
  uint64_t field_mul(uint64_t m1, uint64_t m2) const;
  uint64_t field_div(uint64_t d1, uint64_t d2) const;

 public:
  Share(uint64_t mod);
  virtual ~Share() noexcept {};
  Share(const Share&) = delete;
  Share(const Share&&) = delete;
  Share& operator=(const Share&) = delete;

  uint64_t get_mod() const;

  void seed_rng(const std::string& seed);
  virtual std::map<uint64_t, uint64_t> share(uint64_t in,
                                             size_t num_shares) = 0;
  virtual uint64_t reconstruct(const std::map<uint64_t, uint64_t>& shares) = 0;
  virtual void preprocess(size_t num_shares) = 0;
};
