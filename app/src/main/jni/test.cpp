
#include "javaenv.h"
#include "JavaDispatcher.h"
#include "NativeBridge.h"

using namespace sdkbox;

class GA : public Functor {
public:

   void A( std::string event, jobject params ) {

        JNIEnv *env= __getJNIEnv();

        std::string rec= JavaDispatcher::NewStringFromJString( env, (jstring)params );

        LOGD("GA callback event:%s recv:%s", event.c_str(), rec.c_str() );
   }

};

extern "C" {

    void JNICALL test2(JNIEnv* env, jobject thiz) {

        SPCallback* gg= new SPCallback( new JNIEventListener<GA>( new GA(), &GA::A ) );
        // BUGBUG leaking
        NativeBridge::addEventListener( "GoogleAnalytics.Tick", gg );

    }

    void JNICALL test1(JNIEnv* env, jobject thiz) {
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

        env->SetObjectArrayElement( p2, 2, JavaDispatcher::NewInteger(env,5) );

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
    void JNICALL Java_org_cocos2dx_csc_CSC_testSendMessages(JNIEnv* env, jobject thiz) {
        test1( env, thiz );
        test2( env, thiz );
    }

}