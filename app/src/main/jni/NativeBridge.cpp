#include "javaenv.h"
#include "JavaDispatcher.h"
#include "NativeBridge.h"

using namespace std;

namespace sdkbox {

extern "C" {
    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_NativeBridge_emit(JNIEnv* env, jobject thiz, jlong objPtr, jstring event, jobject params ) {
        if ( objPtr!=0 ) {

            SPCallback* sp= reinterpret_cast<SPCallback*>(objPtr);
            std::string sevent= JavaDispatcher::NewStringFromJString( env, event );
            (*(sp->get()))( sevent, params );

        }
    }
}

void NativeBridge::addEventListener( const char* event, SPCallback* listener ) {

    JNIEnv *pEnv = __getJNIEnv();
    if ( pEnv ) {

        jclass      _classId=   pEnv->FindClass(SERVICE_REGISTRY_CLASS);
        jmethodID   _methodId=  0;

        if ( _classId ) {
            _methodId=  pEnv->GetMethodID(
                    _classId,
                    "addEventListener",
                    "(Ljava/lang/String;J)V" );

            if ( _methodId ) {

                jstring js_event=   pEnv->NewStringUTF( event );
                jlong   ptr=        jlong(listener);

                pEnv->CallVoidMethod( JavaDispatcher::_javaServicesRegistry, _methodId, js_event, ptr );
                CHECK_AND_CLEAR_EXCEPTION(pEnv)

                pEnv->DeleteLocalRef( js_event );

            } else {

                CHECK_AND_CLEAR_EXCEPTION(pEnv)
                LOGD("Failed to get method: ServicesRegistry.addEventListener.");
            }
        } else {

            CHECK_AND_CLEAR_EXCEPTION(pEnv)
            LOGD("Failed to get class id: %s.", SERVICE_REGISTRY_CLASS);
        }

        if ( _classId ) {
            pEnv->DeleteLocalRef( _classId );
        }

    } else {
        LOGD("Failed to get jni env on addEventListener.");
    }
}

void NativeBridge::removeEventListener( const char* event, SPCallback* listener ) {
    LOGD("Remove event listener called.");
}

}