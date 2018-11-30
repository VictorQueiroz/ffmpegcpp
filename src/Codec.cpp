#include "Codec.h"

using namespace av;

Encoder::Encoder(AVCodecID codec_id): Codec(avcodec_find_encoder(codec_id)) {

}

bool Encoder::receivePacket(av::Packet& pkt) {
    int result = avcodec_receive_packet(context(), pkt.c_packet());
    if(result == 0) {
        return true;
    }
    if(result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
        AV_GET_ERROR_STRING(message);
        throw Exception(message);
    }
    return false;
}

bool Encoder::sendFrame(av::Frame& frame) {
    int result = avcodec_send_frame(context(), frame.c_frame());
    if (result == 0) {
        return true;
    }
    if (result == AVERROR(EAGAIN)) {
        return true;
    }
    if (result != AVERROR_EOF) {
        AV_GET_ERROR_STRING(message);
        throw Exception(message);
    }
    return false;
}

Decoder::Decoder(AVCodecID codec_id): Codec(avcodec_find_decoder(codec_id)) {

}

bool Decoder::sendPacket(av::Packet& packet) {
    int result = avcodec_send_packet(context(), packet.c_packet());
    if(result == 0 || result == AVERROR(EAGAIN)) {
        return true;
    }
    if(result == AVERROR_EOF) {
        return false;
    }

    AV_GET_ERROR_STRING(error_str);
    throw Exception(error_str);
}

bool Decoder::receiveFrame(av::Frame& frame) {
    int result = avcodec_receive_frame(context(), frame.c_frame());
    if(result != 0) {
        if(result != AVERROR_EOF && result != AVERROR(EAGAIN)) {
            AV_GET_ERROR_STRING(error_str);
            throw Exception(error_str);
        }
        return false;
    }
    return true;
}

Codec::Codec(AVCodec* codec): codec(codec) {
    if(!codec) {
        throw Exception("Codec not found");
    }
    codec_ctx = avcodec_alloc_context3(codec);
    if(!codec_ctx) {
        throw Exception("Failed to allocate context");
    }
}

void Codec::open() {
    int result = avcodec_open2(codec_ctx, codec, nullptr);
    if(result != 0) {
        AV_GET_ERROR_STRING(error);
        throw Exception(error);
    }
}

void Codec::setParameters(AVCodecParameters* params) {
    int result = avcodec_parameters_to_context(codec_ctx, params);
    if(result >= 0) {
        return;
    }
    throw Exception("Failed to set parameters");
}

Codec::~Codec() {
    avcodec_free_context(&codec_ctx);
}