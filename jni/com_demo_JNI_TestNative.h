/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_demo_JNI_TestNative */

#ifndef _Included_com_demo_JNI_TestNative
#define _Included_com_demo_JNI_TestNative
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_demo_JNI_TestNative
 * Method:    print
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_demo_JNI_TestNative_print
  (JNIEnv *, jobject);

/*
 * Class:     com_demo_JNI_TestNative
 * Method:    sum
 * Signature: (DD)D
 */
JNIEXPORT jdouble JNICALL Java_com_demo_JNI_TestNative_sum
  (JNIEnv *, jobject, jdouble, jdouble);

/*
 * Class:     com_demo_JNI_TestNative
 * Method:    setNameByNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_demo_JNI_TestNative_setNameByNative
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif