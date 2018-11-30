#ifndef FFMPEG_CPP_PACKET_H
#define FFMPEG_CPP_PACKET_H

#include "av.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace av {
    class Packet {
    private:
        AVPacket *pkt;
    public:
        Packet();

        ~Packet() {
            av_packet_free(&pkt);
        }

        void unref() {
            av_packet_unref(pkt);
        }

        AVPacket *c_packet() {
            return pkt;
        }
    };
}

#endif //FFMPEG_CPP_PACKET_H
