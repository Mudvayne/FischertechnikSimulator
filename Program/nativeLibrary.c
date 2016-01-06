#include "nativeLibrary.h"
#include "program.h"
#include "model.h"

#define TRUE 1
#define FALSE 0

SiteState convertFromIntToStateEnum(int32_t state) {
	switch(state) {
		case 0:
		return PANIC_SWITCH;
		case 1:
		return DIAGNOSTIC;
		case 2:
		return START;
		case 3:
		return RUNNING;
		case 4:
		return REST;
		case 5:
		return STOP;
		
		default:
		return PANIC_SWITCH;
	}
}

int32_t convertFromStateEnumToInt(SiteState state) {
	switch(state) {
		case PANIC_SWITCH:
		return 0;
		case DIAGNOSTIC:
		return 1;
		case START:
		return 2;
		case RUNNING:
		return 3;
		case REST:
		return 4;
		case STOP:
		return 5;
		
		default:
		return 0;
	}
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUpdate
  (JNIEnv *env, jobject object) {
	executeProgram();
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callLightBarrierBlocked
  (JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	return lightBarrier->isBlocked;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherUpperBlocked
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->isFrontTriggerActivated;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherLowerBlocked
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->isBackTriggerActivated;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningBackwards
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == BACKWARDS;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherRunningForwards
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == FORWARDS;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callPusherInactive
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	return pusher->runningDirection == INACTIVE;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callToolRunning
  (JNIEnv *env, jobject object, jint id) {
	Tool *tool = resolveTool(id);
	
	return tool->isRunning;
}

JNIEXPORT jboolean JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callTreadmillRunning
  (JNIEnv *env, jobject object, jint id) {
	Treadmill *treadmill = resolveTreadmill(id);
	
	return treadmill->isRunning;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockUpperTrigger
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isFrontTriggerActivated = TRUE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockUpperTrigger
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isFrontTriggerActivated = FALSE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLowerTrigger
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isBackTriggerActivated = TRUE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLowerTrigger
  (JNIEnv *env, jobject object, jint id) {
	Pusher *pusher = resolvePusher(id);
	
	pusher->isBackTriggerActivated = FALSE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callBlockLightBarrier
  (JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	lightBarrier->isBlocked = TRUE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callUnblockLightBarrier
  (JNIEnv *env, jobject object, jint id) {
	LightBarrier *lightBarrier = resolveLightBarrier(id);
	
	lightBarrier->isBlocked = FALSE;
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callSetSiteState
  (JNIEnv *env, jobject object, jint newState) {
	SiteState state = convertFromIntToStateEnum(newState);
	
	setSiteState(state);
}

JNIEXPORT jint JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callGetSiteState
  (JNIEnv *env, jobject object) {
	SiteState currentState = getSiteState();
	
	return convertFromStateEnumToInt(currentState);
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandleTreadmill
  (JNIEnv *env, jobject object, jint id, jboolean activate) {
	Treadmill *treadmill = resolveTreadmill(id);
	
	if(treadmill == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(activate) {
		startTreadmill(treadmill);
	} else {
		stopTreadmill(treadmill);
	}
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandleTool
  (JNIEnv *env, jobject object, jint id, jboolean activate) {
	Tool *tool = resolveTool(id);
	
	if(tool == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(activate) {
		startTool(tool);
	} else {
		stopTool(tool);
	}
	  
}

JNIEXPORT void JNICALL Java_de_hm_cs_embedded_simulator_model_NativeLibrary_callHandlePusher
  (JNIEnv *env, jobject object, jint id, jboolean stop, jboolean forward, jboolean backward) {
	Pusher *pusher = resolvePusher(id);
	
	if(pusher == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(stop) {
		stopPusher(pusher);
	} else if (forward) {
		runForwardPusher(pusher);
	} else if (backward) {
		runBackwardsPusher(pusher);
	}
}
