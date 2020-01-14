// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#ifndef SSTCEED_WAVEFORM_H
#define SSTCEED_WAVEFORM_H

#include "sstCEED/WaveformDataPacket.h"
#include <cstdint>
#include <iostream>

namespace cta {
namespace sstceed {

constexpr uint8_t CHANNELS_PER_ASIC = 16;

/*!
 * @class Waveform
 * @brief Container for a waveform segment of the WaveformDataPacket to
 * provide convenience access methods for waveform contents
 */
class Waveform {
public:
    Waveform() = default;
    ~Waveform() = default;

    void Associate(WaveformDataPacket& packet, uint16_t waveform_index) {
      uint16_t waveform_start = packet.GetWaveformStart(waveform_index);
      waveform_ = &packet.GetDataPacket()[waveform_start];
    }

    bool IsAssociated() { return waveform_; }

    bool IsErrorFlagOn() const { return waveform_[0] & 0x1u; }
    uint8_t GetChannelID() const {
        return static_cast<uint8_t>(waveform_[0] >> 1u) & 0xFu;
    }
    uint8_t GetASICID() const {
        return static_cast<uint8_t>(waveform_[0] >> 5u) & 0x3u;
    }

    uint16_t GetNSamples() const { return 16 * (waveform_[1] & 0x3Fu); }
    bool IsZeroSuppressed() const { return waveform_[1] & 0x80u; }

    // Get the value of a sample (12bit - R0 file)
    uint16_t GetSample12bit(uint16_t sample_index) const;

    // Get the value of a sample (16bit - R1 file)
    uint16_t GetSample16bit(uint16_t sample_index) const;

    // Set the value of a sample (12bit - R0 file)
    void SetSample12bit(uint16_t sample_index, uint16_t value);

    // Set the value of a sample (16bit - R1 file) (Uses the unused 4 bits)
    void SetSample16bit(uint16_t sample_index, uint16_t value);

    uint16_t GetPixelID() const {
        return GetASICID() * CHANNELS_PER_ASIC + GetChannelID();
    }

private:
    uint8_t* waveform_ = nullptr; // Memory owned by WaveformDataPacket

};
}  // namespace sstceed
}  // namespace cta


#endif //SSTCEED_WAVEFORM_H
