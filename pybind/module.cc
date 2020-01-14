// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include <pybind11/pybind11.h>

namespace cta {
namespace sstceed {

namespace py = pybind11;

void waveform_data_packet(py::module &m);
void waveform(py::module &m);

PYBIND11_MODULE(sstCEED, m) {
    waveform_data_packet(m);
    waveform(m);
}

}  // namespace sstceed
}  // namespace cta
