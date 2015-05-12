#ifndef __PROXY_SERVICE_PROXY_H
#define __PROXY_SERVICE_PROXY_H

#include "jni.h"
#include <memory>

using namespace std;

namespace sdkbox {

class Proxy;
typedef shared_ptr<Proxy>       SPProxy;
typedef shared_ptr<const Proxy> CSPProxy;

/**
 * A Proxy is a native object that holds a Java object reference of a given type.
 * The Proxy is built by supplying a canonical class name, and a set of parameters that will be
 * passed to the suitable Java Class constructor.
 * The constructor is inferred based on the parameter types.
 * The result of this constructor invocation will be kept as the Java reference.
 * Successive calls to invoke will invoke a the associated Java method for the reference class
 * with the supplied parameters.
 * The return value will be NULL if either the method returns void or an error has occurred.
 *
 * The way to build a Proxy object is by using its static New method like:
 * <code>
 *  Proxy::New( "com/cocos2dx/example/TestProxy", ... );
 * </code>
 *
 * It is expected the developer will extends this Class with his own proxy types, where each method
 * will internally call invoke to proxy the Java object's function.
 */
class Proxy {
protected:

    jobject     _objectReference;
    const char* _class;

    Proxy();
    Proxy( const char* clazz );

    virtual void init( jobjectArray constructorParams );

public:
    virtual ~Proxy();

    virtual jobject invoke( const char* method ) const;
    virtual jobject invoke( const char* method, int i ) const;
    virtual jobject invoke( const char* method, bool b ) const;
    virtual jobject invoke( const char* method, const std::string& s ) const;
    virtual jobject invoke( const char* method, const char* s ) const;
    virtual jobject invoke( const char* method, const jobjectArray args ) const;

    static SPProxy New( const char* clazz, jobjectArray constructorParams );
};

class ServiceProxy : public Proxy {

public:
    ServiceProxy( const char* serviceClass );
    virtual ~ServiceProxy();

};

}

#endif