typedef enum{
    BACKWARDS,
    INACTIVE,
    FORWARD
} RunningDirection;

typedef struct {
    RunningDirection runningDirection;
    short isTriggerActivated;
} Pusher;

void runForwardPusher(Pusher *pusher);
void runBackwardsPusher(Pusher *pusher);
void stopPusher(Pusher *pusher);

void triggerTrigger(Pusher *pusher);
void untriggerTrigger(Pusher *pusher);
