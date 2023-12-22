#pragma once

#include <boost/asio.hpp>

#include "seal/seal.h"

using namespace boost::asio;
using namespace ip;

class NetworkAdapter {
 protected:
  tcp::socket socket_;
  std::vector<seal::seal_byte> buffer_;

 public:
  // Pass the size determined by the encryption parameter
  NetworkAdapter(size_t buffer_size);
  NetworkAdapter(const NetworkAdapter &other) = delete;
  NetworkAdapter(NetworkAdapter &&other) = default;
  ~NetworkAdapter();
  void send_ciphertext(const seal::Ciphertext &ciphertext);
  void receive_ciphtertext_into(const seal::SEALContext &context,
                                seal::Ciphertext &output);
  seal::Ciphertext receive_ciphertext(const seal::SEALContext &context);
  void shutdown();
};

// Connects to a given address & port
class ClientNetworkAdapter : public NetworkAdapter {
 public:
  ClientNetworkAdapter(size_t buffer_size,
                       const boost::asio::ip::address &ip_address,
                       uint16_t port);
};

// Listens for Clients on a given port
class ServerNetworkAdapter : public NetworkAdapter {
 public:
  ServerNetworkAdapter(size_t buffer_size, uint16_t port);
};
