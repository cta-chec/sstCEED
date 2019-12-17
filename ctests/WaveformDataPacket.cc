// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstCEED/WaveformDataPacket.h"
#include "doctest.h"
#include "test_utils.h"

namespace cta {
namespace sstceed {

TEST_CASE("WaveformDataPacket") {
    std::string path = "../share/sstCEED/waveform_data_packet_example.bin";
    size_t packet_size = 8276;

    SUBCASE("Generic uint8_t* data_packet") {
        auto* data_packet = new uint8_t[packet_size]();
        REQUIRE(data_packet[0] == 0);
        ReadBinary(path, data_packet, packet_size);
        CHECK(data_packet[0] == 32);
    }

    SUBCASE("Constructor") {
        WaveformDataPacket data_packet(packet_size);
        CHECK(data_packet.GetPacketSize() == packet_size);
        REQUIRE(data_packet.GetDataPacket()[0] == 0);
        ReadBinary(path, data_packet.GetDataPacket(), packet_size);
        CHECK(data_packet.GetDataPacket()[0] == 32);
    }

    SUBCASE("Getters") {
        WaveformDataPacket data_packet(packet_size);
        ReadBinary(path, data_packet.GetDataPacket(), packet_size);
        CHECK(data_packet.GetNWaveforms() == 32);
        CHECK(data_packet.GetRegisterSetup() == 0);
        CHECK(!data_packet.IsLastSubPacket());
        CHECK(data_packet.IsFirstSubPacket());
        CHECK(data_packet.GetNBuffers() == 8);
        CHECK(data_packet.GetTACKTime() == 2165717354592);
        CHECK(data_packet.GetEventNumber() == 18528);
        CHECK(data_packet.GetSlotID() == 22);
        CHECK(data_packet.GetDetectorID() == 22);
        CHECK(data_packet.GetEventSequenceNumber() == 236);
        CHECK(data_packet.GetDetectorUniqueTag() == 160);
        CHECK(data_packet.GetColumn() == 5);
        CHECK(data_packet.GetStaleBit() == 0);
        CHECK(data_packet.IsZeroSupressionEnabled() == 0);
        CHECK(data_packet.GetRow() == 5);
        CHECK(data_packet.GetBlockPhase() == 8);
        CHECK(data_packet.GetCRC() == 38078);
        CHECK(data_packet.GetMBZ() == 0);
        CHECK(!data_packet.IsTimeout());
        CHECK(!data_packet.IsError());

        CHECK(data_packet.GetWaveformSamplesNBlocks() == 4);
        CHECK(data_packet.GetWaveformSamplesNBytes() == 256);
        CHECK(data_packet.GetWaveformNSamples() == 128);
        CHECK(data_packet.GetWaveformNBytes() == 258);
        CHECK(data_packet.GetPacketNBytes() == 8276);
    }

    SUBCASE("Utility") {
        WaveformDataPacket data_packet(packet_size);
        ReadBinary(path, data_packet.GetDataPacket(), packet_size);
        CHECK(WaveformDataPacket::CalculatePacketSizeBytes(
                data_packet.GetWaveformNSamples(), data_packet.GetNWaveforms()
        ) == data_packet.GetPacketNBytes());

        uint16_t row_0 = 2, column_0 = 1, blockphase_0 = 12;
        uint16_t cell_id = WaveformDataPacket::CalculateCellID(row_0, column_0, blockphase_0);
        CHECK(cell_id == 332);
        uint16_t row = 0, column = 0, blockphase = 0;
        WaveformDataPacket::CalculateRowColumnBlockPhase(cell_id, row, column, blockphase);
        CHECK(row_0 == row);
        CHECK(column_0 == column);
        CHECK(blockphase_0 == blockphase);
    }

    SUBCASE("Valid") {
        WaveformDataPacket data_packet_0(0);
        CHECK(!data_packet_0.IsValid());

        WaveformDataPacket data_packet(packet_size);
        CHECK(!data_packet.IsValid());

        ReadBinary(path, data_packet.GetDataPacket(), packet_size);
        CHECK(data_packet.IsValid());
    }

    SUBCASE("Waveform Pointer") {
        WaveformDataPacket data_packet(packet_size);
        ReadBinary(path, data_packet.GetDataPacket(), packet_size);

        uint8_t* wf_pointer_0 = data_packet.GetWaveformPointer(0);
        uint8_t* wf_pointer_1 = data_packet.GetWaveformPointer(1);
        uint16_t last_wf = data_packet.GetNWaveforms() - 1;
        CHECK(wf_pointer_1 == &wf_pointer_0[data_packet.GetWaveformNBytes()]);

        uint8_t* wf_pointer_last = data_packet.GetWaveformPointer(last_wf);
        uint8_t* end_pointer_0 = &wf_pointer_last[data_packet.GetWaveformNBytes() + PACKET_FOOTER_WORDS * 2 - 1];
        uint8_t* end_pointer_1 = &data_packet.GetDataPacket()[data_packet.GetPacketNBytes() - 1];
        CHECK(end_pointer_0 == end_pointer_1);
    }
}

}  // namespace sstceed
}  // namespace cta
