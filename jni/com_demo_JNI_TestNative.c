#include <stdio.h>
#include "com_demo_JNI_TestNative.h"

/*
 * Class:     com_demo_JNI_TestNative
 * Method:    print
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_demo_JNI_TestNative_print(JNIEnv *env, jobject jobj)
{
	printf("this is my first jni test\n");
		
}

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
