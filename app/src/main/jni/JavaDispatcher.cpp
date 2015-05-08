#include "javaenv.h"
#include "JavaDispatcher.h"

namespace sdkbox {



extern "C" {

    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_ServicesRegistry_nativeInit(JNIEnv* env, jobject thiz) {
        JavaDispatcher::_javaServicesRegistry= env->NewGlobalRef( thiz );
        LOGD("ServiceRegistry reference set.");
    }

}

jobject JavaDispatcher::_javaServicesRegistry= 0;

jobject JavaDispatcher::dispatchStatic(const char* sclass,
                                    const char* smethodName,
                                    jobjectArray args ) {

    return __dispatch( sclass, smethodName, args, "dispatchStaticMessage" );
}

jobject JavaDispatcher::dispatch(  const char* sclass,
                                const char* smethodName,
                                jobjectArray args ) {

    return __dispatch( sclass, smethodName, args, "dispatchMessage" );
}

jobject JavaDispatcher::__dispatch(const char *serviceCanonicalJavaClass,
                                const char *smethodName,
                                jobjectArray args,
                                const char* servicesRegistryMethod ) {

    JNIEnv *pEnv = __getJNIEnv();
    jobject ret = NULL;

    if ( pEnv ) {

        jclass      _classId=   pEnv->FindClass(SERVICE_REGISTRY_CLASS);
        jmethodID   _methodId=  0;

        if ( _classId ) {
            _methodId=  pEnv->GetMethodID(
                    _classId,
                    servicesRegistryMethod,
                    "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;" );

            if ( _methodId ) {

                jstring serviceClassName=   pEnv->NewStringUTF( serviceCanonicalJavaClass );
                jstring methodName=         pEnv->NewStringUTF( smethodName );

                ret= pEnv->CallObjectMethod( _javaServicesRegistry, _methodId, serviceClassName, methodName, args );
                CHECK_AND_CLEAR_EXCEPTION(pEnv)

                pEnv->DeleteLocalRef( methodName );
                pEnv->DeleteLocalRef( serviceClassName );

            } else {

                CHECK_AND_CLEAR_EXCEPTION(pEnv)
                LOGD("Failed to get method: ServicesRegistry.dispatchMessage.");
            }
        } else {

            CHECK_AND_CLEAR_EXCEPTION(pEnv)
            LOGD("Failed to get class id: %s.", serviceCanonicalJavaClass);
        }

        if ( _classId ) {
            pEnv->DeleteLocalRef( _classId );
        }

    } else {

        LOGD("Failed to get jni env on dispatch.");
    }

    return ret;
}

jobject JavaDispatcher::NewInteger( JNIEnv* env, int v ) {
    jclass typeClass= env->FindClass("java/lang/Integer");
    jobject obj= env->NewObject( typeClass, env->GetMethodID( typeClass, "<init>", "(I)V"), jint(v) );
    env->DeleteLocalRef( typeClass );

    return obj;
}

jobject JavaDispatcher::NewLong( JNIEnv* env, long v ) {

    jclass typeClass= env->FindClass("java/lang/Long");
    jobject obj= env->NewObject( typeClass, env->GetMethodID( typeClass, "<init>", "(J)V"), jlong(v) );
    env->DeleteLocalRef( typeClass );

    return obj;
}

std::string JavaDispatcher::NewStringFromJString( JNIEnv* env, jstring jstr ) {

    const char* chars = env->GetStringUTFChars(jstr, NULL);
    std::string str(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return str;
}

}