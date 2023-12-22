#pragma once

#include <string>

#include "NetworkAdapter.h"
#include "SEAL_Client.h"
#include "Share.h"

class PIRClient {
 private:
  // defines how many colluding servers are mitigated by the protocol
  size_t privacy;
  // total number of servers
  size_t num_server;
  // the network adapters used for sending and receiving ciphertexts, one for
  // each server
  std::vector<std::unique_ptr<ClientNetworkAdapter>> network;
  // the class used for sharing. Either AddShare or ShamirShare
  std::unique_ptr<Share> share;
  // the class used for all HE operations
  SealClient he;

  bool use_shamir;

 public:
  PIRClient(
      std::string seed,
      std::vector<std::pair<const boost::asio::ip::address, uint16_t>>& sockets,
      size_t num_server, bool use_shamir, size_t privacy = 1);
  ~PIRClient() = default;
  PIRClient(const PIRClient&) = delete;
  PIRClient(const PIRClient&&) = delete;
  PIRClient& operator=(const PIRClient&) = delete;

  // use PIR with HE to privately fetch a image
  void fetch_image(size_t index, size_t num_images, std::string& out_filename);
};
