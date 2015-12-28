package de.hm.cs.embedded.simulator.logic.objects;

import java.util.UUID;

/**
 * Created by qriz on 12/27/15.
 */
public abstract class SiteObject {
    private float x;
    private float y;

    private int width;
    private int height;

    private int id;

    public int getId() {
        return id;
    }

    public float getX() {
        return x;
    }

    public void setX(float x) {
        this.x = x;
    }

    public float getY() {
        return y;
    }

    public void setY(float y) {
        this.y = y;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public SiteObject(int id, float x, float y, int width, int height) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public abstract boolean isTriggeredOrActivated();

    public boolean collision(SiteObject other) {
        return ((x <= other.x && other.x <= (x+width)) || (other.x <= x && x <= other.x+other.width)) &&
                ((y <= other.y && other.y <= (y+height)) || (other.y <= y && y <= other.y+other.height));
    }
}
