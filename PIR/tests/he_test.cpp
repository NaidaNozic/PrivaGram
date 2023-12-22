#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cstdint>

#include "../SEAL_Client.h"
#include "../SEAL_Server.h"

static constexpr uint64_t plain_mod = 1032193ULL;
static constexpr uint64_t mod_degree = 4096;

TEST_CASE("HE encrypt/decrypt test", "[he]") {
  SealClient client(SealClient::create_params(mod_degree, plain_mod));
  std::vector<uint64_t> vec = {1, 2, 3, 4, 5, 6};
  auto c = client.encrypt(vec);
  auto r = client.decrypt(c);

  vec.resize(client.get_slots());
  REQUIRE(client.get_noise(c) > 0);
  REQUIRE(r == vec);
}

TEST_CASE("HE add test", "[he]") {
  SealClient client(SealClient::create_params(mod_degree, plain_mod));
  SealServer server(SealServer::create_params(mod_degree, plain_mod));
  std::vector<uint64_t> vec1 = {1, 2, 3, 4, 5, 6};
  std::vector<uint64_t> vec2 = {3, 4, 2, 1, 5, 2, 8};
  std::vector<uint64_t> vec3 = {2, 0, 3, 2, 1};

  std::vector<uint64_t> vecr1 = {4, 6, 5, 5, 10, 8, 8};
  std::vector<uint64_t> vecr2 = {3, 2, 6, 6, 6, 6};

  auto c1 = client.encrypt(vec1);
  auto c2 = client.encrypt(vec2);
  auto c3 = client.encrypt(vec3);

  seal::Ciphertext cr1;

  server.add(c1, c2, cr1);
  server.add_inplace(c1, c3);

  auto r1 = client.decrypt(cr1);
  auto r2 = client.decrypt(c1);

  vecr1.resize(client.get_slots());
  vecr2.resize(client.get_slots());
  REQUIRE(client.get_noise(cr1) > 0);
  REQUIRE(client.get_noise(c1) > 0);
  REQUIRE(r1 == vecr1);
  REQUIRE(r2 == vecr2);
}

TEST_CASE("HE multiply plain test", "[he]") {
  SealClient client(SealClient::create_params(mod_degree, plain_mod));
  SealServer server(SealServer::create_params(mod_degree, plain_mod));
  std::vector<uint64_t> vec1 = {1, 2, 3, 4, 5, 6};
  std::vector<uint64_t> vec2 = {3, 4, 2, 1, 5, 2, 8};
  std::vector<uint64_t> vec3 = {2, 0, 3, 2, 1};

  std::vector<uint64_t> vecr1 = {3, 8, 6, 4, 25, 12, 0};
  std::vector<uint64_t> vecr2 = {2, 0, 9, 8, 5, 0};

  auto c1 = client.encrypt(vec1);

  seal::Ciphertext cr1;

  server.multiply_plain(c1, vec2, cr1);
  server.multiply_plain_inplace(c1, vec3);

  auto r1 = client.decrypt(cr1);
  auto r2 = client.decrypt(c1);

  vecr1.resize(client.get_slots());
  vecr2.resize(client.get_slots());
  REQUIRE(client.get_noise(cr1) > 0);
  REQUIRE(client.get_noise(c1) > 0);
  REQUIRE(r1 == vecr1);
  REQUIRE(r2 == vecr2);
}
