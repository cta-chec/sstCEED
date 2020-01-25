// Copyright 2020 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#ifndef SSTCAM_INTERFACES_EVENT_H
#define SSTCAM_INTERFACES_EVENT_H

#include "sstcam/interfaces/WaveformDataPacket.h"
#include "sstcam/interfaces/Waveform.h"
#include <memory>
#include <vector>
#include <set>


namespace sstcam {
namespace interfaces {

constexpr uint8_t N_ASICS = 4;
constexpr uint8_t N_PIXELS_PER_MODULE = interfaces::CHANNELS_PER_ASIC * N_ASICS;

/*!
 * @struct RunHeader
 * @brief Container for header information that is common for all events in a
 * run. To be filled during construction of an  event builder or file reader.
 */
struct WaveformRunHeader {
    size_t n_packets_per_event;
    size_t packet_size;
    size_t n_pixels;
    size_t n_samples;
    bool is_r1;
    float scale;
    float offset;
    uint8_t first_module_slot;
    std::set<uint8_t> active_modules;

    WaveformRunHeader(size_t n_packets_per_event, size_t packet_size,
        std::set<uint8_t>& active_modules, size_t n_samples,
        bool is_r1=false, float scale=1, float offset=0);
};

/*!
 * @struct WaveformEvent
 * @brief Container for an event. To be filled with WaveformEventPackets and
 * event header information by the event builder or file reader.
 * The method by which the WaveformEvent is filled is defined by the
 * builder or reader.
 */
template<typename T>
struct WaveformEvent {
    WaveformRunHeader* run_header;
    std::vector<WaveformDataPacket> packets;

    // WaveformEvent Headers
    uint32_t index;
    uint16_t first_cell_id;
    bool stale;
    bool missing_packets;
    uint64_t tack;
    int64_t cpu_s;
    int64_t cpu_ns;

    // TODO: fill missing header values

    // TODO: Check difference in tack between header and packet

    explicit WaveformEvent(WaveformRunHeader* run_header)
        : run_header(run_header),
          index(0),
          first_cell_id(0),
          stale(false),
          missing_packets(false),
          tack(0),
          cpu_s(0),
          cpu_ns(0)
    {
        packets.reserve(run_header->n_packets_per_event);
        for (size_t i=0; i<run_header->n_packets_per_event; i++) {
            packets.emplace_back(run_header->packet_size);
        }
    }

    virtual ~WaveformEvent() = default;

    // Move constructor
    WaveformEvent(WaveformEvent&& a) noexcept {
        packets = std::move(a.packets);
    }

    virtual T GetSample(Waveform& wf, uint16_t isam) const = 0;

    void SetEventHeaderFromPackets() {
        for (WaveformDataPacket &packet : packets) {
            std::cout << packet.IsEmpty() << std::endl;
            if (!packet.IsEmpty()) {
                first_cell_id = packet.GetFirstCellID();
                tack = packet.GetTACKTime();
                if (packet.GetStaleBit() == 1) stale = true;
            } else missing_packets = true;
        }
    }

    std::vector<T> GetWaveforms() const {
        size_t size = run_header->n_pixels*run_header->n_samples;
        std::vector<T> samples(size, 0);
        Waveform waveform;

        for (const WaveformDataPacket& packet : packets) {
            uint16_t n_waveforms = packet.GetNWaveforms();
            uint8_t module = packet.GetSlotID() - run_header->first_module_slot;
            for (unsigned short iwav = 0; iwav < n_waveforms; iwav++) {
                waveform.Associate(packet, iwav);
                uint16_t n_samples = waveform.GetNSamples();
                uint16_t ipix = module*N_PIXELS_PER_MODULE + waveform.GetPixelID();
                for (unsigned short isam = 0; isam < n_samples; isam++) {
                    samples[ipix * n_samples + isam] = GetSample(waveform, isam);
                }
            }
        }
        return samples;
    }
};

struct WaveformEventR0 : WaveformEvent<uint16_t> {
    explicit WaveformEventR0(WaveformRunHeader* run_header)
        : WaveformEvent<uint16_t>(run_header) {}

    uint16_t GetSample(Waveform& wf, uint16_t isam) const final {
        return wf.GetSample12bit(isam);
    }
};

struct WaveformEventR1 : WaveformEvent<float> {
    explicit WaveformEventR1(WaveformRunHeader* run_header)
        : WaveformEvent<float>(run_header) {}

    float GetSample(Waveform& wf, uint16_t isam) const final {
        auto sample = static_cast<float>(wf.GetSample16bit(isam));
        return (sample / run_header->scale) - run_header->offset;
    }
};


}}

#endif //SSTCAM_INTERFACES_EVENT_H
