#ifndef FFMPEG_CPP_FORMAT_H
#define FFMPEG_CPP_FORMAT_H

#include "av.h"
#include "Packet.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace av {
    class Format {
    private:
        AVFormatContext* format_ctx = nullptr;
        void ensureInitialization();
    public:
        ~Format();
        void openInput(const char*);
        AVStream* getStreamFromMediaType(AVMediaType);
        /**
         * @param packet
         * @return TRUE if a frame was return, FALSE on end of file
         */
        bool readFrame(Packet&);
    };
}

#endif //FFMPEG_CPP_FORMAT_H
