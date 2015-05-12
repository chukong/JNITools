#include "javaenv.h"
#include "JavaDispatcher.h"
#include "Proxy.h"
#include "JNIUtils.h"

using namespace sdkbox;

namespace sdkbox {

Proxy::Proxy( const char* clazz ) : _class(clazz), _objectReference(0) {
}

Proxy::~Proxy() {
    if ( _objectReference ) {
        __getJNIEnv()->DeleteGlobalRef( _objectReference );
        _objectReference= 0;
    }
}

SPProxy Proxy::New( const char* clazz, jobjectArray constructorParams ) {
    Proxy* p= new Proxy( clazz );
    p->init( constructorParams );
    return SPProxy(p);
}

void Proxy::init( jobjectArray constructorParams ) {
    _objectReference= JavaDispatcher::create( _class, constructorParams );
    if ( _objectReference ) {
        _objectReference= __getJNIEnv()->NewGlobalRef( _objectReference );
    }
}


jobject Proxy::invoke( const char* method ) {
    return invoke( method, 0 );
}

jobject Proxy::invoke( const char* method, const jobjectArray params ) {

    JavaDispatcher::callInInstance( _objectReference, method, params );
}

}