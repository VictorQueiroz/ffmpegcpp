#ifndef FFMPEG_CPP_PARSER_H
#define FFMPEG_CPP_PARSER_H

#include "av.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "Packet.h"
#include "Codec.h"

namespace av {
    class Parser {
    private:
        AVCodecParserContext* parser_ctx;
    public:
        Parser(AVCodecID codec_id) {
            parser_ctx = av_parser_init(codec_id);
        }
        ~Parser() {
            av_parser_close(parser_ctx);
        }
        /**
         * @param codec
         * @param rawPacket
         * @param outPacket
         * @return Return TRUE if packet was returned
         */
        bool parse(Codec& codec, Packet& rawPacket, Packet& outPacket) {
            AVPacket* out_packet = outPacket.c_packet();
            AVPacket* in_pkt = rawPacket.c_packet();
            int used_bytes;
            int remaining_bytes = in_pkt->size;
            while(remaining_bytes) {
                used_bytes = av_parser_parse2(
                        parser_ctx, codec.context(),
                        /* output */
                        &out_packet->data, &out_packet->size,
                        /* input */
                        in_pkt->data, in_pkt->size,
                        in_pkt->pts, in_pkt->dts, in_pkt->pos
                );
                remaining_bytes -= used_bytes;
                in_pkt->data += used_bytes;
                if (out_packet->size > 0) {
                    return true;
                }
            }
            return false;
        }
    };
}

#endif //FFMPEG_CPP_PARSER_H
