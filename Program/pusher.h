#ifndef __PUSHER
#define __PUSHER
typedef enum {
    BACKWARDS,
    INACTIVE,
    FORWARDS
} RunningDirection;

typedef struct {
	short id;
    RunningDirection runningDirection;
    short isFrontTriggerActivated;
    short isBackTriggerActivated;
} Pusher;
#endif // __PUSHER

extern void runForwardPusher(Pusher *pusher);
extern void runBackwardsPusher(Pusher *pusher);
extern void stopPusher(Pusher *pusher);

extern void updatePusher(Pusher *pusher);
