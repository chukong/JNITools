#ifndef __NATIVE_BRIDGE_H
#define __NATIVE_BRIDGE_H

#include "jni.h"
#include <memory>

using namespace std;

namespace sdkbox {

    class Functor {
    public:
        virtual void operator()( const string event, jobject params ) {
        }
    };

    template< class T >
    class JNIEventListener : public Functor {

    private:
        T*  _callee;
        void(T::*_method)(const string event, jobject params);

    public:

        JNIEventListener( T* obj, void(T::*method)( const string event, jobject params ) ) : _callee(obj), _method(method)
        {}

        virtual void operator()( const string event, jobject params ) {

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