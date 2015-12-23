#include "../communication.h"
#include "model.h"
#include "treadmill.h"

void equalTreadmill(uint8_t id, Treadmill *treadmill, Treadmill *target) {
	if(id == treadmill->id) {
		target = treadmill;
	}
}

Treadmill* resolveTreadmill(uint8_t id) {
	Treadmill *desiredMill = 0;
	
	equalTreadmill(id, getFirstTreadmill(), desiredMill);
	equalTreadmill(id, getSecondTreadmill(), desiredMill);
	equalTreadmill(id, getThirdTreadmill(), desiredMill);
	equalTreadmill(id, getFourthTreadmill(), desiredMill);
	
	return desiredMill;
}

void handle_treadmill(const ComType com, const HandleTreadmill *data) {
	Treadmill *treadmill = resolveTreadmill(data->treadmillId);
	
	if(data->start) {
		startTreadmill(treadmill);
	} else {
		stopTreadmill(treadmill);
	}
}