// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

//  #define PERFORM_CRC_CHECK


// clang-format off
//#define T_PACKET_HEADER_WORDS         8
//#define T_PACKET_FOOTER_WORDS         2
//#define T_WAVEFORM_HEADER_WORDS       1
//#define T_SAMPLES_PER_WAVEFORM_BLOCK 32
//// clang-format on
//
//// clang-format off
//#define T_PACKET_OK                0
//#define T_PACKET_ERROR_NODATA      1
//#define T_PACKET_ERROR_BADLENGTH   2
//#define T_PACKET_ERROR_BADCRC      3
//#define T_PACKET_ERROR_LASTBYTES   4
//#define T_PACKET_ERROR_NOWAVEFORMS 5
//// clang-format on


#ifndef SSTCEED_WAVEFORMDATAPACKET_H
#define SSTCEED_WAVEFORMDATAPACKET_H

#include <cstdint>
#include <iostream>

namespace cta {
namespace sstceed {

/*!
 * @class WaveformDataPacket
 * @brief Data packet for the waveform data from TARGET modules
 */
class WaveformDataPacket {
public:
    explicit WaveformDataPacket(size_t packet_size)
            : data_packet_(new uint8_t[packet_size]()),
              packet_size_(packet_size) {}

    // Get a reference to the data_packet_ pointer for filling
    uint8_t* GetDataPacket() { return data_packet_.get(); }
    size_t GetPacketSize() const { return packet_size_; }

    // Functions to extract values from data_packet_ ___________________________

