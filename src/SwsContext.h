#ifndef FFMPEG_CPP_SWSCONTEXT_H
#define FFMPEG_CPP_SWSCONTEXT_H

#include "av.h"

extern "C" {
#include <libswscale/swscale.h>
}

namespace sws {
    class Context {
    private:
        SwsContext* sws_ctx;
    public:
        Context(int srcW, int srcH, AVPixelFormat srcFormat,
                int destW, int destH, AVPixelFormat destFormat,
                int flags = 0,
                SwsFilter* srcFilter = nullptr, SwsFilter* destFilter = nullptr,
                const double* param = nullptr) {
            sws_ctx = sws_getContext(srcW, srcH, srcFormat,
                                     destW, destH, destFormat,
                                     flags, srcFilter, destFilter, param);
            if(!sws_ctx) {
                throw av::Exception("Failed to allocate sws context");
            }
        }
        ~Context() {
            sws_freeContext(sws_ctx);
        }
        int scale(const uint8_t* const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH, uint8_t* const dst[], const int dstStride[]) {
            int height = sws_scale(sws_ctx, srcSlice, srcStride, srcSliceY, srcSliceH, dst, dstStride);
            if(height < 0) {
                throw av::Exception("failed to scale");
            }
            return height;
        }
    };
}

#endif //FFMPEG_CPP_SWSCONTEXT_H
