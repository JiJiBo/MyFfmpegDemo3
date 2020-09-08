//
// Created by Administrator on 2020/9/8.
//

#include "MyAudio.h"


MyAudio::MyAudio(MyStatue *myStatue, MyQueue *pQueue) {
    this->statue = myStatue;
    this->myQueue = pQueue;
    buffer = (uint8_t *) av_malloc(44100 * 2 * 2);

}

void *play(void *data) {
    MyAudio *audio = static_cast<MyAudio *>(data);
    audio->resampleAudio();
    pthread_exit(&audio->thread_play);
}

void MyAudio::start() {
    pthread_create(&thread_play, NULL, play, this);
}

FILE *outFile = fopen("/storage/emulated/0/zcf/mymusic.pcm", "w");

int MyAudio::resampleAudio() {
    while (statue != NULL && !statue->exit) {
        packet = av_packet_alloc();
        if (myQueue->getAvPacket(packet) != 0) {
            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            continue;
        }
        ret = avcodec_send_packet(pContext, packet);
        if (ret != 0) {
            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            continue;
        }
        frame = av_frame_alloc();
        ret = avcodec_receive_frame(pContext, frame);
        //

        if (ret == 0) {
            if (frame->channels > 0 && frame->channel_layout == 0) {
                frame->channel_layout = av_get_default_channel_layout(frame->channels);
            } else if (frame->channel_layout > 0 && frame->channels == 0) {
                frame->channels = av_get_channel_layout_nb_channels(frame->channel_layout);
            }
            SwrContext *sctx;
            LOGE("111111111111")
            sctx = swr_alloc_set_opts(
                    NULL,
                    AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_S16,
                    frame->sample_rate,
                    frame->channel_layout,
                    (AVSampleFormat) frame->format,
                    frame->sample_rate,
                    NULL, NULL
            );
            LOGE("22222222222222")
            if (!sctx || swr_init(sctx) < 0) {
                av_packet_free(&packet);
                av_free(packet);
                packet = NULL;
                av_frame_free(&frame);
                av_free(frame);
                frame = NULL;
                swr_free(&sctx);
                continue;
            }
            LOGE("33333333333333")
            int nb = swr_convert(sctx,
                                 &buffer,
                                 frame->nb_samples,
                                 (const uint8_t **) frame->data,
                                 frame->nb_samples);
            LOGE("44444444444444444444v     %d", nb)
            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            LOGE("5555555555555555555")
            dataSize = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
            LOGE("66666666666666666666666666")
            fwrite(buffer, 1, dataSize, outFile);
            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            av_frame_free(&frame);
            av_free(frame);
            frame = NULL;
            swr_free(&sctx);
        } else {
            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            av_frame_free(&frame);
            av_free(frame);
            frame = NULL;
        }
    }
    fclose(outFile);
    return dataSize;
}
