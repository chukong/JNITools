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

SPJNIArray JNIUtils::NewArray( JNIEnv* env, int size ) {
    return shared_ptr<JNIArray>(new JNIArray(env, size));
}


SPJNIArray JNIArray::NewFromStringVector( JNIEnv* env, const vector<std::string>& strings ) {

    SPJNIArray array= JNIUtils::NewArray(env, strings.size() );
    for( vector<string>::const_iterator i= strings.begin(); i!=strings.end(); ++i ) {
        array->addString( env, (*i).c_str() );
    }

    return array;
}

SPJNIArray JNIArray::NewFromCharPtrArrayV( JNIEnv* env, ... ) {

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

    SPJNIArray array= JNIUtils::NewArray(env, elems.size() );
    for( vector<const char*>::const_iterator i= elems.begin(); i!=elems.end(); ++i ) {
        array->addString( env, *i );
    }

    return array;

}

SPJNIArray JNIArray::NewFromCharPtrArray( JNIEnv* env, const char** strings, int numStrings ) {

    SPJNIArray array= JNIUtils::NewArray(env, numStrings );
    for( int i=0; i<numStrings; i++ ) {
        array->addString( env, strings[i] );
    }

    return array;
}

SPJNIArray JNIArray::NewFromIntVector( JNIEnv* env, const vector<int>& ints ) {
    return shared_ptr<JNIArray>( new JNIArray(env, 0));
}

JNIArray::JNIArray( JNIEnv* env, int size ) : JNIArray(env,size,"Ljava/lang/Object;") {
}

JNIArray::JNIArray( JNIEnv* env, int size, const char* type ) : _size(size) {

    jclass _objClass= env->FindClass("java/lang/Object");
    _array= (jobjectArray)env->NewGlobalRef( env->NewObjectArray( size, _objClass, 0 ) );
    env->DeleteLocalRef(_objClass);
}

JNIArray& JNIArray::addString( JNIEnv* env, const std::string& sstr ) {

    env= __getEnv(env);
    jstring str= env->NewStringUTF(sstr.c_str());

    env->SetObjectArrayElement( _array, _currentIndex, str );

    _currentIndex++;

    return *this;
}

JNIArray& JNIArray::addString( JNIEnv* env, const char* sstr ) {

    env= __getEnv(env);
    jstring str= env->NewStringUTF(sstr);

    env->SetObjectArrayElement( _array, _currentIndex, str );

    _currentIndex++;

    return *this;
}

JNIArray& JNIArray::addInt( JNIEnv* env, int i ) {

    env= __getEnv(env);

    env->SetObjectArrayElement( _array, _currentIndex, JNIUtils::NewInteger(env, i) );
    _currentIndex++;

    return *this;
}

JNIArray& JNIArray::addLong( JNIEnv* env, long l ) {

    env= __getEnv(env);

    env->SetObjectArrayElement( _array, _currentIndex, JNIUtils::NewLong(env, l) );
    _currentIndex++;

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

jobjectArray JNIArray::get() const {
    return _array;
}

}