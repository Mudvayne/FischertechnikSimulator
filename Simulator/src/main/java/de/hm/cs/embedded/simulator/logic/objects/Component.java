package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class Component extends SiteObject {

    public static final int COMPONENT_SIZE = 80;

    private Direction movingDirection;
    private boolean teleportOne;
    private boolean teleportTwo;

    public boolean isTeleportTwo() {
        return teleportTwo;
    }

    public void setTeleportTwo(boolean teleportTwo) {
        this.teleportTwo = teleportTwo;
    }

    public boolean isTeleportOne() {
        return teleportOne;
    }

    public void setTeleportOne(boolean teleportOne) {
        this.teleportOne = teleportOne;
    }

    public Direction getMovingDirection() {
        return movingDirection;
    }

    public void setMovingDirection(Direction movingDirection) {
        this.movingDirection = movingDirection;
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return false;
    }

    public Component(int id, int x, int y) {
        super(id, x, y, COMPONENT_SIZE, COMPONENT_SIZE);
        this.movingDirection = Direction.RESTING;
        this.teleportOne = false;
        this.teleportTwo = false;
    }
}
