#pragma once

#include "Share.h"

class AddShare : public Share {
 private:
  size_t num_shares_;
  
 public:
  // create the class for sharing, mod defines the field Z_p
  AddShare(uint64_t mod);
  virtual ~AddShare() noexcept {};
  AddShare(const AddShare&) = delete;
  AddShare(const AddShare&&) = delete;
  AddShare& operator=(const AddShare&) = delete;

  // Share in additively for num_shares server
  // A share itself is a tuple: <server_index, server_share>
  virtual std::map<uint64_t, uint64_t> share(uint64_t in,
                                             size_t num_shares) override;
  // reconstruct the given shares
  // A share itself is a tuple: <server_index, server_share>
  virtual uint64_t reconstruct(
      const std::map<uint64_t, uint64_t>& shares) override;
  // implement a preprocessing step if necessary
  virtual void preprocess(size_t num_shares) override;
};
