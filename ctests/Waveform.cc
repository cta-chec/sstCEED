// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstCEED/WaveformDataPacket.h"
#include "sstCEED/Waveform.h"
#include "doctest.h"
#include "test_utils.h"


namespace cta {
namespace sstceed {

TEST_CASE("Waveform") {
    std::string path = "../share/sstCEED/waveform_data_packet_example.bin";
    size_t packet_size = 8276;
    WaveformDataPacket data_packet(packet_size);
    ReadBinary(path, data_packet.GetDataPacket(), packet_size);

    SUBCASE("Associate") {
        Waveform waveform{};
        CHECK(!waveform.IsAssociated());
        waveform.Associate(data_packet.GetWaveformPointer(0));
        CHECK(waveform.IsAssociated());
    }

    SUBCASE("Getters") {
        Waveform waveform{};
        waveform.Associate(data_packet.GetWaveformPointer(20));
        CHECK(!waveform.IsErrorFlagOn());
        CHECK(waveform.GetChannelID() == 4);
        CHECK(waveform.GetASICID() == 1);
        CHECK(waveform.GetNSamples() == 128);
        CHECK(!waveform.IsZeroSuppressed());
        CHECK(waveform.GetSample12bit(0) == 584);
        CHECK(waveform.GetSample12bit(10) == 582);
        CHECK(waveform.GetSample16bit(0) == 584);
        CHECK(waveform.GetSample16bit(10) == 8774);
        CHECK(waveform.GetPixelID() == 20);
    }

    SUBCASE("Setters") {
        Waveform waveform{};
        waveform.Associate(data_packet.GetWaveformPointer(20));
        REQUIRE(waveform.GetSample12bit(0) != 4000);
        waveform.SetSample12bit(0, 4000);
        REQUIRE(waveform.GetSample12bit(0) == 4000);
        REQUIRE(waveform.GetSample16bit(0) == 4000);

        REQUIRE(waveform.GetSample16bit(0) != 65000);
        waveform.SetSample16bit(0, 65000);
        REQUIRE(waveform.GetSample12bit(0) != 65000); // Overflow
        REQUIRE(waveform.GetSample16bit(0) == 65000);
    }
}

}  // namespace sstceed
}  // namespace cta
