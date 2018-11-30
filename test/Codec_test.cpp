#include "../src/Codec.h"
#include "../src/SwsContext.h"
#include "../src/Frame.h"
#include "../src/Parser.h"
#include "../src/Format.h"

#include <iostream>
#include <assert.h>

int total_frames = 0;

void CreateThumbnailFromFrame(av::Frame& frame) {
    AVFrame* c_frame = frame.c_frame();
    double ratio = (double) c_frame->width/c_frame->height;
    int dest_width = (int) (c_frame->width*ratio);
    int dest_height = (int) (dest_width/ratio);

    sws::Context swsContext(c_frame->width, c_frame->height, (AVPixelFormat) c_frame->format,
                            dest_width, dest_height, AV_PIX_FMT_RGBA);

    av::Frame scaledFrame;
    scaledFrame.c_frame()->width = dest_width;
    scaledFrame.c_frame()->height = dest_height;
    scaledFrame.c_frame()->format = AV_PIX_FMT_RGBA;
    scaledFrame.getBuffer(32);

    swsContext.scale(c_frame->data, c_frame->linesize, 0, c_frame->height,
                    scaledFrame.c_frame()->data, scaledFrame.c_frame()->linesize);

    av::Encoder pngEncoder(AV_CODEC_ID_PNG);

    AVCodecContext* png_ctx = pngEncoder.context();
    png_ctx->width = dest_width;
    png_ctx->height = dest_height;
    png_ctx->pix_fmt = AV_PIX_FMT_RGBA;
    png_ctx->time_base = { 1, 1 };
    pngEncoder.open();

    FILE* f = fopen("../test/expected_thumbnail.png", "r");
    assert(f != nullptr);

    fseek(f, 0L, SEEK_END);
    long expected_length = ftell(f);
    rewind(f);

    av::Packet pngPacket;

    bool success = pngEncoder.sendFrame(scaledFrame);
    if(success) {
        while(pngEncoder.receivePacket(pngPacket)) {
            int length = pngPacket.c_packet()->size;
            uint8_t* output = new uint8_t[length];
            fread(output, 1, (size_t) length, f);
            assert(expected_length == length);
            for(int i = 0; i < length; i++) {
                assert(pngPacket.c_packet()->data[i] == output[i]);
            }
            pngPacket.unref();
            delete[] output;
        }
    }
    fclose(f);
}

int main() {
    av::Init();
    av::Format format;
    format.openInput("../test/VID_20181120_115325.mp4");

    av::Packet packet;
    AVStream* stm = format.getStreamFromMediaType(AVMEDIA_TYPE_VIDEO);
    assert(stm != nullptr);

    av::Parser parser(stm->codecpar->codec_id);
    av::Decoder decoder(stm->codecpar->codec_id);
    decoder.setParameters(stm->codecpar);
    decoder.open();

    av::Packet parsedPacket;
    av::Frame decodedFrame;

    while(format.readFrame(packet)) {
        if(packet.c_packet()->stream_index != stm->index) {
            packet.unref();
            continue;
        }
        bool packetAvailable = parser.parse(decoder, packet, parsedPacket);
        if(!packetAvailable) {
            continue;
        }

        bool frameAvailable = decoder.sendPacket(parsedPacket);
        if(!frameAvailable) {
            continue;
        }
        if(total_frames == 1) {
            return 0;
        }
        while(decoder.receiveFrame(decodedFrame)) {
            decodedFrame.c_frame()->format = decoder.context()->pix_fmt;
            decodedFrame.c_frame()->width = stm->codecpar->width;
            decodedFrame.c_frame()->height = stm->codecpar->height;
            CreateThumbnailFromFrame(decodedFrame);
            total_frames++;
        }
        packet.unref();
    }
}
