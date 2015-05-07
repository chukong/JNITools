LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := msgbroker

LOCAL_CPPFLAGS  := -std=c++0x

LOCAL_SRC_FILES := JavaDispatcher.cpp

LOCAL_EXPORT_CFLAGS := -Wno-psabi

LOCAL_CFLAGS += -D__GXX_EXPERIMENTAL_CXX0X__

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.9/include \
					$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.9/include/libs/armeabi/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
