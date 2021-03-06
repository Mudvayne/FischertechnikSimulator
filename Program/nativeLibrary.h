/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class de_hm_cs_embedded_simulator_model_NativeLibrary */

#ifndef _Included_de_hm_cs_embedded_simulator_model_NativeLibrary
#define _Included_de_hm_cs_embedded_simulator_model_NativeLibrary
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callUpdate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUpdate
  (JNIEnv *, jobject);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callLightBarrierBlocked
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callLightBarrierBlocked
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callPusherUpperBlocked
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherUpperBlocked
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callPusherLowerBlocked
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherLowerBlocked
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callPusherRunningBackwards
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningBackwards
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callPusherRunningForwards
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningForwards
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callPusherInactive
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherInactive
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callToolRunning
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callToolRunning
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callTreadmillRunning
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callTreadmillRunning
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callBlockUpperTrigger
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockUpperTrigger
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callUnblockUpperTrigger
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockUpperTrigger
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callBlockLowerTrigger
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLowerTrigger
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callUnblockLowerTrigger
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLowerTrigger
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callBlockLightBarrier
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLightBarrier
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callUnblockLightBarrier
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLightBarrier
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callSetSiteState
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callSetSiteState
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callGetSiteState
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callGetSiteState
  (JNIEnv *, jobject);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callGetErrorCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callGetErrorCode
  (JNIEnv *, jobject);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callHandleTreadmill
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandleTreadmill
  (JNIEnv *, jobject, jint, jboolean);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callHandleTool
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandleTool
  (JNIEnv *, jobject, jint, jboolean);

/*
 * Class:     de_hm_cs_embedded_simulator_model_NativeLibrary
 * Method:    callHandlePusher
 * Signature: (IZZZ)V
 */
JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandlePusher
  (JNIEnv *, jobject, jint, jboolean, jboolean, jboolean);

#ifdef __cplusplus
}
#endif
#endif
