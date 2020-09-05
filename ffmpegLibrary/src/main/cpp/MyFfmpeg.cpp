//
// Created by Administrator on 2020/9/5.
//


#include "MyFfmpeg.h"

void *prepare_method(void *data) {
    MyFfmpeg *ffmpeg = (MyFfmpeg *) data;
    ffmpeg->deal();
    pthread_exit(&ffmpeg->prepare_thread);
}

MyFfmpeg::MyFfmpeg(MyCallJava *callJava, const char *str) {
    this->myCallJava = callJava;
    this->myStatue = new MyStatue();
    this->myQueue = new MyQueue(myStatue);
    url = str;
}

void MyFfmpeg::prepare() {
    pthread_create(&prepare_thread, NULL, prepare_method, this);
}

void MyFfmpeg::callJava() {
    this->myCallJava->callJava(CHILD_THREAD);
}

void MyFfmpeg::deal() {
    avcodec_register_all();
    avformat_network_init();
    avFormatContext = avformat_alloc_context();
    if (avformat_open_input(&avFormatContext, url, NULL, NULL) != 0) {
        LOGE("文件打开失败");
        return;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        LOGE("寻找流失败");
        return;
    }
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            streamIndex = i;
            codecpar = avFormatContext->streams[i]->codecpar;
            break;
        }
    }

    AVCodec *pCodec = avcodec_find_decoder(codecpar->codec_id);
    if (!pCodec) {
        LOGE("寻找解码器失败")
        return;
    }

    pContext = avcodec_alloc_context3(pCodec);
    if (!pContext) {
        LOGE("寻找AVCodecContext失败")
        return;
    }
    if (avcodec_parameters_to_context(pContext, codecpar) < 0) {
        LOGE("avcodec_parameters_to_context  失败")
        return;
    }
    int open2 = avcodec_open2(pContext, pCodec, NULL);
    if (open2 != 0) {
        LOGE("avcodec_open2  失败")
        return;
    }
    callJava();
}

void MyFfmpeg::start() {
    while (1) {
        AVPacket *pPacket = av_packet_alloc();
        if (av_read_frame(avFormatContext, pPacket) == 0) {
            if (pPacket->stream_index == this->streamIndex) {
                this->myQueue->putAvPacket(pPacket);
            } else {
                av_packet_free(&pPacket);
                av_free(pPacket);
            }
        } else {
            av_packet_free(&pPacket);
            av_free(pPacket);

            break;
        }
    }

    while (myQueue->getQueueSize() > 0) {

        AVPacket *pAvPacket = av_packet_alloc();

        myQueue->getAvPacket(pAvPacket);

        av_packet_free(&pAvPacket);

        av_free(pAvPacket);

        pAvPacket = NULL;


    }

    LOGE("编译完成");
}

MyFfmpeg::~MyFfmpeg() {

}
