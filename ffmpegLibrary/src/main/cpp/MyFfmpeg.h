//
// Created by Administrator on 2020/9/5.
//

#ifndef MYFFMPEGDEMO3_MYFFMPEG_H
#define MYFFMPEGDEMO3_MYFFMPEG_H

extern "C"{
#include "libavformat/avformat.h"
};
#include "MyCallJava.h"
#include "MyQueue.h"
#include "pthread.h"

class MyFfmpeg {
public:
    MyCallJava *myCallJava;
    MyQueue *myQueue;
    const char *  url;
    MyStatue *myStatue;
    pthread_t prepare_thread;
    AVFormatContext *avFormatContext;
    int  streamIndex;
    AVCodecParameters *codecpar;
    AVCodecContext *pContext;
public:
    MyFfmpeg(MyCallJava *callJava, const char * str);

    ~MyFfmpeg();

    void prepare();

    void callJava();

    void deal();
    void start();
};


#endif //MYFFMPEGDEMO3_MYFFMPEG_H