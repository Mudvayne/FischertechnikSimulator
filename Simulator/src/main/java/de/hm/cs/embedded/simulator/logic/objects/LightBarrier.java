package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class LightBarrier extends SiteObject {

    public static final int LIGHT_BARRIER_SIZE = 20;
    private boolean blocked;
    private LightBarrierSensor sensor;

    public boolean isBlocked() {
        return blocked;
    }

    public void setBlocked(boolean blocked) {
        this.blocked = blocked;
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return blocked;
    }

    public LightBarrierSensor getSensor() {
        return sensor;
    }

    public LightBarrier(int id, int x, int y, LightBarrierSensor sensor) {
        super(id, x, y, LIGHT_BARRIER_SIZE, LIGHT_BARRIER_SIZE);

        this.blocked = false;
        this.sensor = sensor;
    }
}
