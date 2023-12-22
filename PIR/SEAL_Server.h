#pragma once

#include "seal/seal.h"

class SealServer {
 private:
  seal::SEALContext context;
  std::unique_ptr<seal::Evaluator> evaluator_;
  std::unique_ptr<seal::BatchEncoder> encoder_;
  seal::GaloisKeys galois_keys;


 public:
  // Construct from EncryptionParameters
  SealServer(seal::EncryptionParameters parms);
  ~SealServer() = default;
  SealServer(const SealServer&) = delete;
  SealServer(const SealServer&&) = delete;
  SealServer& operator=(const SealServer&) = delete;

  // create encryption parameters from the mod_degree and the plaintext modulus
  static seal::EncryptionParameters create_params(size_t mod_degree,
                                                  uint64_t plain_mod);

  // add two ciphertexts
  void add(seal::Ciphertext& c1, seal::Ciphertext& c2, seal::Ciphertext& des);
  // add two ciphertexts, store result in c1_des
  void add_inplace(seal::Ciphertext& c1_des, seal::Ciphertext& c2);

  // multiplay a ciphertext slotwise to a vector of integers
  void multiply_plain(seal::Ciphertext& c1, std::vector<uint64_t>& p2,
                      seal::Ciphertext& des);
  // multiplay a ciphertext slotwise to a vector of integers, store the result
  // in c1_des
  void multiply_plain_inplace(seal::Ciphertext& c1_des,
                              std::vector<uint64_t>& p2);

  void rotate(seal::Ciphertext& c, size_t steps);

  seal::SEALContext get_context() const;
};
