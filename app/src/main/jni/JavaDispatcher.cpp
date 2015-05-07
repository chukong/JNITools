#include "javaenv.h"
#include "JavaDispatcher.h"

namespace sdkbox {

static jobject _javaServicesRegistry;

extern "C" {
    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_CSC_testSendMessages(JNIEnv* env, jobject thiz) {

        jclass _objClass= env->FindClass("java/lang/Object");
        jstring _str0= env->NewStringUTF("hellou");
        jobjectArray p0= env->NewObjectArray( 1, _objClass, _str0 );

        JavaDispatcher::dispatch(
                "org/cocos2dx/services/GoogleAnalytics",
                "logScreen",
                p0 );


        jstring _str1= env->NewStringUTF("3-405683-083530-84");
        jobjectArray p1= env->NewObjectArray( 2, _objClass, _str0 );
        env->SetObjectArrayElement( p1, 1, _str1);

        JavaDispatcher::dispatch(
                "org/cocos2dx/services/GoogleAnalytics",
                "logScreen2",
                p1 );

        jobjectArray p2= env->NewObjectArray( 3, _objClass, _str0 );
        env->SetObjectArrayElement( p2, 1, _str1);

        jclass integerClass= env->FindClass("java/lang/Integer");
        jobject integer= env->NewObject(
            integerClass,
            env->GetMethodID( integerClass, "<init>", "(I)V"),
            5
        );
        env->SetObjectArrayElement( p2, 2, integer );

        JavaDispatcher::dispatch(
                "org/cocos2dx/services/GoogleAnalytics",
                "logScreen3",
                p2 );

        JavaDispatcher::dispatchStatic(
            "org/cocos2dx/csc/CSC",
            "test_call",
            p1 );

        env->DeleteLocalRef(p0);
        env->DeleteLocalRef(_objClass);
    }

    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_ServicesRegistry_nativeInit(JNIEnv* env, jobject thiz) {
        _javaServicesRegistry= env->NewGlobalRef( thiz );
        LOGD("ServiceRegistry reference set.");
    }

}

void JavaDispatcher::dispatchStatic(const char* sclass,
                                    const char* smethodName,
                                    jobjectArray args ) {

    __dispatch( sclass, smethodName, args, "dispatchStaticMessage" );
}

void JavaDispatcher::dispatch(  const char* sclass,
                                const char* smethodName,
                                jobjectArray args ) {

    __dispatch( sclass, smethodName, args, "dispatchMessage" );
}

void JavaDispatcher::__dispatch(const char *serviceCanonicalJavaClass,
                                const char *smethodName,
                                jobjectArray args,
                                const char* servicesRegistryMethod ) {

    JNIEnv *pEnv = __getJNIEnv();
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

                jobject obj= pEnv->CallObjectMethod( _javaServicesRegistry, _methodId, serviceClassName, methodName, args );
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

}

}