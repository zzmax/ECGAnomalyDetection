LOCAL_PATH := $(call my-dir)
MY_SOURCES := noiseTransform.c
MY_SOURCES += asSignal.c
MY_SOURCES += nearKmean.c
MY_SOURCES += diffKmean.c
MY_SOURCES += asSignal_initialize.c
MY_SOURCES += asSignal_emxAPI.c
MY_SOURCES += asSignal_emxutil.c
MY_SOURCES += rt_nonfinite.c
MY_SOURCES += rtGetInf.c
MY_SOURCES += rtGetNaN.c
include $(CLEAR_VARS)
LOCAL_MODULE    := transformNoise
LOCAL_SRC_FILES := $(MY_SOURCES)
include $(BUILD_SHARED_LIBRARY)