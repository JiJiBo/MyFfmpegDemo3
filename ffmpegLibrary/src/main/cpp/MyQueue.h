//
// Created by Administrator on 2020/9/5.
//

#ifndef MYFFMPEGDEMO3_MYQUEUE_H
#define MYFFMPEGDEMO3_MYQUEUE_H

#include "queue"
#include "AndroidLog.h"
#include "pthread.h"
#include "MyStatue.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};

class MyQueue {
public:
    pthread_mutex_t  mutex;
    pthread_cond_t  cond;
    std::queue<AVPacket *> queue;
    MyStatue *statue;
public:
    MyQueue(MyStatue *statue);

    ~MyQueue();

    int putAvPacket(AVPacket *packet);

    int getAvPacket(AVPacket *packet);

    int getQueueSize();
};


#endif //MYFFMPEGDEMO3_MYQUEUE_H
