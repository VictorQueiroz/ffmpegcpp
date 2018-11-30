#ifndef FFMPEG_CPP_CODEC_H
#define FFMPEG_CPP_CODEC_H

#include "av.h"
#include "Frame.h"
#include "Packet.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <exception>
#include <string>

namespace av {
    class Codec {
    private:
        AVCodecContext* codec_ctx;
        AVCodec* codec;
    public:
        Codec(AVCodec*);
        AVCodecContext* context() {
            return codec_ctx;
        }
        void open();
        void setParameters(AVCodecParameters*);
        ~Codec();
    };

    class Encoder : public Codec {
    public:
        Encoder(AVCodecID);
        bool sendFrame(Frame&);
        /**
         * @param pkt
         * @return return TRUE if packet was received, otherwise FALSE
         */
        bool receivePacket(Packet&);
    };
    class Decoder : public Codec {
    public:
        Decoder(AVCodecID);
        /**
         * @param packet
         * @return TRUE if a frame should be received
         */
        bool sendPacket(Packet&);
        /**
         * @param frame
         * @return return TRUE if a frame was return but FALSE otherwise
         */
        bool receiveFrame(Frame&);
    };
}

#endif //FFMPEG_CPP_CODEC_H
