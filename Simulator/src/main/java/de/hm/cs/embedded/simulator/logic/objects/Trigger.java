package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class Trigger extends SiteObject {
    public static final int TRIGGER_SIZE = 20;

    private boolean triggered;

    public Trigger(int x, int y) {
        super(-1, x, y, TRIGGER_SIZE, TRIGGER_SIZE);
    }

    public static int getTriggerSize() {
        return TRIGGER_SIZE;
    }

    public boolean isTriggered() {
        return triggered;
    }

    public void setTriggered(boolean triggered) {
        this.triggered = triggered;
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return triggered;
    }
}
