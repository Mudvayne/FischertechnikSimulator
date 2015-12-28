package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/28/15.
 */
public class NullObject extends SiteObject {

    public NullObject(int id, float x, float y, int width, int height) {
        super(id, x, y, width, height);
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return false;
    }
}
