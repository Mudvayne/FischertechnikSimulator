package de.hm.cs.embedded.simulator.view;

import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.logic.objects.SiteObject;

import javax.swing.*;
import java.awt.*;

/**
 * Created by qriz on 12/27/15.
 */
public class Drawable {
    public static final int SCALE = 4;
    public static final int OFFSET = 50;

    private SiteObject object;

    private int x;
    private int y;
    private int width;
    private int height;

    private boolean triggeredOrActivated;

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getHeight() {
        return height;
    }

    public int getWidth() {
        return width;
    }

    public boolean isTriggeredOrActivated() {
        return triggeredOrActivated;
    }

    public SiteObject getObject() {
        return object;
    }

    public Drawable(SiteObject object) {
        super();
        this.object = object;

        x = (int)(object.getX() * SCALE + OFFSET);
        y = (int)(object.getY() * SCALE + OFFSET);
        width = object.getWidth() * SCALE;
        height = object.getHeight() * SCALE;
        this.triggeredOrActivated = object.isTriggeredOrActivated();
    }

    public boolean update() {
        boolean change = false;
        int x = (int)(object.getX() * SCALE + OFFSET);
        int y = (int)(object.getY() * SCALE + OFFSET);
        boolean triggeredOrActivated = object.isTriggeredOrActivated();

        if(this.x != x) {
            change = true;
        }
        if(this.y != y) {
            change = true;
        }
        if(this.triggeredOrActivated != triggeredOrActivated) {
            change = true;
        }

        this.x = x;
        this.y = y;
        this.triggeredOrActivated = triggeredOrActivated;

        return change;
    }
}
