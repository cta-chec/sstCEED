// Copyright 2020 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstcam/interfaces/WaveformEvent.h"
#include "doctest.h"
#include <fstream>

namespace sstcam {
namespace interfaces {

TEST_CASE("WaveformEvent") {
    std::string path = "../../share/sstcam/interfaces/waveform_data_packet_example.bin";
    size_t packet_size = 8276;
    std::ifstream file (path, std::ios::in | std::ios::binary);
    CHECK(file.is_open());
    WaveformDataPacket packet(packet_size);
    file.read(reinterpret_cast<char*>(packet.GetDataPacket()), packet_size);

    size_t n_packets_per_event = 1;
    std::set<uint8_t> active_module_slots = {packet.GetSlotID()};
    size_t n_samples = packet.GetWaveformNSamples();

    SUBCASE("WaveformRunHeader") {
        std::set<uint8_t> slots = {5};
        WaveformRunHeader run_header_single(n_packets_per_event, packet_size, slots, n_samples);
        CHECK(run_header_single.n_pixels == 64);
        CHECK(run_header_single.n_packets_per_event == n_packets_per_event);
        CHECK(run_header_single.packet_size == packet_size);
        CHECK(run_header_single.n_samples == n_samples);
        CHECK(!run_header_single.is_r1);
        CHECK(run_header_single.scale == 1);
        CHECK(run_header_single.offset == 0);
        CHECK(run_header_single.first_module_slot == 5);

        slots.insert(4);
        WaveformRunHeader run_header_dual(n_packets_per_event, packet_size, slots,
            n_samples, true, 5, 6);
        CHECK(run_header_dual.n_pixels == 2048);
        CHECK(run_header_dual.n_packets_per_event == n_packets_per_event);
        CHECK(run_header_dual.packet_size == packet_size);
        CHECK(run_header_dual.n_samples == n_samples);
        CHECK(run_header_dual.is_r1);
        CHECK(run_header_dual.scale == 5);
        CHECK(run_header_dual.offset == 6);
        CHECK(run_header_dual.first_module_slot == 0);

        for (uint8_t slot=0; slot < 32; slot++) {
            slots.insert(slot);
            WaveformRunHeader run_header_camera(n_packets_per_event, packet_size, slots, n_samples);
            CHECK(run_header_camera.n_pixels == 2048);
        }

        slots.insert(32);
        WaveformRunHeader run_header_unknown(n_packets_per_event, packet_size, slots, n_samples);
        CHECK(run_header_unknown.n_pixels == 2048+64);
    }

    auto run_header = std::make_shared<WaveformRunHeader>(
        n_packets_per_event, packet_size, active_module_slots, n_samples);

    SUBCASE("WaveformEventR0 Constructor") {
        WaveformEventR0 event(run_header);
        CHECK(event.packets.size() == n_packets_per_event);
        CHECK(event.packets[0].IsEmpty());
        std::vector<uint16_t> waveforms = event.GetWaveforms();
        bool all_zero = true;
        for (size_t ipix = 0; ipix < event.run_header->n_pixels; ipix++) {
            for (size_t isam = 0; isam < event.run_header->n_samples; isam++) {
                if (waveforms[ipix * n_samples + isam] != 0) all_zero = false;
            }
        }
        CHECK(all_zero);
    }

    SUBCASE("WaveformEventR1 Constructor") {
        WaveformEventR1 event(run_header);
        CHECK(event.packets.size() == n_packets_per_event);
        CHECK(event.packets[0].IsEmpty());
        std::vector<float> waveforms = event.GetWaveforms();
        bool all_zero = true;
        for (size_t ipix = 0; ipix < event.run_header->n_pixels; ipix++) {
            for (size_t isam = 0; isam < event.run_header->n_samples; isam++) {
                if (waveforms[ipix * n_samples + isam] != 0) all_zero = false;
            }
        }
        CHECK(all_zero);
    }

    WaveformEventR0 event_r0(run_header);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(event_r0.packets[0].GetDataPacket()), packet_size);

    WaveformEventR1 event_r1(run_header);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(event_r1.packets[0].GetDataPacket()), packet_size);

    SUBCASE("WaveformEventR0 Headers") {
        event_r0.SetEventHeaderFromPackets();
        CHECK(event_r0.index == 0);
        CHECK(event_r0.first_cell_id == 1448);
        CHECK(event_r0.tack == 2165717354592);
        CHECK(!event_r0.stale);
        CHECK(!event_r0.missing_packets);
    }

    SUBCASE("WaveformEventR0 Sample") {
        Waveform waveform;
        waveform.Associate(event_r0.packets[0], event_r0.packets[0].GetWaveformStart(0));
        CHECK(event_r0.GetSample(waveform, 0) == 636);
    }

    SUBCASE("WaveformEventR1 Sample") {
        Waveform waveform;
        waveform.Associate(event_r1.packets[0], event_r1.packets[0].GetWaveformStart(0));
        CHECK(event_r1.GetSample(waveform, 0) == 636.0f);
    }

    run_header->scale = 10;
    run_header->offset = 3;

    SUBCASE("WaveformEventR1 Sample Scale&Offset") {
        REQUIRE(event_r0.run_header->scale == 10);
        REQUIRE(event_r0.run_header->offset == 3);
        Waveform waveform;
        waveform.Associate(event_r0.packets[0], event_r0.packets[0].GetWaveformStart(0));
        CHECK(event_r0.GetSample(waveform, 0) == 636);
    }

    SUBCASE("WaveformEventR1 Sample Scale&Offset") {
        REQUIRE(event_r1.run_header->scale == 10);
        REQUIRE(event_r1.run_header->offset == 3);
        Waveform waveform;
        waveform.Associate(event_r1.packets[0], event_r1.packets[0].GetWaveformStart(0));
        CHECK(event_r1.GetSample(waveform, 0) == 60.60f);
    }

    SUBCASE("WaveformEventR0 Waveforms") {
        std::vector<uint16_t> waveforms = event_r0.GetWaveforms();
        bool none_zero = true;
        for (size_t ipix = 0; ipix < event_r0.run_header->n_pixels; ipix++) {
            for (size_t isam = 0; isam < event_r0.run_header->n_samples; isam++) {
                if (ipix >= 32) continue;
                if (waveforms[ipix * n_samples + isam] == 0) none_zero = false;
            }
        }
        CHECK(none_zero);
    }

    SUBCASE("WaveformEventR1 Waveforms") {
        std::vector<float> waveforms = event_r1.GetWaveforms();
        bool none_zero = true;
        for (size_t ipix = 0; ipix < event_r1.run_header->n_pixels; ipix++) {
            for (size_t isam = 0; isam < event_r1.run_header->n_samples; isam++) {
                if (ipix >= 32) continue;
                if (waveforms[ipix * n_samples + isam] == 0) none_zero = false;
            }
        }
        CHECK(none_zero);
    }
}

}}
