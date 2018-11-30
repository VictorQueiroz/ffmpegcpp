#ifndef FFMPEG_CPP_AV_H
#define FFMPEG_CPP_AV_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#include <exception>

/**
 * Create a variable with an error message
 */
#define AV_GET_ERROR_STRING(v) \
    char v[AV_ERROR_MAX_STRING_SIZE]; \
    av_make_error_string(v, AV_ERROR_MAX_STRING_SIZE, result);

namespace av {
    class Exception : std::exception {
    private:
        const char* message;
    public:
        Exception(const char*);
        const char* what();
    };
    void SetLogLevel(int level);
    void Init();
}

#endif //FFMPEG_CPP_AV_H
