#ifndef __JAVA_ENV_H
#define __JAVA_ENV_H

    #include <android/log.h>
    #include <string.h>
    #include "jni.h"

    #define  LOG_TAG    "ServicesNative"
    #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
    #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

    extern JavaVM* _javaVM;


    extern "C" {
        extern JNIEnv* __getJNIEnv();
    }

    #define CHECK_AND_CLEAR_EXCEPTION(p) if((p)->ExceptionCheck()) { (p)->ExceptionClear(); }

#endif