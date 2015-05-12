#ifndef __JAVA_DISPATCHER_H_
#define __JAVA_DISPATCHER_H_


#include "jni.h"
#include <string>

#define SERVICE_REGISTRY_CLASS "org/cocos2dx/csc/ServicesRegistry"

namespace sdkbox {

    /**
     * The JavaDispatcher is a Booch utility object which knows how to communicate with the Java
     * side JavaDispatcher object.
     * Its main responsibility is to marshall calls from Native to Java and handle results. To do so,
     * the developer has not to deal with Java's native method signatures. These will be derived in
     * Java and upon method call, the resulting object of the Java method invocation will be given
     * as return value.
     *
     * There are 3 methods to communicate with Java side. In all of them, the Java-side method
     * arguments are supplied as a <code>jobjectArray</code>. See <code>JNIUtils</code> for a
     * convenient way of dealing with these objects.
     *
     * <b>callInService</b>
     * This method will invoke a method that belongs to a Java Service object. The service will be
     * identified by its canonical Java class representation. Then, the method name and its arguments
     * must be supplied to the method call.
     * The java method invoked will be inferred from the arguments' signature (class type).
     * Primitive types will, before method inference, be unboxed.
     * If no suitable method exists for the service, or there's an error, or the invoked method
     * is <code>void</code>, null will be returned.
     *
     * <b>callStatic</b>
     * This method will invoke an arbitrary static method in any Class object. This method needs
     * the canonical name of a class, as well as the method name and its invocation arguments.
     * Same restrictions that in <code>callInService</code> apply to this method.
     *
     * <b>callInInstance</b>
     * This method will invoke ant method in a given object instance. The object instance can be
     * obtained using regular JNI calls, passing it from Java to Native, or by using a
     * <code>Proxy</code> object.
     * In this case, the method needs a <code>jobject</code> Java reference, as well as the method
     * name and its arguments.
     * Same restrictions apply to the arguments than in previous methods.
     *
     * Most of the DynamicJNI functionalities will rely on these methods to perform their work.
     *
     * Warning: this class is useless without its Java JavaDispatcher.java object counterpart.
     */
    class JavaDispatcher {

    private:
        JavaDispatcher();
        JavaDispatcher( const JavaDispatcher& );
        ~JavaDispatcher();
        JavaDispatcher& operator=(const JavaDispatcher&);

    public:
        static jobject _javaServicesRegistry;


        static jobject __dispatch(
                        const char* serviceCanonicalJavaClass,
                        const char* methodName,
                        jobjectArray args,
                        const char* servicesRegistryMethodName );

        static jobject callInService(
                        const char* serviceCanonicalJavaClass,
                        const char* methodName,
                        jobjectArray args );

        static jobject callStatic(
                        const char* className,
                        const char* methodName,
                        jobjectArray args );

        static jobject create(
                        const char* className,
                        jobjectArray args );

        static jobject callInInstance(
                        jobject objRef,
                        const char* method,
                        jobjectArray params );

        static jobject getService( const char* serviceClass );
    };

}

#endif