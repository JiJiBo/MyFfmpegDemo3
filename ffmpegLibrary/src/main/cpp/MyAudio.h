//
// Created by Administrator on 2020/9/8.
//

#ifndef MYFFMPEGDEMO3_MYAUDIO_H
#define MYFFMPEGDEMO3_MYAUDIO_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
};

#include "string.h"
#include "MyStatue.h"
#include "MyQueue.h"

class MyAudio {
public:
    MyAudio(MyStatue *myStatue, MyQueue *pQueue);

public:
    int streamIndex;
    AVCodecParameters *codecpar;
    AVCodecContext *pContext;
    MyStatue *statue = NULL;
    MyQueue *myQueue = NULL;

    pthread_t thread_play;
    AVPacket *packet = NULL;
    AVFrame *frame = NULL;
    int ret = 0;
    uint8_t *buffer = NULL;
    int dataSize = 0;
public:
    void start();

    int resampleAudio();
};


#endif //MYFFMPEGDEMO3_MYAUDIO_H
