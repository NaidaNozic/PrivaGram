
#include "SEAL_Client.h"

#include <iostream>

using namespace seal;

//----------------------------------------------------------------

// hint: initialize keygen, encryptor, encoder.....
SealClient::SealClient(EncryptionParameters parms)
    : context(parms) {

  keygen_ = std::make_unique<KeyGenerator>(context);
  SecretKey secret_key = keygen_->secret_key();
  PublicKey public_key;
  keygen_->create_public_key(public_key);

  encryptor_ = std::make_unique<Encryptor>(context, public_key);
  decryptor_ = std::make_unique<Decryptor>(context, secret_key);
  evaluator_ = std::make_unique<Evaluator>(context);
  encoder_ = std::make_unique<BatchEncoder>(context);
}

//----------------------------------------------------------------

// hint: set set_poly_modulus_degree, set_plain_modulus.... 
seal::EncryptionParameters SealClient::create_params(size_t mod_degree,
                                                     uint64_t plain_mod) {
  EncryptionParameters parms(scheme_type::bfv);
  parms.set_poly_modulus_degree(mod_degree);
  parms.set_plain_modulus(htole64(plain_mod));
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(mod_degree));
  

  std::cout<<"finished setting up parameters for encryption in seal\n";
  return parms;                                                  
}

//----------------------------------------------------------------

uint64_t SealClient::get_slots() const {
  // return the number of plaintexts per ciphertext
  return encoder_->slot_count();
}

//----------------------------------------------------------------

int SealClient::get_noise(seal::Ciphertext& ciph) {
  return decryptor_->invariant_noise_budget(ciph);
}

//----------------------------------------------------------------

seal::Ciphertext SealClient::encrypt(std::vector<uint64_t>& plain) {
  seal::Plaintext plaintext_batch;
  encoder_->encode(plain, plaintext_batch);
  // Encrypt the plaintext matrix into a ciphertext
  seal::Ciphertext ciphertext;
  encryptor_->encrypt(plaintext_batch, ciphertext);

  return ciphertext;
}

//----------------------------------------------------------------

std::vector<uint64_t> SealClient::decrypt(seal::Ciphertext& cipher) {
  // Decrypt the ciphertext into a plaintext
  seal::Plaintext plaintext;
  decryptor_->decrypt(cipher, plaintext);
  // Decode the plaintext into a vector of integers
  std::vector<uint64_t> decrypted_vector;
  encoder_->decode(plaintext, decrypted_vector);

  return decrypted_vector;
}

//----------------------------------------------------------------

seal::SEALContext SealClient::get_context() const { return context; }
