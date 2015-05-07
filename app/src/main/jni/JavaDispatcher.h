#ifndef __JAVA_DISPATCHER_CPP
#define __JAVA_DISPATCHER_CPP

#include "jni.h"

using namespace std;

#define SERVICE_REGISTRY_CLASS "org/cocos2dx/csc/ServicesRegistry"

namespace sdkbox {

    class JavaDispatcher {

    private:
        JavaDispatcher();
        JavaDispatcher( const JavaDispatcher& );
        ~JavaDispatcher();
        JavaDispatcher& operator=(const JavaDispatcher&);

    public:

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
    };

}

#endif