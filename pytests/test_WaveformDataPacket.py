import pytests
import numpy as np
from sstCEED import WaveformDataPacket


@pytests.fixture(scope='module')
def packet():
    path = "../share/sstCEED/waveform_data_packet_example.bin"
    return np.fromfile(path, dtype=np.uint8)


def test_constructor(packet):
    data_packet = WaveformDataPacket(packet.size)
    assert data_packet.GetPacketSize() == packet.size

