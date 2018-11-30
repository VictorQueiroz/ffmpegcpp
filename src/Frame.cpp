#include "Frame.h"

using namespace av;

Frame::Frame() {
    frame = av_frame_alloc();
    if(!frame) {
        throw Exception("Failed to allocate frame");
    }
}

Frame::~Frame() {
    av_frame_free(&frame);
}

AVFrame* Frame::c_frame() {
    return frame;
}

void Frame::getBuffer(int align) {
    int result = av_frame_get_buffer(frame, align);
    if(result < 0) {
        AV_GET_ERROR_STRING(error);
        throw Exception(error);
    }
}