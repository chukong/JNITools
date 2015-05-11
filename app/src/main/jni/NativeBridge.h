#ifndef __NATIVE_BRIDGE_H
#define __NATIVE_BRIDGE_H

#include "jni.h"
#include <memory>

using namespace std;

namespace sdkbox {

    class Functor {
    public:
        virtual void operator()( const string& event, jobject params ) {
        }
    };

    template< class T >
    class JNIEventListener : public Functor {

        typedef void(T::*Method)(const string& event, jobject params );

    private:
        T*  _callee;
        Method _method;

    public:

        JNIEventListener( T* obj, Method method ) : _callee(obj), _method(method)
        {}

        virtual void operator()( const string& event, jobject params ) {

            if ( _callee ) {
                (_callee->*_method)(event, params);
            }
        }
    };

    typedef shared_ptr<Functor>         SPCallback;
    typedef shared_ptr<const Functor>   CSPCallback;

    class NativeBridge {

    private:
        NativeBridge();
        ~NativeBridge();
        NativeBridge(NativeBridge&);
        NativeBridge& operator=(NativeBridge&);
        NativeBridge& operator=(NativeBridge*);

    public:

        static void addEventListener( const char* event, SPCallback* callback );
        static void removeEventListener( const char* event, SPCallback* callback );

    };



}

#endif