//
// Created by Administrator on 2020/9/8.
//

#ifndef MYFFMPEGDEMO3_MYAUDIO_H
#define MYFFMPEGDEMO3_MYAUDIO_H

#include "string.h"
#include "MyStatue.h"
#include "MyQueue.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};


class MyAudio {
public:
    MyAudio(MyStatue *myStatue, MyQueue *pQueue,int sample_rate);

public:
    int streamIndex;
    int sample_rate;
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


    SLObjectItf engineObj = NULL;
    SLEngineItf engineEngine = NULL;

    SLObjectItf outputObj = NULL;
    SLEnvironmentalReverbItf outputItf = NULL;
    SLEnvironmentalReverbSettings outputSlSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    SLObjectItf pcmObj = NULL;
    SLPlayItf pcmPlay = NULL;
    SLVolumeItf pcmVolume = NULL;

    SLAndroidSimpleBufferQueueItf androidSimpleBufferQueue = NULL;

    uint8_t *out_buffer;
public:
    void start();

    int resampleAudio();
    void init();

    int getCurrentSampleRateForOpensles(int sample_rate);
};


#endif //MYFFMPEGDEMO3_MYAUDIO_H
