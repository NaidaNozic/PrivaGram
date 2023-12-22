#include "NetworkAdapter.h"

boost::asio::io_service global_service;

NetworkAdapter::NetworkAdapter(size_t buffer_size)
    : socket_(tcp::socket(global_service)),
      buffer_(std::vector<seal::seal_byte>(buffer_size)){};

ClientNetworkAdapter::ClientNetworkAdapter(
    size_t buffer_size, const boost::asio::ip::address &ip_address,
    uint16_t port)
    : NetworkAdapter(buffer_size) {
  std::cout << "Connecting to Server on port " << port << " ..." << std::endl;
  socket_.connect(tcp::endpoint(ip_address, port));
  boost::asio::ip::tcp::no_delay option(true);
  socket_.set_option(option);
  std::cout << "Connected successfully" << std::endl;
}

ServerNetworkAdapter::ServerNetworkAdapter(size_t buffer_size, uint16_t port)
    : NetworkAdapter(buffer_size) {
  tcp::acceptor acceptor(global_service, tcp::endpoint(tcp::v4(), port));
  std::cout << "Waiting for connections on port " << port << std::endl;
  acceptor.accept(socket_);
  std::cout << "Client connected" << std::endl;
}

void NetworkAdapter::send_ciphertext(const seal::Ciphertext &ciphertext) {
  ciphertext.save(buffer_.data(), buffer_.size());
  boost::system::error_code error;
  boost::asio::write(
      socket_, boost::asio::buffer(buffer_.data(), buffer_.size()), error);
}

seal::Ciphertext NetworkAdapter::receive_ciphertext(
    const seal::SEALContext &context) {
  boost::system::error_code error;
  boost::asio::read(socket_,
                    boost::asio::buffer(buffer_.data(), buffer_.size()), error);
  seal::Ciphertext result;
  result.load(context, buffer_.data(), buffer_.size());
  return std::move(result);
}

void NetworkAdapter::receive_ciphtertext_into(const seal::SEALContext &context,
                                              seal::Ciphertext &output) {
  boost::system::error_code error;
  boost::asio::read(socket_,
                    boost::asio::buffer(buffer_.data(), buffer_.size()), error);
  output.load(context, buffer_.data(), buffer_.size());
}

void NetworkAdapter::shutdown() {
  boost::system::error_code error;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
  socket_.close();
}

NetworkAdapter::~NetworkAdapter() { shutdown(); }
