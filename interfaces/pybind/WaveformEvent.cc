// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstcam/interfaces/WaveformEvent.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace sstcam {
namespace interfaces {

namespace py = pybind11;

void waveform_run_header(py::module &m) {
    py::class_<WaveformRunHeader> header(m, "WaveformRunHeader");
    header.def(py::init<size_t, size_t, std::set<uint8_t>&, size_t>());
    header.def(py::init<size_t, size_t, std::set<uint8_t>&, size_t, bool>());
    header.def(py::init<size_t, size_t, std::set<uint8_t>&, size_t, bool, float>());
    header.def(py::init<size_t, size_t, std::set<uint8_t>&, size_t, bool, float, float>());
    header.def_readwrite("n_packets_per_event", &WaveformRunHeader::n_packets_per_event);
    header.def_readwrite("packet_size", &WaveformRunHeader::packet_size);
    header.def_readwrite("n_pixels", &WaveformRunHeader::n_pixels);
    header.def_readwrite("n_samples", &WaveformRunHeader::n_samples);
    header.def_readwrite("is_r1", &WaveformRunHeader::is_r1);
    header.def_readwrite("scale", &WaveformRunHeader::scale);
    header.def_readwrite("offset", &WaveformRunHeader::offset);
    header.def_readwrite("first_module_slot", &WaveformRunHeader::first_module_slot);
}

template<typename T, typename TT>
py::array_t<TT> GetWaveforms(const T& waveform_event) {
    auto v = new std::vector<TT>(waveform_event.GetWaveforms());
    auto capsule = py::capsule(v, [](void *v) { delete reinterpret_cast<std::vector<TT>*>(v); });
    auto n_pixels = static_cast<long>(waveform_event.run_header->n_pixels);
    auto n_samples = static_cast<long>(waveform_event.run_header->n_samples);
    auto shape = std::vector<ptrdiff_t>{n_pixels, n_samples};
    return py::array(shape, v->data(), capsule);
}

template<typename T, typename TT>
void waveform_event_template(py::module &m, const std::string& name) {
    py::class_<T> waveform_event(m, name.c_str());
    waveform_event.def(py::init<WaveformRunHeader*>());
    waveform_event.def("GetWaveforms", GetWaveforms<T, TT>);
    waveform_event.def("SetEventHeaderFromPackets", &T::SetEventHeaderFromPackets);
    waveform_event.def_readonly("run_header", &T::run_header);
    waveform_event.def_readonly("packets", &T::packets);
    waveform_event.def_readonly("index", &T::index);
    waveform_event.def_readonly("first_cell_id", &T::first_cell_id);
    waveform_event.def_readonly("missing_packets", &T::missing_packets);
    waveform_event.def_readonly("tack", &T::tack);
    waveform_event.def_readonly("cpu_s", &T::cpu_s);
    waveform_event.def_readonly("cpu_ns", &T::cpu_ns);
}

void waveform_event(py::module &m) {
    waveform_event_template<WaveformEventR0, uint16_t>(m, "WaveformEventR0");
    waveform_event_template<WaveformEventR1, float>(m, "WaveformEventR1");
}

}}
