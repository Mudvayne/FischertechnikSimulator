package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.LightBarrier;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.logic.objects.Tool;
import de.hm.cs.embedded.simulator.logic.objects.Treadmill;

/**
 * Created by qriz on 12/27/15.
 */
public class CModel extends LocalModel {
    private NativeLibrary library;

    public CModel() {
        library = new NativeLibrary();
    }

    public void update() {
        library.callUpdate();
        updateValuesOnly();
    }

    public void updateValuesOnly() {
        for(LightBarrier lightBarrier : getLightBarriers()) {
            lightBarrier.setBlocked(library.callLightBarrierBlocked(lightBarrier.getId()));
        }

        for(Pusher pusher : getPushers()) {
            pusher.getUpperTrigger().setTriggered(library.callPusherUpperBlocked(pusher.getId()));
            pusher.getLowerTrigger().setTriggered(library.callPusherLowerBlocked(pusher.getId()));

            if(library.callPusherRunningForwards(pusher.getId())) {
                pusher.setState(Pusher.State.GOING_FORWARD);
            } else if (library.callPusherRunningBackwards(pusher.getId())) {
                pusher.setState(Pusher.State.GOING_BACKWARD);
            } else if (library.callPusherInactive(pusher.getId())) {
                pusher.setState(Pusher.State.NOT_MOVING);
            }
        }

        for(Tool tool : getTools()) {
            tool.setActivated(library.callToolRunning(tool.getId()));
        }

        for(Treadmill treadmill : getTreadmills()) {
            treadmill.setActivated(library.callTreadmillRunning(treadmill.getId()));
        }
    }

    public void blockUpperTrigger(Pusher pusher) {
        library.callBlockUpperTrigger(pusher.getId());
        pusher.getUpperTrigger().setTriggered(true);
    }

    public void unblockUpperTrigger(Pusher pusher) {
        library.callUnblockUpperTrigger(pusher.getId());
        pusher.getUpperTrigger().setTriggered(false);
    }

    public void blockLowerTrigger(Pusher pusher) {
        library.callBlockLowerTrigger(pusher.getId());
        pusher.getLowerTrigger().setTriggered(true);
    }

    public void unblockLowerTrigger(Pusher pusher) {
        library.callUnblockLowerTrigger(pusher.getId());
        pusher.getLowerTrigger().setTriggered(false);
    }

    public void blockLightBarrier(LightBarrier lightBarrier) {
        library.callBlockLightBarrier(lightBarrier.getId());
        lightBarrier.setBlocked(true);
    }

    public void unblockLightBarrier(LightBarrier lightBarrier) {
        library.callUnblockLightBarrier(lightBarrier.getId());
        lightBarrier.setBlocked(false);
    }
}
