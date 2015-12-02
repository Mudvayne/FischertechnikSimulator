typedef enum {
    BACKWARDS,
    INACTIVE,
    FORWARDS
} RunningDirection;

typedef struct {
    RunningDirection runningDirection;
    short isFrontTriggerActivated;
    short isBackTriggerActivated;
} Pusher;

void runForwardPusher(Pusher *pusher);
void runBackwardsPusher(Pusher *pusher);
void stopPusher(Pusher *pusher);
