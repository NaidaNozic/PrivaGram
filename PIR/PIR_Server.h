#pragma once

#include <string>

#include "NetworkAdapter.h"
#include "SEAL_Server.h"

class PIRServer {
 private:
  // the network adapter used for sending and receiving ciphertexts
  ServerNetworkAdapter network;
  // the class used for all HE operations
  SealServer he;
  // the loaded database of images. Each image is split into blocks of size
  // NUM_BYTES each
  std::vector<std::vector<uint64_t>> database;
  // path to all images
  std::string db_path;

  void read_database(size_t num_images);
  uint64_t getMaxWordsPerImage();

 public:
  PIRServer(uint16_t port, size_t num_images, std::string db_path);
  ~PIRServer() = default;
  PIRServer(const PIRServer&) = delete;
  PIRServer(const PIRServer&&) = delete;
  PIRServer& operator=(const PIRServer&) = delete;

  // respond to encrypted queries from clients
  void respond();
};
