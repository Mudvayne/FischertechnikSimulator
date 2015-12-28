package de.hm.cs.embedded.simulator.logic.objects;

import java.util.UUID;

/**
 * Created by qriz on 12/27/15.
 */
public class Tool extends SiteObject {
    public static final int TOOL_SIZE = 100;
    private boolean activated;

    public Tool(int id, int x, int y) {
        super(id, x, y, TOOL_SIZE, TOOL_SIZE);

        this.activated = false;
    }

    public boolean isActivated() {
        return activated;
    }

    public void setActivated(boolean activated) {
        this.activated = activated;
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return activated;
    }
}
