LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include /Applications/eclipse/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := \
	NativeProcessingComponent

LOCAL_SRC_FILES := \
	Constants.hpp AuxiliaryClasses/Component.hpp \
	AuxiliaryClasses/Board.hpp \
	AuxiliaryClasses/Box.hpp \
	AuxiliaryClasses/Storage.hpp \
	AuxiliaryClasses/TypeValidator.hpp \
	Functions/ProcessingFunctions.hpp \
	Functions/FileReadingFunctions.hpp \
	Functions/GeometryFunctions.hpp \
	JNI_Functions/JNI_Functions.hpp \
	Functions/ProcessingFunctions.cpp \
	JNI_Functions/ColorCorrection.cpp \
	JNI_Functions/Detection.cpp \
	JNI_Functions/ColorMeasurement.cpp \
	Functions/GeometryFunctions.cpp 
	
LOCAL_LDLIBS += \
	-llog \
	-ldl
	
include $(BUILD_SHARED_LIBRARY)
