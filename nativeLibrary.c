#include "nativeLibrary.h"
#include "program.h"
#include "model.h"

#define TRUE 1
#define FALSE 0

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUpdate(JNIEnv *env, jobject object) {
	executeProgram();
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callLightBarrierBlocked(JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	return lightBarrier->isBlocked;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherUpperBlocked(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->isFrontTriggerActivated;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherLowerBlocked(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->isBackTriggerActivated;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningBackwards(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == BACKWARDS;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningForwards(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == FORWARDS;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherInactive(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == INACTIVE;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callToolRunning(JNIEnv *env, jobject object, jint id) {
	Tool *tool = resolveTool(id);
	
	return tool->isRunning;
}


JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callTreadmillRunning(JNIEnv *env, jobject object, jint id) {
	Treadmill *treadmill = resolveTreadmill(id);
	
	return treadmill->isRunning;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockUpperTrigger(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isFrontTriggerActivated = TRUE;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockUpperTrigger(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isFrontTriggerActivated = FALSE;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLowerTrigger(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isBackTriggerActivated = TRUE;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLowerTrigger(JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isBackTriggerActivated = FALSE;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLightBarrier(JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	lightBarrier->isBlocked = TRUE;
}


JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLightBarrier(JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	lightBarrier->isBlocked = FALSE;
}
