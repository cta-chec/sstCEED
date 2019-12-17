// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

//#include "sstCEED/WaveformDataPacket.h"
#include <pybind11/pybind11.h>
namespace py = pybind11;

template <typename T, uint8_t size, uint8_t start>
inline T BitExtractTemplate_(uint8_t byte) {
    constexpr uint8_t mask = (1u << size) - 1u;
    return mask & static_cast<uint8_t>(byte >> start);
}

uint8_t BitExtractManual(uint8_t byte) {return (byte >> 5) & 0x3; }
uint8_t BitExtractTemplate(uint8_t byte) { return BitExtractTemplate_<uint8_t, 2, 5>(byte); }

PYBIND11_MODULE(sstCEEDpy, m) {
    m.def("BitExtractManual", &BitExtractManual);
    m.def("BitExtractTemplate", &BitExtractTemplate);
}
