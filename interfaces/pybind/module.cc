// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include <pybind11/pybind11.h>
#include <sstcam/interfaces/WaveformEvent.h>

namespace sstcam {
namespace interfaces {

namespace py = pybind11;

void waveform_data_packet(py::module &m);
void waveform(py::module &m);
void waveform_event(py::module &m);
void waveform_run_header(py::module &m);

PYBIND11_MODULE(sstcam_interfaces, m) {
    waveform_data_packet(m);
    waveform(m);
    waveform_event(m);
    waveform_run_header(m);
}

}  // namespace interfaces
}  // namespace sstcam
