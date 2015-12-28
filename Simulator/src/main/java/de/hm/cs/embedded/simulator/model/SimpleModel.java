package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.LightBarrier;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;

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
}
