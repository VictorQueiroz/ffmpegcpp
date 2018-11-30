#include "Format.h"

using namespace av;

void Format::ensureInitialization() {
    if(format_ctx == nullptr) {
        throw Exception("You need to initialize AVFormatContext before accessing it");
    }
}

Format::~Format() {
    avformat_close_input(&format_ctx);
}

void Format::openInput(const char* url) {
    if(avformat_open_input(&format_ctx, url, nullptr, nullptr) != 0) {
        throw Exception("Failed to open input");
    }
}

AVStream* Format::getStreamFromMediaType(AVMediaType type) {
    ensureInitialization();
    unsigned int i;
    AVStream* stm = nullptr;
    for(i = 0; i < format_ctx->nb_streams; i++) {
        stm = format_ctx->streams[i];
        if(stm->codecpar->codec_type == type) {
            return stm;
        }
    }
    return nullptr;
}

bool Format::readFrame(av::Packet &packet)  {
    ensureInitialization();
    int result = av_read_frame(format_ctx, packet.c_packet());
    if(result != 0 && result != AVERROR_EOF) {
        AV_GET_ERROR_STRING(message);
        throw Exception(message);
    }
    if(result == AVERROR_EOF) {
        return false;
    }
    return true;
}