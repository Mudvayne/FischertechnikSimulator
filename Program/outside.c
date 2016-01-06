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

void utilShiftBit(uint8_t isActive, uint32_t shiftWidth, uint32_t *status) {
	uint32_t bitField = isActive ? 1 : 0;
	
	(*status) | (isActive << shiftWidth);
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
	GetSiteStateStatusReturn response;
	
	response.header = data-header;
	response.currentStatus = convertFromStateEnumToInt(getSiteState());
	
	send_blocking_with_timeout(&response, sizeof(GetSiteStateStatusReturn), com);
}

void setSiteStateStatus(const ComType com, const SetSiteStateStatus *data) {
	SiteState newState = convertFromIntToStateEnum(data->newStatus);
	
	setSiteState(newState);
}

void getWholeSiteStatus(const ComType com, const GetWholeSiteStatus *data) {
	int32_t status = 0;
	
	//Treadmill
	utilShiftBit(getFirstTreadmill()->isRunning, 	0, &status);
	utilShiftBit(getSecondTreadmill()->isRunning, 	1, &status);
	utilShiftBit(getThirdTreadmill()->isRunning, 	2, &status);
	utilShiftBit(getFourthTreadmill()->isRunning, 	3, &status);
	
	//Tool
	utilShiftBit(getFirstTool()->isRunning,			4, &status);
	utilShiftBit(getSecondTool()->isRunning,		5, &status);
	
	//LightBarriers
	utilShiftBit(getFirstLightBarrier()->isBlocked,	6, &status);
	utilShiftBit(getSecondLightBarrier()->isBlocked,7, &status);
	utilShiftBit(getThirdLightBarrier()->isBlocked,	8, &status);
	utilShiftBit(getFourthLightBarrier()->isBlocked,9, &status);
	utilShiftBit(getFifthLightBarrier()->isBlocked,	10, &status);
	
	//Pusher
	Pusher *pusher = getFirstPusher();
	uint8_t isActive = pusher->runningDirection == BACKWARDS ? 1 : 0;
	utilShiftBit(isActive, 							11, &status);
	
	isActive = pusher->runningDirection == FORWARDS ? 1 : 0;
	utilShiftBit(isActive, 							12, &status);
	
	utilShiftBit(pusher->isFrontTriggerActivated,	13, &status);
	utilShiftBit(pusher->isBackTriggerActivated,	14, &status);
	
	//Now build and send response.
	GetWholeSiteStatusReturn response;
	
	response.header = data->header;
	response.status = status;

	send_blocking_with_timeout(&response, sizeof(GetWholeSiteStatusReturn), com);
}
