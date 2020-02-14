// Copyright 2019 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstcam/interfaces/WaveformEvent.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace sstcam {
namespace interfaces {

namespace py = pybind11;

void get_hardcoded_module_situation(py::module &m) {
    m.def("GetHardcodedModuleSituation",
          [](std::set<uint8_t> active) {
              size_t n_pixels;
              uint8_t first;
              GetHardcodedModuleSituation(active, n_pixels, first);
              return std::make_tuple(n_pixels, first);
          });
}

template<typename T, typename TT>
py::array_t<TT> GetWaveforms(const T& waveform_event) {
    auto v = new std::vector<TT>(waveform_event.GetWaveforms());
    auto capsule = py::capsule(v, [](void *v) { delete reinterpret_cast<std::vector<TT>*>(v); });
    auto n_pixels = static_cast<long>(waveform_event.GetNPixels());
    auto n_samples = static_cast<long>(waveform_event.GetNSamples());
    auto shape = std::vector<ptrdiff_t>{n_pixels, n_samples};
    return py::array(shape, v->data(), capsule);
}

template<typename T, typename TT>
void waveform_event_template(py::module &m, const std::string& name) {
    py::class_<T> waveform_event(m, name.c_str());
    waveform_event.def(py::init<size_t, size_t, uint8_t>());
    waveform_event.def("GetWaveforms", GetWaveforms<T, TT>);
    waveform_event.def("GetPackets", &T::GetPackets);
    waveform_event.def("GetNPixels", &T::GetNPixels);
    waveform_event.def("GetNSamples", &T::GetNSamples);
    waveform_event.def("IsMissingPackets", &T::IsMissingPackets);
    waveform_event.def("GetFirstCellID", &T::GetFirstCellID);
    waveform_event.def("GetTACK", &T::GetTACK);
    waveform_event.def("IsStale", &T::IsStale);
}

void waveform_event(py::module &m) {
    waveform_event_template<WaveformEventR0, uint16_t>(m, "WaveformEventR0");
    waveform_event_template<WaveformEventR1, float>(m, "WaveformEventR1");
}

}}
