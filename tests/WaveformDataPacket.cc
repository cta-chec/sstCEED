// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstCEED/WaveformDataPacket.h"
#include "doctest.h"
#include <fstream>

namespace cta {
namespace sstceed {


void ReadBinary(const std::string& path, uint8_t* buffer, size_t packet_size) {
    std::ifstream file (path, std::ios::in | std::ios::binary);
    REQUIRE(file.is_open());
    file.read(reinterpret_cast<char*>(buffer), packet_size);
}


TEST_CASE("Read WaveformDataPacket from binary") {
    std::string path = "../share/sstCEED/waveform_data_packet_example.bin";
    size_t packet_size = 8276;

    SUBCASE("Generic uint8_t* data_packet") {
        auto* data_packet = new uint8_t[packet_size]();
        REQUIRE(data_packet[0] == 0);
        ReadBinary(path, data_packet, packet_size);
        CHECK(data_packet[0] == 32);
    }

    SUBCASE("WaveformDataPacket") {
        WaveformDataPacket data_packet(packet_size);
        CHECK(data_packet.GetPacketSize() == packet_size);
        REQUIRE(data_packet.GetDataPacket()[0] == 0);
        ReadBinary(path, data_packet.GetDataPacket(), packet_size);
        CHECK(data_packet.GetDataPacket()[0] == 32);
    }
}

}  // namespace sstceed
}  // namespace cta
