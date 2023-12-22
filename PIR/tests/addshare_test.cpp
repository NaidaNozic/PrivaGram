#define CATCH_CONFIG_MAIN
#include "../AddShare.h"

#include <catch2/catch.hpp>
#include <cstdint>

static constexpr uint64_t PRIME = 8088322049ULL;
static constexpr uint64_t NUM_PARTIES = 10;
static const std::string SEED = "SHARE_SEED";

TEST_CASE("Additive Share test (33 bit)", "[addshare]") {
  static constexpr uint64_t value = 1234567890;

  AddShare share(PRIME);
  share.seed_rng(SEED);
  share.preprocess(NUM_PARTIES);
  auto shares = share.share(value, NUM_PARTIES);

  REQUIRE(shares.size() == NUM_PARTIES);
  REQUIRE(share.reconstruct(shares) == value);
  shares.erase(shares.begin());
  REQUIRE(share.reconstruct(shares) != value);
}

TEST_CASE("Additive Share test (33 bit) -- add", "[addshare]") {
  static constexpr uint64_t value1 = 1234567890;
  static constexpr uint64_t value2 = 8028422012ULL;

  AddShare share(PRIME);
  share.seed_rng(SEED);
  share.preprocess(NUM_PARTIES);
  auto shares1 = share.share(value1, NUM_PARTIES);
  auto shares2 = share.share(value2, NUM_PARTIES);

  auto shares_r = shares1;
  for (auto &it : shares_r) {
    auto s = shares2.find(it.first);
    REQUIRE(s != shares2.end());
    it.second = share.field_add(it.second, s->second);
  }

  REQUIRE(shares1.size() == NUM_PARTIES);
  REQUIRE(shares2.size() == NUM_PARTIES);
  REQUIRE(shares_r.size() == NUM_PARTIES);
  REQUIRE(share.reconstruct(shares_r) == share.field_add(value1, value2));
}

TEST_CASE("Additive Share test (33 bit) -- multiply plain", "[addshare]") {
  static constexpr uint64_t value1 = 1234567890;
  static constexpr uint64_t value2 = 8028422012ULL;

  AddShare share(PRIME);
  share.seed_rng(SEED);
  share.preprocess(NUM_PARTIES);
  auto shares = share.share(value1, NUM_PARTIES);

  auto shares_r = shares;
  for (auto &it : shares_r) {
    it.second = share.field_mul(it.second, value2);
  }

  REQUIRE(shares.size() == NUM_PARTIES);
  REQUIRE(shares_r.size() == NUM_PARTIES);
  REQUIRE(share.reconstruct(shares_r) == share.field_mul(value1, value2));
}
