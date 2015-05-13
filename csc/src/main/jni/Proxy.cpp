#include "javaenv.h"
#include "JavaDispatcher.h"
#include "Proxy.h"
#include "JNIUtils.h"

using namespace sdkbox;

namespace sdkbox {

Proxy::Proxy() : Proxy(NULL) {
}

Proxy::Proxy( const char* clazz ) : _objectReference(0) {
    if ( clazz ) {
        _class= strdup(clazz);
    }
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


jobject Proxy::invoke( const char* method ) const {
    return invoke( method, jobjectArray(0) );
}

jobject Proxy::invoke( const char* method, const jobjectArray params ) const {

    if ( _objectReference ) {
        return JavaDispatcher::callInInstance( _objectReference, method, params );
    }

    LOGD("Invoking method %s with no object instance.", method);

    return NULL;
}

int     Proxy::invokeInt( const char* method, const jobjectArray args ) const {
    jobject obj= invoke( method, args );
    if ( obj ) {
        return JNIUtils::GetInt(__getJNIEnv(), obj);
    }

    return 0;
}

long    Proxy::invokeLong( const char* method, const jobjectArray args ) const {
    jobject obj= invoke( method, args );
    if ( obj ) {
        return JNIUtils::GetLong(__getJNIEnv(), obj);
    }

    return 0L;
}

bool    Proxy::invokeBoolean( const char* method, const jobjectArray args ) const {
    jobject obj= invoke( method, args );
    if ( obj ) {
        return JNIUtils::GetBoolean(__getJNIEnv(), obj);
    }

    return false;
}

string  Proxy::invokeString( const char* method, const jobjectArray args ) const {
    jobject obj= invoke( method, args );
    if ( obj ) {
        return JNIUtils::NewStringFromJString(__getJNIEnv(), (jstring)obj);
    }

    return "";
}





jobject Proxy::invoke( const char* method, int i ) const {
    return invoke( method, JNIArray().addInt(i) );
}

jobject Proxy::invoke( const char* method, bool b ) const {
    return invoke( method, JNIArray().addBoolean(b) );
}

jobject Proxy::invoke( const char* method, const std::string& s ) const {
    return invoke( method, JNIArray().addString(s) );
}

jobject Proxy::invoke( const char* method, const char * s ) const {
    return invoke( method, JNIArray().addString(s) );
}

jobject Proxy::invoke( const char* method, JNIArray& params ) const {
    return invoke( method, params.get() );
}

ServiceProxy::ServiceProxy( const char* serviceClass ) {
    _objectReference= (jobject)__getJNIEnv()->NewGlobalRef( JavaDispatcher::getService( serviceClass ) );
    if ( _objectReference ) {
        LOGD("Got service reference for class: %s.", serviceClass );
    } else {
        LOGD("ERROR: NO service reference for class: %s.", serviceClass );
    }
}

ServiceProxy::~ServiceProxy() {
}


}