    // Functions to set specific values in data_packet_ ________________________



private:
    std::unique_ptr<uint8_t[]> data_packet_; // Memory owned by class
    size_t packet_size_;

};


//public:
//    DataPacket_v2(uint16_t waveforms_per_packet, uint16_t samples_per_waveform);  // for simulations
//    explicit DataPacket_v2(uint16_t packetsize = 0);
//    virtual ~DataPacket_v2() { Deallocate(); }
//    void Clear();
//    void Deallocate();
//    void Allocate(uint16_t packetsize);
////    void Assign(uint8_t* data, uint16_t packetsize);
//
//    //Functions to extract information from the data paqcket
//    //TODO: MBG Check format, it seems to be a bit inconsistent.
//
//    inline uint16_t GetNumberOfWaveforms() const { return fData[0] & 0x7F; } // bits 0..6
//    inline bool IsLastSubPacket() const { return fData[1] & 0x1; }
//    inline bool IsFirstSubPacket() const { return fData[1] & 0x2; }  // need to implement for sims
//    inline uint16_t GetBuffers() const { return (fData[1] >> 2) & 0x3F; }
//    inline uint16_t GetEventNumber() const { return static_cast<uint16_t>((uint16_t(fData[2]) << 8) | fData[3]); }
//    // Is this correct?
//    inline uint64_t GetTACKTime() const {
//        return (uint64_t(fData[12]) << 56) | (uint64_t(fData[13]) << 48) |
//               (uint64_t(fData[10]) << 40) | (uint64_t(fData[11]) << 32) |
//               (uint64_t(fData[ 8]) << 24) | (uint64_t(fData[ 9]) << 16) |
//               (uint64_t(fData[ 2]) <<  8) | (uint64_t(fData[ 3]));
//    }
//    inline uint8_t GetSlotID() const { return fData[4]; }
//    inline uint8_t GetDetectorID() const { return fData[5]; }
//    inline uint8_t GetEventSequenceNumber() const { return fData[6]; }
//    inline uint8_t GetDetectorUniqueTag() const { return fData[7]; }
//
//    inline uint8_t GetColumn() const { return fData[14] & 0x3F; } //bits 0 .. 5
//    inline uint8_t GetStaleBit() const { return  (fData[14] >> 6) & 0x1; } //bit 6
//    inline bool IsZeroSupressionEnabled() const { return fData[14] & 0x8; } //Sould not be 0x80? (bit 7)
//
//    inline uint8_t GetRow() const { return (fData[15] >> 5) & 0x7; }
//    inline uint8_t GetBlockPhase() const { return fData[15] & 0x1F; }
//
//    inline uint8_t GetASICID() const { return (fData[16] >> 5) & 0x3; }  // of first waveform
//    inline uint8_t GetChannelID() const { return (fData[16] >> 1) & 0xF; }  // of first waveform
//    inline uint8_t GetTACKMode() const { return fData[17] >> 6; }
//
//    inline uint16_t CalculateWaveformLength() const { return (GetBuffers() / 2); }  // in 32 sample blocks
//    inline uint16_t CalculateWaveformSamples() const { return (CalculateWaveformBytes() / 2); }
//    inline uint16_t CalculateWaveformBytes() const { return (GetBuffers() * T_SAMPLES_PER_WAVEFORM_BLOCK); }
//    inline uint16_t CalculateTotalSizeInBytes() const {
//        return (GetNumberOfWaveforms() * (GetBuffers() * T_SAMPLES_PER_WAVEFORM_BLOCK + 2) +
//                2 * (T_PACKET_HEADER_WORDS + T_PACKET_FOOTER_WORDS));
//    }
//
//    //Helper functions to perform some calculations
//    static uint16_t CalculateFirstCellId(uint16_t pRow, uint16_t pColumn, uint16_t pBlockPhase) {
//        return (((pColumn * 8u) + pRow) * T_SAMPLES_PER_WAVEFORM_BLOCK + pBlockPhase);
//    }
//
//    static uint16_t CalculatePacketSizeInBytes(uint16_t pWaveformsPerPacket, uint16_t pSamplesPerWaveform) {
//        uint16_t nBlocks = 2 * (pSamplesPerWaveform / T_SAMPLES_PER_WAVEFORM_BLOCK);
//        return (pWaveformsPerPacket * (nBlocks * T_SAMPLES_PER_WAVEFORM_BLOCK + 2) +
//                2 * (T_PACKET_HEADER_WORDS + T_PACKET_FOOTER_WORDS));
//    }
//
//    static void CalculateRowColumnBlockPhase(uint16_t pCellId, uint16_t& pRow, uint16_t& pColumn, uint16_t& pBlockPhase) {
//        pBlockPhase = (pCellId % T_SAMPLES_PER_WAVEFORM_BLOCK);
//        pRow = (pCellId / T_SAMPLES_PER_WAVEFORM_BLOCK) % 8u;
//        pColumn = (pCellId / T_SAMPLES_PER_WAVEFORM_BLOCK) / 8u;
//    }
//
//    /// Return the total number of cells per ASIC channel
//    static uint16_t GetNCells() { return CalculateFirstCellId(7, 63, 0) + T_SAMPLES_PER_WAVEFORM_BLOCK; }
//    uint16_t GetFirstCellId() const { return CalculateFirstCellId(GetRow(), GetColumn(), GetBlockPhase()); }
//
//    bool IsValid();
//
//    inline void SetFilled (bool pFilled) { fFilled=pFilled; }
//    inline bool IsFilled (void) const { return fFilled; }
//    inline void SetBuilt (bool pBuilt) { fBuilt=pBuilt; }
//    inline bool IsBuilt (void) const { return fBuilt; }
//    inline void SetProcessed (bool pProcessed) { fProcessed=pProcessed; }
//    inline bool IsProcessed (void) const { return fProcessed; }
//
//    inline int GetStatus() { return fStatusFlag; };
//    inline std::string GetStatusString() { return fStatusString; };
//
//    uint8_t* GetData() { return fData; }
//    uint16_t GetPacketSize() const { return fPacketSize; }
//    bool GetPacketID(uint16_t& packet_id);  // NOLINT(runtime/references)
//    void AssociateWaveform(uint16_t n, Waveform& pWaveform);  // NOLINT(runtime/references)
//    Waveform* GetWaveform(uint16_t waveformindex);
//
//    void SummarisePacket(std::ostream& os = std::cout);  // NOLINT(runtime/references)
//    void Print() const;
//};
//
}  // namespace sstceed
}  // namespace cta


#endif //SSTCEED_WAVEFORMDATAPACKET_H
