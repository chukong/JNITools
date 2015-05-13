#ifndef __NATIVE_BRIDGE_H
#define __NATIVE_BRIDGE_H

#include <memory>
#include "jni.h"
#include "JavaDispatcher.h"
#include "JNIUtils.h"


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

    /**
     * This class is a Booch utility with the responsibility of registering native callbacks in Java.
     * This is a common blueprint across all Cocos Service Center, where Java-side plugins need to
     * marshall Java events to native.
     * The mechanism is very simple:
     *   * An event name is associated with a native side functor object.
     *   * then, calling from Java the method
     *     <code>ServicesRegistry.emit( String event, Object params )</code> will invoke the functor.
     *
     * This is much like an observer pattern, where all functors associated with the same Event will
     * be invocated.
     *
     * This object has method to only register and unregister callbacks:
     *  <code>static void addEventListener( const char* event, SPCallback* callback );</code>
     *  <code>static void removeEventListener( const char* event, SPCallback* callback );</code>
     * Warning: make the native callbacks fast since they will all be invoked in a row.
     */
     template < class T >
    class NativeBridge {

    private:
        NativeBridge();
        ~NativeBridge();
        NativeBridge(NativeBridge&);
        NativeBridge& operator=(NativeBridge&);
        NativeBridge& operator=(NativeBridge*);



static void addEventListener( const char* event, const Functor* listener ) {

    JNIEnv *pEnv = __getJNIEnv();
    if ( pEnv ) {

        jclass      _classId=   pEnv->FindClass(SERVICE_REGISTRY_CLASS);
        jmethodID   _methodId=  0;

        if ( _classId ) {
            _methodId=  pEnv->GetMethodID(
                    _classId,
                    "addEventListener",
                    "(Ljava/lang/String;J)V" );

            if ( _methodId ) {

                jstring js_event=   pEnv->NewStringUTF( event );
                jlong   ptr=        jlong(listener);

                pEnv->CallVoidMethod( JavaDispatcher::_javaServicesRegistry, _methodId, js_event, ptr );
                CHECK_AND_CLEAR_EXCEPTION(pEnv)

                pEnv->DeleteLocalRef( js_event );

            } else {

                CHECK_AND_CLEAR_EXCEPTION(pEnv)
                LOGD("Failed to get method: ServicesRegistry.addEventListener.");
            }
        } else {

            CHECK_AND_CLEAR_EXCEPTION(pEnv)
            LOGD("Failed to get class id: %s.", SERVICE_REGISTRY_CLASS);
        }

        if ( _classId ) {
            pEnv->DeleteLocalRef( _classId );
        }

    } else {
        LOGD("Failed to get jni env on addEventListener.");
    }
}

    public:

 static void RemoveEventListener( const char* event, const Functor* listener ) {
    LOGD("Remove event listener called.");
}

        static SPCallback AddEventListener( const char* event, T* obj, void(T::*method)(const string&, jobject)  ) {
            Functor *f= new JNIEventListener<T>( obj, method );
            addEventListener( event, f );
            return SPCallback(f);
        }
    };



}

#endif