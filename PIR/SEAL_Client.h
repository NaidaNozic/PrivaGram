#pragma once

#include "seal/seal.h"

class SealClient {
 private:
  seal::SEALContext context;
  std::unique_ptr<seal::Encryptor> encryptor_;
  std::unique_ptr<seal::Decryptor> decryptor_;
  std::unique_ptr<seal::Evaluator> evaluator_;
  std::unique_ptr<seal::KeyGenerator> keygen_;
  std::unique_ptr<seal::BatchEncoder> encoder_;

 public:
  // Construct from EncryptionParameters
  SealClient(seal::EncryptionParameters parms);
  ~SealClient() = default;
  SealClient(const SealClient&) = delete;
  SealClient(const SealClient&&) = delete;
  SealClient& operator=(const SealClient&) = delete;

  // create encryption parameters from the mod_degree and the plaintext modulus
  static seal::EncryptionParameters create_params(size_t mod_degree,
                                                  uint64_t plain_mod);

  // return the available noise budget of a ciphertext
  int get_noise(seal::Ciphertext&);

  // Encrypt a vector of integers into one ciphertext
  seal::Ciphertext encrypt(std::vector<uint64_t>& plain);
  // decrypt a ciphertext into a vector of integers
  std::vector<uint64_t> decrypt(seal::Ciphertext& cipher);

  // get the number of integers which fit into one ciphertext
  uint64_t get_slots() const;

  seal::SEALContext get_context() const;
};
