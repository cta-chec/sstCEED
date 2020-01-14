// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstCEED/WaveformDataPacket.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

namespace cta {
namespace sstceed {

namespace py = pybind11;

class WaveformDataPacketPy : public WaveformDataPacket {
public:
    using WaveformDataPacket::WaveformDataPacket;

    py::array_t<uint8_t> GetDataPacketBuffer() {
        return py::array_t<uint8_t>(GetPacketSize(), GetDataPacket(), py::cast(*this));
    }
};

void waveform_data_packet(py::module &m) {
    py::class_<WaveformDataPacket> datapacketbase(m, "_WaveformDataPacket");
    py::class_<WaveformDataPacketPy, WaveformDataPacket> datapacket(m, "WaveformDataPacket");
    datapacket.def(py::init<size_t>());
    datapacket.def("GetDataPacket", &WaveformDataPacketPy::GetDataPacketBuffer, py::return_value_policy::reference_internal);
    datapacket.def("GetPacketSize", &WaveformDataPacketPy::GetPacketSize);
    datapacket.def("GetNWaveforms", &WaveformDataPacketPy::GetNWaveforms);
    datapacket.def("GetRegisterSetup", &WaveformDataPacketPy::GetRegisterSetup);
    datapacket.def("IsLastSubPacket", &WaveformDataPacketPy::IsLastSubPacket);
    datapacket.def("IsFirstSubPacket", &WaveformDataPacketPy::IsFirstSubPacket);
    datapacket.def("GetNBuffers", &WaveformDataPacketPy::GetNBuffers);
    datapacket.def("GetTACKTime", &WaveformDataPacketPy::GetTACKTime);
    datapacket.def("GetEventNumber", &WaveformDataPacketPy::GetEventNumber);
    datapacket.def("GetSlotID", &WaveformDataPacketPy::GetSlotID);
    datapacket.def("GetDetectorID", &WaveformDataPacketPy::GetDetectorID);
    datapacket.def("GetEventSequenceNumber", &WaveformDataPacketPy::GetEventSequenceNumber);
    datapacket.def("GetDetectorUniqueTag", &WaveformDataPacketPy::GetDetectorUniqueTag);
    datapacket.def("GetColumn", &WaveformDataPacketPy::GetColumn);
    datapacket.def("GetStaleBit", &WaveformDataPacketPy::GetStaleBit);
    datapacket.def("IsZeroSupressionEnabled", &WaveformDataPacketPy::IsZeroSupressionEnabled);
    datapacket.def("GetRow", &WaveformDataPacketPy::GetRow);
    datapacket.def("GetBlockPhase", &WaveformDataPacketPy::GetBlockPhase);
    datapacket.def("GetCRC", &WaveformDataPacketPy::GetCRC);
    datapacket.def("GetMBZ", &WaveformDataPacketPy::GetMBZ);
    datapacket.def("IsTimeout", &WaveformDataPacketPy::IsTimeout);
    datapacket.def("IsError", &WaveformDataPacketPy::IsError);
    datapacket.def("GetWaveformSamplesNBlocks", &WaveformDataPacketPy::GetWaveformSamplesNBlocks);
    datapacket.def("GetWaveformSamplesNBytes", &WaveformDataPacketPy::GetWaveformSamplesNBytes);
    datapacket.def("GetWaveformNSamples", &WaveformDataPacketPy::GetWaveformNSamples);
    datapacket.def("GetWaveformNBytes", &WaveformDataPacketPy::GetWaveformNBytes);
    datapacket.def("GetWaveformStart", &WaveformDataPacketPy::GetWaveformStart);
    datapacket.def("GetPacketNBytes", &WaveformDataPacketPy::GetPacketNBytes);
    datapacket.def("CalculatePacketSizeBytes", &WaveformDataPacketPy::CalculatePacketSizeBytes);
    datapacket.def("CalculateCellID", &WaveformDataPacketPy::CalculateCellID);
    datapacket.def("CalculateRowColumnBlockPhase", &WaveformDataPacketPy::CalculateRowColumnBlockPhase);
    datapacket.def("IsValid", &WaveformDataPacketPy::IsValid);
}

}  // namespace sstceed
}  // namespace cta
