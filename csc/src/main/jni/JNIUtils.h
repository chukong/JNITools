#ifndef __JNIUTILS_H
#define __JNIUTILS_H

#include "jni.h"
#include "javaenv.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace sdkbox {


/**
 * JNIArray helper objects are supposed to be short-lived objets.
 * Their only motivation is to create a jobjectArray of elements, which should be immediately passed
 * to a method call and discarded.
 *
 * WARNING: don't reuse JNIArray objects, the kept JNIEnv* object could be in the wrong state by the
 *          time you decided to reuse the object.
 */
class JNIArray {

private:
    jobjectArray    _array= 0;
    int             _currentIndex= 0;
    vector<jobject> _elems;
    JNIEnv*         _env;

public:
    JNIArray( );
    JNIArray( JNIEnv* env );
    JNIArray( JNIEnv* env, const char* type );

    ~JNIArray();

    static JNIArray NewFromStringVector( JNIEnv* env, const vector<std::string>& strings );
    static JNIArray NewFromCharPtrArray( JNIEnv* env, const char** strings, int numStrings );
    static JNIArray NewFromCharPtrArrayV(JNIEnv* env, ... );

    JNIArray& addString(    const std::string& str );
    JNIArray& addString(    const char* str );
    JNIArray& addInt(       int i );
    JNIArray& addLong(      long l );
    JNIArray& addBoolean(   bool b );

    jobjectArray get();

    JNIEnv* __getEnv( JNIEnv* env );
};


class JNIUtils {

private:
    JNIUtils();
    JNIUtils( const JNIUtils& );
    JNIUtils& operator=(const JNIUtils& other);
    ~JNIUtils();

    static jobjectArray EMPTY_ARRAY;
public:

    static jobject  NewInteger(           JNIEnv* env, int v );
    static jobject  NewLong(              JNIEnv* env, long v );
    static jobject  NewBoolean(           JNIEnv* env, bool v );
    static string   NewStringFromJString( JNIEnv* env, jstring jstr );

    static int      GetInt(               JNIEnv* env, jobject o );
    static long     GetLong(              JNIEnv* env, jobject o );
    static bool     GetBoolean(           JNIEnv* env, jobject o );

    static jobjectArray EmptyArray(       JNIEnv* env );
};


}


#endif