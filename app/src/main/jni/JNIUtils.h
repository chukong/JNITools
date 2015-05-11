#ifndef __JNIUTILS_H
#define __JNIUTILS_H

#include "jni.h"
#include "javaenv.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace sdkbox {

class JNIArray;

typedef std::shared_ptr<JNIArray>       SPJNIArray;
typedef std::shared_ptr<const JNIArray> CSPJNIArray;

class JNIArray {

private:
    jobjectArray _array= 0;
    int          _size= 0;
    int          _currentIndex= 0;

    JNIEnv* __getEnv();

public:
    JNIArray( JNIEnv* env, int size );
    JNIArray( JNIEnv* env, int size, const char* type );

    ~JNIArray();

    static SPJNIArray NewFromStringVector( JNIEnv* env, const vector<std::string>& strings );
    static SPJNIArray NewFromCharPtrArray( JNIEnv* env, const char** strings, int numStrings );
    static SPJNIArray NewFromCharPtrArrayV(JNIEnv* env, ... );
    static SPJNIArray NewFromIntVector(    JNIEnv* env, const vector<int>& ints );

    JNIArray& addString(    JNIEnv* env, const std::string& str );
    JNIArray& addString(    JNIEnv* env, const char* str );
    JNIArray& addInt(       JNIEnv* env, int i );
    JNIArray& addLong(      JNIEnv* env, long l );
    JNIArray& addBoolean(   JNIEnv* env, bool b );

    jobjectArray get() const;

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

    static jobject NewInteger(          JNIEnv* env, int v );
    static jobject NewLong(             JNIEnv* env, long v );
    static string NewStringFromJString( JNIEnv* env, jstring jstr );
    static SPJNIArray NewArray(         JNIEnv* env, int size );

    static int getIntValue(             JNIEnv* env, jobject obj );

    static jobjectArray EmptyArray( JNIEnv* env );
};


}


#endif