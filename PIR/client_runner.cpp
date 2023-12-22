#include <algorithm>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "PIR_Client.h"

int main(int argc, const char *argv[]) {
  std::string seed;
  size_t index, num_images;
  std::string out_file;
  std::vector<std::string> servers;
  std::string configfile;
  bool use_shamir;
  size_t threshold;
  boost::program_options::options_description description(
      "Supported arguments");
  description.add_options()("help", "print usage message")(
      "index,i", boost::program_options::value<size_t>(&index)->required(),
      "The index of the image that should be retreived")(
      "output,o",
      boost::program_options::value<std::string>(&out_file)->default_value(
          "result.jpg"),
      "The filename to store the retreived image")

      ("seed,s",
       boost::program_options::value<std::string>(&seed)
           ->default_value("abcdefgh")
           ->notifier([](const std::string &given_seed) {
             if (given_seed.size() < 8) {
               throw boost::program_options::validation_error(
                   boost::program_options::validation_error::
                       invalid_option_value,
                   "seed", given_seed);
             }
           }),
       "the seed for random value generation")(
          "images,n",
          boost::program_options::value<size_t>(&num_images)->required(),
          "The total number of available images")(
          "servers",
          boost::program_options::value<std::vector<std::string>>(&servers)
              ->multitoken(),
          "The list of servers, expected format is <HOSTNAME>:<PORT>")(
          "shamir",
          boost::program_options::value<bool>(&use_shamir)->default_value(true),
          "Use shamir secret sharing during the protocol")(
          "privacy,t",
          boost::program_options::value<size_t>(&threshold)->default_value(0),
          "Protection against t colluding servers in shamir sharing")(
          "configfile,c",
          boost::program_options::value<std::string>(&configfile),
          "Path to a server config file containing one server per line in the "
          "format expected by the servers argument");

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, description), vm);
  if (vm.count("help")) {
    std::cout << description << "\n";
    return 1;
  }
  boost::program_options::notify(vm);
  if (vm.count("configfile")) {
    std::ifstream config_file(configfile, std::ifstream::in);
    std::string line;
    while (std::getline(config_file, line)) {
      // Check for multiple entries of the same server string
      if (std::find(servers.begin(), servers.end(), line) == servers.end()) {
        servers.push_back(line);
      } else {
        throw std::invalid_argument(
            "Server " + line +
            " defined in command line arguments and config file.");
      }
    }
  }
  if (servers.size() == 0) {
    throw std::invalid_argument(
        "No server addresses defined, either provide the servers argument or a "
        "config file containg the server information");
  }

  std::vector<std::pair<const boost::asio::ip::address, uint16_t>> sockets;
  for (const auto &server : servers) {
    boost::asio::ip::address address = boost::asio::ip::address::from_string(
        server.substr(0, server.find(":")));
    uint16_t port =
        std::stoi(server.substr(server.find(":") + 1, server.size()));
    sockets.push_back(std::make_pair(address, port));
  }
  PIRClient client(seed, sockets, sockets.size(), use_shamir, threshold);
  client.fetch_image(index, num_images, out_file);

  return 0;
}
