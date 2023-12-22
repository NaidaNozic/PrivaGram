#define CATCH_CONFIG_MAIN
#include "../ShamirShare.h"

#include <catch2/catch.hpp>
#include <cstdint>

static constexpr uint64_t PRIME = 8088322049ULL;
static constexpr uint64_t NUM_PARTIES = 10;
static const std::string SEED = "SHARE_SEED";
static constexpr uint64_t DEGREE = 5;

TEST_CASE("Shamir Share test (33 bit)", "[shamirshare]") {
  static constexpr uint64_t value = 1234567890;

  ShamirShare share(PRIME);
  share.seed_rng(SEED);
  share.set_degree(DEGREE);
  share.preprocess(NUM_PARTIES);

  auto shares = share.share(value, NUM_PARTIES);

  REQUIRE(shares.size() == NUM_PARTIES);
  REQUIRE(share.reconstruct(shares) == value);

  for (size_t i = NUM_PARTIES; i > DEGREE + 1; i--) {
    shares.erase(shares.begin());
    REQUIRE(share.reconstruct(shares) == value);
  }

  shares.erase(shares.begin());
  try {
    uint64_t v = share.reconstruct(shares);
    REQUIRE(false);
  } catch (const std::runtime_error &exc) {
    REQUIRE(true);
  } catch (...) {
    REQUIRE(false);
  }
}

TEST_CASE("Shamir Share test (33 bit) -- add", "[shamirshare]") {
  static constexpr uint64_t value1 = 1234567890;
  static constexpr uint64_t value2 = 8028422012ULL;

  ShamirShare share(PRIME);
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

TEST_CASE("Shamir Share test (33 bit) -- multiply plain", "[shamirshare]") {
  static constexpr uint64_t value1 = 1234567890;
  static constexpr uint64_t value2 = 8028422012ULL;

  ShamirShare share(PRIME);
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
