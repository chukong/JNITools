#ifndef __PROXY_X
#define __PROXY_h

#include "jni.h"
#include <memory>

using namespace std;

namespace sdkbox {

class Proxy;
typedef shared_ptr<Proxy>       SPProxy;
typedef shared_ptr<const Proxy> CSPProxy;

class Proxy {
protected:

    jobject     _objectReference;
    const char* _class;

    Proxy( const char* clazz );

    void init( jobjectArray constructorParams );

public:
    ~Proxy();

    jobject invoke( const char* method, const jobjectArray args );

    static SPProxy New( const char* clazz, jobjectArray constructorParams );
};


}

#endif