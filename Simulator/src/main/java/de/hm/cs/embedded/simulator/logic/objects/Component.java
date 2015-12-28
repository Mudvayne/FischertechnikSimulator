package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class Component extends SiteObject {

    public static final int COMPONENT_SIZE = 18;

    private Direction movingDirection;

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
        super(id, x+1, y, COMPONENT_SIZE, COMPONENT_SIZE);
        this.movingDirection = Direction.RESTING;
    }
}
