#include <stdint.h>
#include "treadmill.h"
#include "lightBarrier.h"
#include "pusher.h"
#include "tool.h"
#include "site.h"

#define CODE_NO_ERROR 0
#define CODE_PANIC_SWITCH 100
#define CODE_NOT_IMPLEMENTED 200

extern Treadmill* getFirstTreadmill();
extern Treadmill* getSecondTreadmill();
extern Treadmill* getThirdTreadmill();
extern Treadmill* getFourthTreadmill();

extern Pusher* getFirstPusher();
extern Pusher* getSecondPusher();

extern Tool* getFirstTool();
extern Tool* getSecondTool();

extern LightBarrier* getFirstLightBarrier();
extern LightBarrier* getSecondLightBarrier();
extern LightBarrier* getThirdLightBarrier();
extern LightBarrier* getFourthLightBarrier();
extern LightBarrier* getFifthLightBarrier();

extern SiteState getSiteState();
extern void setSiteState(SiteState newState, uint8_t code);
extern uint8_t getErrorCode();

extern Treadmill* resolveTreadmill(uint8_t id);
extern Tool* resolveTool(uint8_t id);
extern Pusher* resolvePusher(uint8_t id);
extern LightBarrier* resolveLightBarrier(uint8_t id);
