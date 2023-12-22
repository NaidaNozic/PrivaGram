#include <boost/program_options.hpp>
#include <cstdint>
#include <filesystem>
#include <iostream>

#include "PIR_Server.h"

int main(int argc, const char *argv[]) {
  unsigned short port;
  size_t num_images;
  std::string database_path;
  boost::program_options::options_description description(
      "Supported arguments");
  description.add_options()("help", "print usage message")(
      "port,p",
      boost::program_options::value<unsigned short>(&port)->required(),
      "the port the Server will listen on")(
      "database,d",
      boost::program_options::value<std::string>(&database_path)
          ->default_value("../database")
          ->notifier([](const std::string &given_path) {
            if (!std::filesystem::is_directory(given_path)) {
              throw boost::program_options::validation_error(
                  boost::program_options::validation_error::
                      invalid_option_value,
                  "database", given_path);
            }
          }),
      "The path to the database")(
      "images,n",
      boost::program_options::value<size_t>(&num_images)->required(),
      "The number of images the server should load");
  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, description), vm);
  if (vm.count("help")) {
    std::cout << description << "\n";
    return 1;
  }
  boost::program_options::notify(vm);
  PIRServer server(port, num_images, database_path);
  server.respond();

  return 0;
}
