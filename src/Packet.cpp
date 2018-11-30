#include "Packet.h"

using namespace av;

Packet::Packet() {
    pkt = av_packet_alloc();
    if (!pkt) {
        throw Exception("Failed to allocate packet");
    }
}