package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class LightBarrierSensor extends SiteObject {

    public LightBarrierSensor(int x, int y, boolean isXAligned) {
        super(-1, isXAligned?x:x-(1/2), isXAligned?y-(1/2):y,
                isXAligned?100:1, isXAligned?1:100);
        // I hope I don't go to hell for this ctor...
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return false;
    }
}
