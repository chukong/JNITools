#ifndef __JAVA_DISPATCHER_H_
#define __JAVA_DISPATCHER_H_


#include "jni.h"
#include <string>

#define SERVICE_REGISTRY_CLASS "org/cocos2dx/csc/ServicesRegistry"

namespace sdkbox {

    class JavaDispatcher {

    private:
        JavaDispatcher();
        JavaDispatcher( const JavaDispatcher& );
        ~JavaDispatcher();
        JavaDispatcher& operator=(const JavaDispatcher&);

    public:

        static jobject _javaServicesRegistry;

        static void __dispatch(
                        const char* serviceCanonicalJavaClass,
                        const char* methodName,
                        jobjectArray args,
                        const char* servicesRegistryMethodName );

        static void dispatch(
                        const char* serviceCanonicalJavaClass,
                        const char* methodName,
                        jobjectArray args );

        static void dispatchStatic(
                        const char* className,
                        const char* methodName,
                        jobjectArray args );


        static jobject      NewInteger( JNIEnv* env, int v );
        static jobject      NewLong( JNIEnv* env, long v );
        static std::string  NewStringFromJString( JNIEnv*, jstring jstr );
    };

}

#endif