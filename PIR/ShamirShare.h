#pragma once

#include <vector>

#include "Share.h"

class ShamirShare : public Share {
 private:
  size_t degree;  // degree of the polynomials
  size_t num_shares_;


 public:
  // create the class for sharing, mod defines the field Z_p
  ShamirShare(uint64_t mod);
  virtual ~ShamirShare() noexcept {};
  ShamirShare(const ShamirShare&) = delete;
  ShamirShare(const ShamirShare&&) = delete;
  ShamirShare& operator=(const ShamirShare&) = delete;

  // set the degree of the polynomial
  void set_degree(size_t deg);
  std::vector<uint64_t> get_lagrange_const(const std::map<uint64_t, uint64_t>& shares);
  uint64_t evaluate_polynomial(uint64_t x, std::vector<uint64_t> coeff);

  // Share in for num_shares server using shamir secret sharing
  // A share itself is a tuple: <index, server_share>, where index is a random
  // value in Z_p and server_share the polynomial evaluated at index
  virtual std::map<uint64_t, uint64_t> share(uint64_t in,
                                             size_t num_shares) override;
  // reconstruct the given shares
  // A share itself is a tuple: <index, server_share>, where index is a random
  // value in Z_p and server_share the polynomial evaluated at index
  virtual uint64_t reconstruct(
      const std::map<uint64_t, uint64_t>& shares) override;
  // implement a preprocessing step if necessary
  virtual void preprocess(size_t num_shares) override;
};
