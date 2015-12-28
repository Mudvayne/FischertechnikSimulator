#include "model.h"

Treadmill firstTreadmill = {.id=0, .isRunning=0};
Treadmill secondTreadmill = {.id=1, .isRunning=0};
Treadmill thirdTreadmill = {.id=2, .isRunning=0};
Treadmill fourthTreadmill = {.id=3, .isRunning=0};

Pusher firstPusher = {.id=0, .runningDirection=INACTIVE};
Pusher secondPusher = {.id=1, .runningDirection=INACTIVE};

LightBarrier firstLightBarrier = {.id=0, .isBlocked = 0};
LightBarrier secondLightBarrier = {.id=1, .isBlocked = 0};
LightBarrier thirdLightBarrier = {.id=2, .isBlocked = 0};
LightBarrier fourthLightBarrier = {.id=3, .isBlocked = 0};
LightBarrier fifthLightBarrier = {.id=4, .isBlocked = 0};

Tool firstTool = {.id=0, .isRunning=0};
Tool secondTool = {.id=1, .isRunning=0};

SiteState currentState = RUNNING;

Treadmill* getFirstTreadmill()
{
    return &firstTreadmill;
}

Treadmill* getSecondTreadmill()
{
    return &secondTreadmill;
}

Treadmill* getThirdTreadmill()
{
    return &thirdTreadmill;
}

Treadmill* getFourthTreadmill()
{
    return &fourthTreadmill;
}

Pusher* getFirstPusher()
{
    return &firstPusher;
}

Pusher* getSecondPusher()
{
    return &secondPusher;
}

LightBarrier* getFirstLightBarrier()
{
    return &firstLightBarrier;
}

LightBarrier* getSecondLightBarrier()
{
    return &secondLightBarrier;
}

LightBarrier* getThirdLightBarrier()
{
    return &thirdLightBarrier;
}

LightBarrier* getFourthLightBarrier()
{
    return &fourthLightBarrier;
}

LightBarrier* getFifthLightBarrier()
{
    return &fifthLightBarrier;
}

Tool* getFirstTool()
{
    return &firstTool;
}

Tool* getSecondTool()
{
    return &secondTool;
}

SiteState getSiteState()
{
    return currentState;
}

void setSiteState(SiteState newState)
{
    currentState = newState;
}

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
