package de.hm.cs.embedded.simulator.logic.objects;

import java.util.UUID;

/**
 * Created by qriz on 12/27/15.
 */
public class Treadmill extends SiteObject {
    public static final int TREADMILL_SHORTER_SIZE = 100;
    public static final int TREADMILL_LONGER_SIZE = 320;

    private boolean activated;
    private Direction direction;

    @Override
    public boolean isTriggeredOrActivated() {
        return activated;
    }

    public Direction getDirection() {
        return direction;
    }

    public boolean isActivated() {
        return activated;
    }

    public void setActivated(boolean activated) {
        this.activated = activated;
    }

    public Treadmill(int id, int x, int y, int width, int height, Direction direction) {
        super(id, x, y, width, height);

        this.activated = false;
        this.direction = direction;
    }
}
