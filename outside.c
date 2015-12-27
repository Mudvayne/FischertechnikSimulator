#include "../communication.h"
#include "model.h"
#include "treadmill.h"
#include "pusher.h"
#include "tool.h"
#include "lightBarrier.h"
#include "site.h"

//#######################
//####HELPER METHODS#####
//#######################

void equalTreadmill(uint8_t id, Treadmill *treadmill, Treadmill **target) {
	if(id == treadmill->id) {
		(*target) = treadmill;
	}
}

Treadmill* resolveTreadmill(uint8_t id) {
	Treadmill *desiredMill = 0;
	
	equalTreadmill(id, getFirstTreadmill(), &desiredMill);
	equalTreadmill(id, getSecondTreadmill(), &desiredMill);
	equalTreadmill(id, getThirdTreadmill(), &desiredMill);
	equalTreadmill(id, getFourthTreadmill(), &desiredMill);
	
	return desiredMill;
}

void equalTool(uint8_t id, Tool *tool, Tool **target) {
	if(id == tool->id) {
		(*target) = tool;
	}
}

Tool* resolveTool(uint8_t id) {
	Tool *desiredTool = 0;
	
	equalTool(id, getFirstTool(), &desiredTool);
	equalTool(id, getSecondTool(), &desiredTool);
	
	return desiredTool;
}

void equalPusher(uint8_t id, Pusher *pusher, Pusher **target) {
	if (id == pusher->id) {
		(*target) = pusher;
	}
}

Pusher* resolvePusher(uint8_t id) {
	Pusher *desiredPusher = 0;
	
	equalPusher(id, getFirstPusher(), &desiredPusher);
	equalPusher(id, getSecondPusher(), &desiredPusher);
	
	return desiredPusher;
}

void equalLightBarrier(uint8_t id, LightBarrier *lightBarrier, LightBarrier **target) {
	if(id == lightBarrier->id) {
		(*target) = lightBarrier;
	}
}

LightBarrier* resolveLightBarrier(uint8_t id) {
	LightBarrier *desiredLightBarrier = 0;
	
	equalLightBarrier(id, getFirstLightBarrier(), &desiredLightBarrier);
	equalLightBarrier(id, getSecondLightBarrier(), &desiredLightBarrier);
	equalLightBarrier(id, getThirdLightBarrier(), &desiredLightBarrier);
	equalLightBarrier(id, getFourthLightBarrier(), &desiredLightBarrier);
	equalLightBarrier(id, getFifthLightBarrier(), &desiredLightBarrier);
	
	return desiredLightBarrier;
}

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


