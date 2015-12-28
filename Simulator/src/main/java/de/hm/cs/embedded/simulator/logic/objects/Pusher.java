package de.hm.cs.embedded.simulator.logic.objects;

import java.util.UUID;

/**
 * Created by qriz on 12/27/15.
 */
public class Pusher extends SiteObject {
    public static final int PUSHER_SHORTER_SIZE = 5;
    public static final int PUSHER_LONGER_SIZE = 30;

    private State state;
    private Trigger upperTrigger;
    private Trigger lowerTrigger;

    public Pusher(int id, int x, int y, int width, int height, Trigger upperTrigger, Trigger lowerTrigger) {
        super(id, x, y, width, height);

        this.state = State.NOT_MOVING;
        this.upperTrigger = upperTrigger;
        this.lowerTrigger = lowerTrigger;
    }

    public Trigger getUpperTrigger() {
        return upperTrigger;
    }

    public Trigger getLowerTrigger() {
        return lowerTrigger;
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return state == State.GOING_BACKWARD || state == State.GOING_FORWARD;
    }

    public enum State {
        GOING_FORWARD,
        GOING_BACKWARD,
        NOT_MOVING
    }
}
