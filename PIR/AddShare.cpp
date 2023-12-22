#include "AddShare.h"
#include <stdexcept>

AddShare::AddShare(uint64_t mod) : Share(mod) {}

//----------------------------------------------------------------

// Hint: generate random shares, adjust the last share, return
std::map<uint64_t, uint64_t> AddShare::share(uint64_t in, size_t num_shares) {
  std::map<uint64_t, uint64_t> shares;
  uint64_t last_share_value = in;

  for (size_t i = 1; i < num_shares; ++i) {
        uint64_t qj = get_random_field_element();
        shares[i] = qj;
        last_share_value = field_sub(last_share_value, qj);
  }
  shares[num_shares] = last_share_value;

  return shares;
}

//----------------------------------------------------------------

// Hint: combine shares
uint64_t AddShare::reconstruct(const std::map<uint64_t, uint64_t>& shares) {
  uint64_t res = 0;

  for (const auto& share : shares) {
      res = field_add(share.second, res);
  }

  return res;
}

//----------------------------------------------------------------

void AddShare::preprocess(size_t num_shares) {
  num_shares_ = num_shares;
};
