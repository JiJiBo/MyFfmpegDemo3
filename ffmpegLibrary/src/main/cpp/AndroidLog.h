//
// Created by Administrator on 2020/9/5.
//

#ifndef MYFFMPEGDEMO3_ANDROIDLOG_H
#define MYFFMPEGDEMO3_ANDROIDLOG_H

#endif //MYFFMPEGDEMO3_ANDROIDLOG_H

#include "android/log.h"

#define LOG_DEBUG true

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,"1123",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"1123",FORMAT,##__VA_ARGS__);