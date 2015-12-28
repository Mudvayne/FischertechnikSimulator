#include "../communication.h"
#include "model.h"
#include "treadmill.h"
#include "pusher.h"
#include "tool.h"
#include "lightBarrier.h"
#include "site.h"

SiteState convertFromIntToStateEnum(uint8_t state) {
	switch(state) {
		case 0:
		return PANIC_SWITCH;
		
		
		default:
		return PANIC_SWITCH;
	}
}

uint8_t convertFromStateEnumToInt(SiteState state) {
	switch(state) {
		case PANIC_SWITCH:
		return 0;
		
		
		default:
		return 0;
	}
}

//#######################
//##IMPLEMENTED METHODS##
//#######################

void handle_treadmill(const ComType com, const HandleTreadmill *data) {
	Treadmill *treadmill = resolveTreadmill(data->treadmillId);
	
	if(treadmill == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(data->start) {
		startTreadmill(treadmill);
	} else {
		stopTreadmill(treadmill);
	}
}

void handle_tool(const ComType com, const HandleTool *data) {
	Tool *tool = resolveTool(data->toolId);
	
	if(tool == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(data->start) {
		startTool(tool);
	} else {
		stopTool(tool);
	}
}

void handle_pusher(const ComType com, const HandlePusher *data) {
	Pusher *pusher = resolvePusher(data->pusherId);
	
	if(pusher == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	if(data->stop) {
		stopPusher(pusher);
	} else if (data->goForward) {
		runForwardPusher(pusher);
	} else if (data->goBackward) {
		runBackwardsPusher(pusher);
	}
}

void getLightBarrierStatus(const ComType com, const GetLightBarrierStatus *data) {
	LightBarrier *lightBarrier = resolveLightBarrier(data->lightBarrierId);
	
	if(lightBarrier == 0) {
		//TODO: Add exceptionhandling.
		return;
	}
	
	GetLightBarrierStatusReturn response;

	response.header = data->header;
	response.lightBarrierId; = data->lightBarrierId;
	response.blocked = lightBarrier->isBlocked;

	send_blocking_with_timeout(&response, sizeof(GetLightBarrierStatusReturn), com);
}

void getSiteStateStatus(const ComType com, const GetSiteSateStatus *data) {
	
}

void setSiteStateStatus(const ComType com, const SetSiteStateStatus *data) {
	
}


