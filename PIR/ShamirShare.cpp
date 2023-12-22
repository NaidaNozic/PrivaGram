#include "ShamirShare.h"
#include <stdexcept>
#include <cmath>

ShamirShare::ShamirShare(uint64_t mod) : Share(mod), degree(0) {}

//----------------------------------------------------------------

void ShamirShare::set_degree(size_t deg) { degree = deg; }

//----------------------------------------------------------------

uint64_t ShamirShare::evaluate_polynomial(uint64_t x, std::vector<uint64_t> coeff){
  uint64_t polResult = 0;
  for(int i = 0; i <= degree; i++)
  {
    polResult = field_add(polResult, field_mul(coeff.at(i), std::pow(x, i)));
  }
  return polResult;
}

// Hint: get random Shamir poly and generate shares
std::map<uint64_t, uint64_t> ShamirShare::share(uint64_t in,
                                                size_t num_shares) {
  std::map<uint64_t, uint64_t> shares;
  std::vector<uint64_t> coeff(degree + 1);
  coeff.at(0) = in;

  // Generate random coefficients
  for (int j=1; j <= degree; j++){
    coeff.at(j) = get_random_field_element();
  }

  //Generate shares
  for (uint64_t x=1; x <= num_shares; x++){
    uint64_t y = evaluate_polynomial(x, coeff);
    shares[x] = y;
  }
  return shares;
}

//----------------------------------------------------------------

std::vector<uint64_t> ShamirShare::get_lagrange_const(const std::map<uint64_t, uint64_t>& shares){

  std::vector<uint64_t> constants (shares.size());
  int i = 1;
  int j = 1;
  for(const auto& share_i : shares){
    uint64_t num = 1;
    uint64_t d = 1;
    for(const auto& share_j : shares){
      if ( i != j ){
        num = field_mul(num, share_j.first);
        d = field_mul(d, field_sub(share_j.first, share_i.first));
      }
      j++;
    }

    constants.at(i - 1) = field_div(num, d);
    i++;
    j=1;
  }
  return constants;
}

uint64_t ShamirShare::reconstruct(const std::map<uint64_t, uint64_t>& shares) {
  if (shares.size() < degree + 1) throw std::runtime_error("Insufficient number of shares for secret reconstruction");

  uint64_t secret = 0;
  std::vector<uint64_t> constants = get_lagrange_const(shares);
  int i = 1;
  for(const auto& share : shares){
    secret = field_add(secret, field_mul(constants.at(i - 1), share.second));
    i++;
  }
  return secret;
}

// Hint: implement random Shamir poly, get random Shamir indices, evaluate point,
//        lagrange coefs function...

//----------------------------------------------------------------

void ShamirShare::preprocess(size_t num_shares) {
  num_shares_ = num_shares;
};
