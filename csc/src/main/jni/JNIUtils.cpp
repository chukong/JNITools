#include "jni.h"
#include "JNIUtils.h"

#include <string>
#include <vector>

#define MAX_ARGS 10

using namespace std;

namespace sdkbox {

jobjectArray JNIUtils::EMPTY_ARRAY= 0;

jobject JNIUtils::NewInteger( JNIEnv* env, int v ) {
    jclass typeClass= env->FindClass("java/lang/Integer");
    jobject obj= env->NewObject( typeClass, env->GetMethodID( typeClass, "<init>", "(I)V"), jint(v) );
    env->DeleteLocalRef( typeClass );

    return obj;
}

jobject JNIUtils::NewLong( JNIEnv* env, long v ) {

    jclass typeClass= env->FindClass("java/lang/Long");
    jobject obj= env->NewObject( typeClass, env->GetMethodID( typeClass, "<init>", "(J)V"), jlong(v) );
    env->DeleteLocalRef( typeClass );

    return obj;
}

jobject JNIUtils::NewBoolean( JNIEnv* env, bool v ) {

    jclass typeClass= env->FindClass("java/lang/Boolean");
    jobject obj= env->NewObject( typeClass, env->GetMethodID( typeClass, "<init>", "(Z)V"), jboolean(v) );
    env->DeleteLocalRef( typeClass );

    return obj;
}

std::string JNIUtils::NewStringFromJString( JNIEnv* env, jstring jstr ) {

    const char* chars = env->GetStringUTFChars(jstr, NULL);
    std::string str(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return str;
}

int JNIUtils::getIntValue( JNIEnv* env, jobject obj ) {

}

jobjectArray JNIUtils::EmptyArray( JNIEnv* env ) {
    if (!EMPTY_ARRAY) {
        jclass _objClass= env->FindClass("java/lang/Object");
        EMPTY_ARRAY= env->NewObjectArray(0, _objClass, NULL);
        env->DeleteLocalRef( _objClass );
    }

    return EMPTY_ARRAY;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


JNIArray JNIArray::NewFromStringVector( JNIEnv* env, const vector<std::string>& strings ) {

    JNIArray array(env);
    for( vector<string>::const_iterator i= strings.begin(); i!=strings.end(); ++i ) {
        array.addString( (*i).c_str() );
    }

    return array;
}

JNIArray JNIArray::NewFromCharPtrArrayV( JNIEnv* env, ... ) {

    vector<const char*> elems;

    int argCount= 0;
    va_list vl;
    va_start(vl,env);
    do {
        const char* el= va_arg(vl,const char*);
        if ( el ) {
            elems.push_back( el );
        } else {
            break;
        }

        argCount++;
        if ( argCount>MAX_ARGS ) {
            LOGD("NewFromCharPtrArrayV with more than %d arguments. ??!?!?!?!?", MAX_ARGS );
        }
    } while( 1 );
    va_end(vl);

    JNIArray array(env);
    for( vector<const char*>::const_iterator i= elems.begin(); i!=elems.end(); ++i ) {
        array.addString( *i );
    }

    return array;

}

JNIArray JNIArray::NewFromCharPtrArray( JNIEnv* env, const char** strings, int numStrings ) {

    JNIArray array(env);
    for( int i=0; i<numStrings; i++ ) {
        array.addString( strings[i] );
    }

    return array;
}

JNIArray::JNIArray() : JNIArray(NULL) {
}

JNIArray::JNIArray( JNIEnv* env ) : JNIArray(env,"Ljava/lang/Object;") {
}

JNIArray::JNIArray( JNIEnv* env, const char* type ) {

    _env= __getEnv(env);
}

JNIArray& JNIArray::addString( const std::string& sstr ) {
    jstring str= _env->NewStringUTF(sstr.c_str());
    _elems.push_back(str);
    return *this;
}

JNIArray& JNIArray::addString( const char* sstr ) {
    jstring str= _env->NewStringUTF(sstr);
    _elems.push_back(str);
    return *this;
}

JNIArray& JNIArray::addInt( int i ) {
    _elems.push_back( JNIUtils::NewInteger(_env, i) );
    return *this;
}

JNIArray& JNIArray::addLong( long l ) {
    _elems.push_back(JNIUtils::NewLong(_env, l) );
    return *this;
}

JNIArray& JNIArray::addBoolean(   bool b ) {
    _elems.push_back(JNIUtils::NewBoolean(_env, b) );
    return *this;
}

JNIEnv* JNIArray::__getEnv( JNIEnv* e ) {
    if ( !e ) {
        e= __getJNIEnv();
    }

    return e;
}

JNIArray::~JNIArray() {
    if ( _array ) {
        __getJNIEnv()->DeleteGlobalRef( _array );
        _array= 0;
    }
}

jobjectArray JNIArray::get() {

    if ( _array ) {
        _env->DeleteGlobalRef( _array );
    }

    jclass _objClass= _env->FindClass("java/lang/Object");
    _array= (jobjectArray)_env->NewGlobalRef( _env->NewObjectArray( _elems.size(), _objClass, 0 ) );
    _env->DeleteLocalRef(_objClass);

    int pos= 0;
    for( vector<jobject>::const_iterator i= _elems.begin(); i!=_elems.end(); ++i ) {
        _env->SetObjectArrayElement( _array, pos, (*i) );
        pos++;
    }

    return _array;
}

}