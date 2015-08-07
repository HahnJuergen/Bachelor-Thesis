#ifndef JNI_FUNCTIONS_H_
#define JNI_FUNCTIONS_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT jobjectArray JNICALL Java_de_ur_zollner_juergenhahn_ba_Handler_NativeHandler_calibrateNative(JNIEnv *, jobject, jlong, jint, jint, jint, jint, jint, jint, jstring);
	JNIEXPORT jint JNICALL Java_de_ur_zollner_juergenhahn_ba_Handler_NativeHandler_processImplementationNative(JNIEnv *, jobject, jlong, jint);
	JNIEXPORT jint JNICALL Java_de_ur_zollner_juergenhahn_ba_Handler_NativeHandler_processWithdrawalNative(JNIEnv *, jobject, jlong, jint);

	JNIEXPORT jdoubleArray JNICALL Java_de_ur_zollner_juergenhahn_ba_Handler_NativeHandler_measureImageValuesNative(JNIEnv *, jobject, jlong, jdoubleArray);


	static jobjectArray row(JNIEnv *, const jsize, const char **);
	static void uiInfos(jobjectArray *, std::vector<std::vector<std::string>> const &, JNIEnv *);
	static void getUIDisplayalInformation(jobjectArray *, std::vector<std::vector<std::string>> const &, JNIEnv *);

#ifdef __cplusplus
}
#endif
#endif /* JNI_FUNCTIONS_H_ */
