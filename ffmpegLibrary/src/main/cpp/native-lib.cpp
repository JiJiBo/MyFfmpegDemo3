#include <jni.h>
#include <string>
#include "AndroidLog.h"
#include "MyQueue.h"
#include "MyCallJava.h"
#include "MyFfmpeg.h"

_JavaVM *javaVM = NULL;

MyFfmpeg *ffmpeg = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {

        return result;
    }
    return JNI_VERSION_1_4;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_zcf_ffmpeglibrary_FfmpegDemo_prepare(JNIEnv *env, jobject thiz, jstring url) {
    if (ffmpeg == NULL) {

        MyCallJava *pJava = new MyCallJava(javaVM, env, thiz);

        ffmpeg = new MyFfmpeg(pJava, env->GetStringUTFChars(url, 0));

        ffmpeg->prepare();
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_zcf_ffmpeglibrary_FfmpegDemo_start(JNIEnv *env, jobject thiz) {
    if (ffmpeg != NULL) {
        try {
            ffmpeg->start();
        } catch (std::exception &e) {
            LOGE(" exception")
        } catch (std::runtime_error &e) {
            LOGE(" runtime_error")
        } catch (std::overflow_error &e) {
            LOGE(" runtime_error")
        } catch (std::underflow_error &e) {
            LOGE(" runtime_error")
        }
    }
}