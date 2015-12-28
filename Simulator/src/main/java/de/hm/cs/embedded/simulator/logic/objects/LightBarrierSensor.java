package de.hm.cs.embedded.simulator.logic.objects;

/**
 * Created by qriz on 12/27/15.
 */
public class LightBarrierSensor extends SiteObject {

    public LightBarrierSensor(int x, int y, boolean isXAligned) {
        super(-1, isXAligned?x:x-(5/2), isXAligned?y-(5/2):y,
                isXAligned?20:5, isXAligned?5:20);
        // I hope I don't go to hell for this ctor...
    }

    @Override
    public boolean isTriggeredOrActivated() {
        return false;
    }
}
