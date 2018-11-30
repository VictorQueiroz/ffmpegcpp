#include "av.h"

using namespace av;

void av::SetLogLevel(int level) {
    av_log_set_level(level);
}

void av::Init() {
    av_register_all();
}

Exception::Exception(const char* message): std::exception(), message(message) {}

const char* Exception::what() {
    return message;
}