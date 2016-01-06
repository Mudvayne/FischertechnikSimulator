package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.*;

/**
 * Created by qriz on 12/27/15.
 */
public class SimpleModel extends LocalModel {

    public void update() {
        for (Pusher pusher : getPushers()) {
            if (pusher.getUpperTrigger().isTriggered()) {
                pusher.setState(Pusher.State.NOT_MOVING);

            } else if (pusher.getLowerTrigger().isTriggered()) {
                pusher.setState(Pusher.State.NOT_MOVING);
            }
        }
    }

    public void updateValuesOnly() {

    }

    public void blockUpperTrigger(Pusher pusher) {
        pusher.getUpperTrigger().setTriggered(true);
    }

    public void unblockUpperTrigger(Pusher pusher) {
        pusher.getUpperTrigger().setTriggered(false);
    }

    public void blockLowerTrigger(Pusher pusher) {
        pusher.getLowerTrigger().setTriggered(true);
    }

    public void unblockLowerTrigger(Pusher pusher) {
        pusher.getLowerTrigger().setTriggered(false);
    }

    public void blockLightBarrier(LightBarrier lightBarrier) {
        lightBarrier.setBlocked(true);
    }

    public void unblockLightBarrier(LightBarrier lightBarrier) {
        lightBarrier.setBlocked(false);
    }

    public void changeSiteState(SiteState siteState) {
        this.siteState = siteState;
    }

    public void handleTreadmill(Treadmill treadmill, boolean activate) {
        treadmill.setActivated(activate);
    }

    public void handleTool(Tool tool, boolean activate) {
        tool.setActivated(activate);
    }

    public void handlePusher(Pusher pusher, Pusher.State state) {
        pusher.setState(state);
    }
}
