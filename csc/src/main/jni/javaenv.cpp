#include "javaenv.h"

JavaVM* _javaVM;

extern "C" {

    jint JNI_OnLoad(JavaVM* vm, void* reserved) {
        JNIEnv *env;
        _javaVM = vm;

        LOGD("JNI_OnLoad called");
        if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("Failed to get the environment using GetEnv()");
            return -1;
        }

        return JNI_VERSION_1_4;
    }

    JNIEnv* __getJNIEnv() {

        JNIEnv *env= 0;

        if(!_javaVM) {
            LOGD("JavaVM is NULL");
            return 0;
        }

        if (_javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
            LOGD("Failed to get the environment using GetEnv()");
            return 0;
        }

        if (_javaVM->AttachCurrentThread(&env, 0) < 0) {
            LOGD("Failed to get the environment using AttachCurrentThread()");
            return 0;
        }

        return env;
    }
}