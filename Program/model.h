#include <stdint.h>
#include "treadmill.h"
#include "lightBarrier.h"
#include "pusher.h"
#include "tool.h"
#include "site.h"

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
extern void setSiteState(SiteState newState, short errCode);

extern Treadmill* resolveTreadmill(uint8_t id);
extern Tool* resolveTool(uint8_t id);
extern Pusher* resolvePusher(uint8_t id);
extern LightBarrier* resolveLightBarrier(uint8_t id);
