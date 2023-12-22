cmake_minimum_required(VERSION 3.11)
project(Keccak C)

set(KECCAK_SRCS
  KeccakSpongeWidth1600.c
  KeccakHash.c
  KeccakP-1600-opt64.c
)

add_library(keccak STATIC ${KECCAK_SRCS})
target_include_directories(keccak PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

