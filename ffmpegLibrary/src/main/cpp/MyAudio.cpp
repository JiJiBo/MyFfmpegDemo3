//
// Created by Administrator on 2020/9/8.
//

#include "MyAudio.h"


MyAudio::MyAudio(MyStatue *myStatue, MyQueue *pQueue, int sample_rate) {
    this->statue = myStatue;
    this->myQueue = pQueue;
    buffer = (uint8_t *) av_malloc(sample_rate * 2 * 2);
    this->sample_rate = sample_rate;
}

void *play(void *data) {
    MyAudio *audio = static_cast<MyAudio *>(data);
    audio->init();
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

            int nb = swr_convert(sctx,
                                 &buffer,
                                 frame->nb_samples,
                                 (const uint8_t **) frame->data,
                                 frame->nb_samples);

            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);

            dataSize = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            fwrite(buffer, 1, dataSize, outFile);

            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            av_frame_free(&frame);
            av_free(frame);
            frame = NULL;
            swr_free(&sctx);

            break;
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


void pcmBufferCallback(SLAndroidSimpleBufferQueueItf buff, void *context) {
    MyAudio *audio = static_cast<MyAudio *>(context);
    if (NULL != audio) {
        SLresult result;
        int datasize = audio->resampleAudio();
        if (datasize > 0) {
            int i = (*audio->androidSimpleBufferQueue)->Enqueue(
                    audio->androidSimpleBufferQueue,
                    (char *) audio->buffer,
                    datasize);
        }

    }
}

int MyAudio::getCurrentSampleRateForOpensles(int sample_rate) {
    int rate = 0;
    switch (sample_rate) {
        case 8000:
            rate = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            rate = SL_SAMPLINGRATE_11_025;
            break;
        case 12000:
            rate = SL_SAMPLINGRATE_12;
            break;
        case 16000:
            rate = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            rate = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            rate = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            rate = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            rate = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            rate = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            rate = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            rate = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            rate = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            rate = SL_SAMPLINGRATE_192;
            break;
        default:
            rate = SL_SAMPLINGRATE_44_1;
    }
    return rate;
}

void MyAudio::init() {
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);
    SLresult result;
    slCreateEngine(&engineObj, 0, 0, 0, 0, 0);

    (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engineEngine);

    const SLInterfaceID mid[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputObj, 1, mid, mreq);
    result = (*outputObj)->Realize(outputObj, SL_BOOLEAN_FALSE);
    result = (*outputObj)->GetInterface(outputObj, SL_IID_ENVIRONMENTALREVERB, &outputItf);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputItf)->SetEnvironmentalReverbProperties(outputItf, &outputSlSettings);
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputObj};
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            static_cast<SLuint32>(getCurrentSampleRateForOpensles(sample_rate)),
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource slDataSource = {&android_queue, &pcm};
    SLDataSink audioSink = {&outputMix, NULL};
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmObj, &slDataSource, &audioSink,
                                                3, ids, req);

    (*pcmObj)->Realize(pcmObj, SL_BOOLEAN_FALSE);
    (*pcmObj)->GetInterface(pcmObj, SL_IID_PLAY, &pcmPlay);
    (*pcmObj)->GetInterface(pcmObj, SL_IID_BUFFERQUEUE, &androidSimpleBufferQueue);
    (*androidSimpleBufferQueue)->RegisterCallback(androidSimpleBufferQueue, pcmBufferCallback,
                                                  this);

    (*pcmObj)->GetInterface(pcmObj, SL_IID_VOLUME, &pcmVolume);
    (*pcmPlay)->SetPlayState(pcmPlay, SL_PLAYSTATE_PLAYING);
    pcmBufferCallback(androidSimpleBufferQueue, this);

}
