#include "Share.h"

#include <stdexcept>

Share::Share(uint64_t modulus) : mod(modulus), mod_bitsize(0), seeded(false) {
  if (modulus <= 1) throw std::invalid_argument("modulus must be at least 2");

  while (modulus) {
    mod_bitsize++;
    modulus >>= 1;
  }
  if (mod_bitsize > 60) throw std::runtime_error("prime must be <= 60 bit");
  mod_bitsize = (1ULL << mod_bitsize) - 1;
};

//----------------------------------------------------------------

void Share::seed_rng(const std::string &seed) {
  if (SUCCESS != Keccak_HashInitialize_SHAKE128(&shake128_))
    throw std::runtime_error("failed to initialize SHAKE128");
  if (SUCCESS != Keccak_HashUpdate(
                     &shake128_, reinterpret_cast<const uint8_t *>(seed.data()),
                     seed.size() * 8))
    throw std::runtime_error("failed to update SHAKE128");
  if (SUCCESS != Keccak_HashFinal(&shake128_, NULL))
    throw std::runtime_error("failed to finalize SHAKE128");
  seeded = true;
}

//----------------------------------------------------------------

uint64_t Share::get_random_field_element() {
  if (!seeded) throw std::runtime_error("SHAKE128 not seeded");

  uint8_t random_bytes[sizeof(uint64_t)];
  while (1) {
    if (SUCCESS !=
        Keccak_HashSqueeze(&shake128_, random_bytes, sizeof(random_bytes) * 8))
      throw std::runtime_error("failed to squeeze SHAKE128");
    uint64_t ele = be64toh(*((uint64_t *)random_bytes)) & mod_bitsize;
    if (ele < mod) return ele;
  }
}

//----------------------------------------------------------------

uint64_t Share::get_mod() const { return mod; }

//----------------------------------------------------------------

uint64_t Share::convert(const uint128_t &in) {
#if defined(__SIZEOF_INT128__)
  return in;
#else
  return in.convert_to<uint64_t>();
#endif
}

//----------------------------------------------------------------

uint64_t Share::field_add(uint64_t a1, uint64_t a2) const {
  return (a1 + a2) % mod;
}

//----------------------------------------------------------------

uint64_t Share::field_sub(uint64_t s1, uint64_t s2) const {
  if (s1 < s2)
    return mod + s1 - s2;
  else
    return s1 - s2;
}

//----------------------------------------------------------------

uint64_t Share::field_mul(uint64_t m1, uint64_t m2) const {
  return convert((uint128_t(m1) * uint128_t(m2)) % mod);
}

//----------------------------------------------------------------

uint64_t Share::field_div(uint64_t d1, uint64_t d2) const {
  uint64_t inv = mod_inverse(d2);
  return field_mul(d1, inv);
}

//----------------------------------------------------------------

uint64_t Share::mod_inverse(uint64_t val) const {
  if (val == 0) throw(std::runtime_error("0 has no inverse!"));

  int64_t prev_a = 1;
  int64_t a = 0;
  uint64_t modulus = mod;

  while (modulus != 0) {
    int64_t q = val / modulus;
    int64_t temp = val % modulus;
    val = modulus;
    modulus = temp;

    temp = a;
    a = prev_a - q * a;
    prev_a = temp;
  }

  if (val != 1) throw(std::runtime_error("value has no inverse!"));

  uint64_t res = prev_a;
  if (prev_a < 0) res += mod;

  return res;
}
