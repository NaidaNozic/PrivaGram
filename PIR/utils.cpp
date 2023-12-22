#include "utils.h"

#include <math.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

std::vector<uint64_t> utils::read_image(const std::string &filename,
                                        size_t num_bytes) {
  if (num_bytes > 8) {
    throw std::invalid_argument("Cannot store more than 8 bytes in an uint64");
  }
  std::ifstream input(filename, std::ios::binary);

  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
  std::vector<uint64_t> values;
  for (std::vector<unsigned char>::const_iterator it = buffer.begin();
       it <= buffer.end(); it += num_bytes) {
    uint64_t value = 0;
    unsigned char *v_begin = reinterpret_cast<unsigned char *>(&value);
    memcpy(v_begin, &(*it),
           std::min(buffer.end() - it, static_cast<long>(num_bytes)));
    values.push_back(htole64(value));
  }
  return values;
}

void utils::write_image(const std::string &filename,
                        const std::vector<uint64_t> &values, size_t num_bytes) {
  if (num_bytes > 8) {
    throw std::invalid_argument(
        "Cannot extract more than 8 bytes from an uint64");
  }
  std::vector<unsigned char> bytes;
  bytes.resize(values.size() * num_bytes, 0);
  size_t offset = 0;
  for (const auto &v : values) {
    const unsigned char *v_begin = reinterpret_cast<const unsigned char *>(&v);
    memcpy(bytes.data() + offset, v_begin, num_bytes);
    offset += num_bytes;
  }
  // Remove trailing zeros
  while (bytes.back() == 0) {
    bytes.pop_back();
  }
  std::ofstream out(filename, std::ios::out | std::ios::binary);
  out.write(reinterpret_cast<char *>(bytes.data()), bytes.size());
}

// required for setting the buffer for networking
uint64_t utils::get_ciphertext_size(uint64_t mod_degree) {
  switch (mod_degree) {
    case 4096:
      return 131689;
    case 8192:
      return 526441;
    case 16384:
      return 2105449;
    default:
      return 2105449;
  }
}
