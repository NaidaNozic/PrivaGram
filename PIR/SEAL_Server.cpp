#include "SEAL_Server.h"

#include <iostream>

using namespace seal;

//----------------------------------------------------------------

// hint: initialize evaluator, encoder.....
SealServer::SealServer(seal::EncryptionParameters parms)
    : context(parms) {
  evaluator_ = std::make_unique<Evaluator>(context);
  encoder_ = std::make_unique<BatchEncoder>(context);

  KeyGenerator keygen(context);
  keygen.create_galois_keys(galois_keys);

}

//----------------------------------------------------------------

// hint: set set_poly_modulus_degree, set_plain_modulus.... 
seal::EncryptionParameters SealServer::create_params(size_t mod_degree,
                                                     uint64_t plain_mod) {
  EncryptionParameters parms(scheme_type::bfv);
  parms.set_poly_modulus_degree(mod_degree);
  parms.set_plain_modulus(htole64(plain_mod));
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(mod_degree));
  std::cout<<"finished setting up parameters for encryption in seal server\n";
  return parms;        
}

//----------------------------------------------------------------

void SealServer::add(seal::Ciphertext& c1, seal::Ciphertext& c2,
                     seal::Ciphertext& des) {
  evaluator_->add(c1, c2, des);
}

//----------------------------------------------------------------

void SealServer::add_inplace(seal::Ciphertext& c1_des, seal::Ciphertext& c2) {
  evaluator_->add_inplace(c1_des, c2);
}

//----------------------------------------------------------------

void SealServer::multiply_plain(seal::Ciphertext& c1, std::vector<uint64_t>& p2,
                                seal::Ciphertext& des) {
  Plaintext plain;
  encoder_->encode(p2, plain);
  evaluator_->multiply_plain(c1, plain, des);
}

//----------------------------------------------------------------

void SealServer::multiply_plain_inplace(seal::Ciphertext& c1_des,
                                        std::vector<uint64_t>& p2) {
  Plaintext plain;
  encoder_->encode(p2, plain);
  evaluator_->multiply_plain_inplace(c1_des, plain);
}

//----------------------------------------------------------------

seal::SEALContext SealServer::get_context() const { return context; }

void SealServer::rotate(seal::Ciphertext& c, size_t steps)
{
  evaluator_->rotate_rows_inplace(c, steps, galois_keys);
}
