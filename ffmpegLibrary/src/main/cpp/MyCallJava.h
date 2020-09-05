//
// Created by Administrator on 2020/9/5.
//

#ifndef MYFFMPEGDEMO3_MYCALLJAVA_H
#define MYFFMPEGDEMO3_MYCALLJAVA_H

#include "jni.h"
#define MAIN_THREAD 0
#define CHILD_THREAD 1

class MyCallJava {
public:
    JavaVM *jvm;
    JNIEnv *jenv;
    jobject jobj;
    jclass jclz;
    jmethodID jmid;
public:
    MyCallJava(JavaVM *vm, JNIEnv *env, jobject obj);

    ~MyCallJava();

    void callJava(int type);
};


#endif //MYFFMPEGDEMO3_MYCALLJAVA_H
