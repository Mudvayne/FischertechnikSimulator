package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public enum Direction {
    UP(0,-1),
    RIGHT(1,0),
    DOWN(0,1),
    RESTING(0,0);

    private float directionX;
    private float directionY;

    Direction(float directionX, float directionY) {
        this.directionX = directionX;
        this.directionY = directionY;
    }

    public float getDirectionX() {
        return directionX;
    }

    public float getDirectionY() {
        return directionY;
    }
}
