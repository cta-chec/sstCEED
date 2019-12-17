// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "doctest.h"
#include <fstream>

#ifndef SSTCEED_TEST_UTILS_H
#define SSTCEED_TEST_UTILS_H

namespace cta {
namespace sstceed {

void ReadBinary(const std::string& path, uint8_t* buffer, size_t packet_size) {
    std::ifstream file (path, std::ios::in | std::ios::binary);
            REQUIRE(file.is_open());
    file.read(reinterpret_cast<char*>(buffer), packet_size);
}

}  // namespace sstceed
}  // namespace cta

#endif //SSTCEED_TEST_UTILS_H
