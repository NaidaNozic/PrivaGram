#pragma once
#include <string>
#include <vector>

namespace utils {
// Reads the given file into a vector of uint64_t values, where each uint64_t
// holds a maximum value that can be stored in num_bytes
std::vector<uint64_t> read_image(const std::string &filename, size_t num_bytes);

// Interprets each of the elements in the given vector of uint64_t values as
// num_bytes bytes and writes the resulting byte vector to the given file name
void write_image(const std::string &filename,
                 const std::vector<uint64_t> &values, size_t num_bytes);

// required for setting the buffer for networking
uint64_t get_ciphertext_size(uint64_t mod_degree);

}  // namespace utils
