#ifndef FFMPEG_CPP_FRAME_H
#define FFMPEG_CPP_FRAME_H

#include "av.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace av {
    class Frame {
    private:
        AVFrame* frame;
    public:
        Frame();
        ~Frame();
        AVFrame* c_frame();
        void getBuffer(int align);
    };
}

#endif //FFMPEG_CPP_FRAME_H
