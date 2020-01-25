// Copyright 2020 Cherenkov Telescope Array Observatory
// This software is distributed under the terms of the BSD-3-Clause license.

#include "sstcam/interfaces/WaveformEvent.h"

namespace sstcam {
namespace interfaces {

WaveformRunHeader::WaveformRunHeader(size_t n_packets_per_event, size_t packet_size,
    std::set<uint8_t>& active_modules, size_t n_samples,
    bool is_r1, float scale, float offset)
        : n_packets_per_event(n_packets_per_event),
          packet_size(packet_size),
          n_samples(n_samples),
          is_r1(is_r1),
          scale(scale),
          offset(offset),
          first_module_slot(*active_modules.begin()),
          active_modules(active_modules)
{
    size_t n_modules_in_file = active_modules.size();
    size_t n_modules;

    // Hardcoded n_module situations
    if (n_modules_in_file == 1) {
        n_modules = 1; // Single module case
    }
    else if (n_modules_in_file > 1 && n_modules_in_file <= 32) {
        n_modules = 32; // SST camera case
        first_module_slot = 0;
    }
    else {
        n_modules = n_modules_in_file;
        std::cout << "WARNING: No case set up for files with N modules:"
        << n_modules_in_file << std::endl;
    }
    n_pixels = n_modules * N_PIXELS_PER_MODULE;
}

}}
