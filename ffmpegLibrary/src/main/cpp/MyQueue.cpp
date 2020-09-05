//
// Created by Administrator on 2020/9/5.
//

#include <unistd.h>
#include "MyQueue.h"

MyQueue::MyQueue(MyStatue *statue) {
    this->statue = statue;


    pthread_cond_init(&cond, NULL);

    pthread_mutex_init(&mutex, NULL);

}

MyQueue::~MyQueue() {


}

int MyQueue::putAvPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutex);
    queue.push(packet);
//    LOGE("放入队列  队列有%d条数据", queue.size())
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int MyQueue::getAvPacket(AVPacket *packet) {

        pthread_mutex_lock(&mutex);
        while (statue != NULL && !statue->exit) {
            if (queue.size() > 0) {
                AVPacket *pPacket = queue.front();
                if (av_packet_ref(packet, pPacket) == 0) {
                    queue.pop();
                }
                av_packet_free(&pPacket);
                av_free(pPacket);
                pPacket = NULL;
//            LOGE("取出队列  队列有%d条数据", queue.size())
                break;
            } else {
                pthread_cond_wait(&cond, &mutex);
            }
        }
        pthread_mutex_unlock(&mutex);

    return 0;
}

int MyQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutex);
    size = queue.size();
    pthread_mutex_unlock(&mutex);
    return size;
}
