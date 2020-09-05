//
// Created by Administrator on 2020/9/5.
//

#include "MyCallJava.h"
#include "AndroidLog.h"

MyCallJava::MyCallJava(JavaVM *vm, JNIEnv *env, jobject obj) {
    this->jvm = vm;
    this->jenv = env;
    this->jobj = env->NewGlobalRef(obj);
    jclz = env->GetObjectClass(jobj);
    jmid = env->GetMethodID(this->jclz, "havePrepare", "()V");
}

void MyCallJava::callJava(int type) {

    if (type == MAIN_THREAD) {
        jenv->CallVoidMethod(jobj, jmid);
    } else if (type == CHILD_THREAD) {
        jint attachCurrentThread = jvm->AttachCurrentThread(&jenv, NULL);
        if (attachCurrentThread != 0) {
            LOGE("触摸线程失败")
            return;
        }
        jenv->CallVoidMethod(jobj, jmid);
        jvm->DetachCurrentThread();
    }
}
