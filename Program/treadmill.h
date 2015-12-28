#ifndef __TREADMILL
#define __TREADMILL
typedef struct {
	short id;
    short isRunning;
} Treadmill;
#endif // __TREADMILL

extern void startTreadmill(Treadmill *treadmill);
extern void stopTreadmill(Treadmill *treadmill);
