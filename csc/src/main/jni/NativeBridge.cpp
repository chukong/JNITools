#include "javaenv.h"
#include "JavaDispatcher.h"
#include "NativeBridge.h"
#include "JNIUtils.h"

using namespace std;

namespace sdkbox {

extern "C" {
    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_NativeBridge_emit(JNIEnv* env, jobject thiz, jlong objPtr, jstring event, jobject params ) {
        if ( objPtr!=0 ) {

            Functor* sp= reinterpret_cast<Functor*>(objPtr);
            std::string sevent= JNIUtils::NewStringFromJString( env, event );
            (*sp)( sevent, params );

        }
    }
}


}