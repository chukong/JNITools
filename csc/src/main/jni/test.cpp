
#include "javaenv.h"
#include "JavaDispatcher.h"
#include "NativeBridge.h"
#include "Proxy.h"
#include "JNIUtils.h"
#include "GoogleAnalyticsProxy.h"

using namespace sdkbox;

class GA  {
public:

   void A( const std::string& event, jobject params ) {

        JNIEnv *env= __getJNIEnv();

        std::string rec= JNIUtils::NewStringFromJString( env, (jstring)params );

        LOGD("GA callback event:%s recv:%s", event.c_str(), rec.c_str() );
   }

};

extern "C" {

    void JNICALL test4(JNIEnv* env, jobject thiz) {

        GoogleAnalyticsProxy* sp= new GoogleAnalyticsProxy();
        sp->logScreen("Screen test jni 4");

        LOGD("calling getInt %d", sp->getInt());
        LOGD("calling getLong %ld", sp->getLong());
        LOGD("calling getBoolean %d", sp->getBoolean());
        LOGD("calling getString %s", sp->getString().c_str());
    }

    void JNICALL test3(JNIEnv* env, jobject thiz) {

        SPProxy p= Proxy::New( 
            "org/cocos2dx/example/TestProxy",
            JNIArray::NewFromCharPtrArrayV( env, "jaja", "jeje", NULL ).get() );

        // String m1();
        jobject obj= p->invoke("m1");
            LOGD("Result from proxy m1: %s", JNIUtils::NewStringFromJString( env, (jstring)obj ).c_str());
            env->DeleteLocalRef( obj );

        // void m2()
        obj= p->invoke("m2");

        obj= p->invoke("m3", JNIArray(env).addString("str").addInt(23).get() );
//            LOGD("Result from proxy m3: %d", JNIUtils.getIntValue(obj) );
            env->DeleteLocalRef( obj );
    }

    void JNICALL test2(JNIEnv* env, jobject thiz) {

        // BUGBUG leaking. in other examples, keep the GA instance and SPCallback safe.
        SPCallback sp= NativeBridge<GA>::AddEventListener( "GoogleAnalytics.Tick", new GA(), &GA::A );

        typedef struct {
            SPCallback sp;
        } ss;
        ss* s= new ss();
        s->sp= sp;
    }

    void JNICALL test1(JNIEnv* env, jobject thiz) {

        const char* serviceClass= "org/cocos2dx/services/GoogleAnalytics";

        JavaDispatcher::callInService(
                serviceClass,
                "logScreen",
                JNIArray::NewFromCharPtrArrayV( env, "hellou", NULL ).get() );


        JavaDispatcher::callInService(
                serviceClass,
                "logScreen2",
                JNIArray::NewFromCharPtrArrayV( env, "hellou", "3-405683-083530-84", NULL ).get() );

        JavaDispatcher::callInService(
                serviceClass,
                "logScreen3",
                JNIArray( env ).
                            addString("str0").
                            addString("str1").
                            addInt(5).
                            get() );

        JavaDispatcher::callStatic(
            "org/cocos2dx/csc/activity/CSC",
            "test_call",
            JNIArray::NewFromCharPtrArrayV( env, "calling", "CSC::test_call static method.", NULL ).get() );

    }

    JNIEXPORT
    void JNICALL Java_org_cocos2dx_csc_activity_CSC_testSendMessages(JNIEnv* env, jobject thiz) {
        test1( env, thiz );
        test2( env, thiz );
        test3( env, thiz );
        test4( env, thiz );
    }

}