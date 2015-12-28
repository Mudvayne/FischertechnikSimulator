#ifndef __LIGHT_BARRIER
#define __LIGHT_BARRIER
typedef struct {
	short id;
    short isBlocked;
} LightBarrier;
#endif // __LIGHT_BARRIER

extern void setBlocked(LightBarrier *lightBarrier);
extern void setUnblocked(LightBarrier *lightBarrier);

extern void updateLightBarrier(LightBarrier *lightBarrier);
