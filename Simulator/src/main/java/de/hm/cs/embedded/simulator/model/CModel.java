package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.*;

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

        siteState = convertIntToState(library.callGetSiteState());
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

    public void changeSiteState(SiteState newState) {
        library.callSetSiteState(convertStateToInt(newState));
        siteState = newState;
    }

    public void handleTreadmill(Treadmill treadmill, boolean activate) {
        library.callHandleTreadmill(treadmill.getId(), activate);
        treadmill.setActivated(activate);
    }

    public void handleTool(Tool tool, boolean activate) {
        library.callHandleTool(tool.getId(), activate);
        tool.setActivated(activate);
    }

    public void handlePusher(Pusher pusher, Pusher.State state) {
        library.callHandlePusher(pusher.getId(),
                state == Pusher.State.NOT_MOVING, state == Pusher.State.GOING_FORWARD, state == Pusher.State.GOING_BACKWARD);
        pusher.setState(state);
    }

    private int convertStateToInt(SiteState siteState) {
        switch (siteState) {
            case PANIC_SWITCH:
                return 0;
            case DIAGNOSTIC:
                return 1;
            case START:
                return 2;
            case RUNNING:
                return 3;
            case REST:
                return 4;
            case STOP:
                return 5;

            default:
                return 0;
        }
    }

    private SiteState convertIntToState(int state) {
        switch (state) {
            case 0:
                return SiteState.PANIC_SWITCH;
            case 1:
                return SiteState.DIAGNOSTIC;
            case 2:
                return SiteState.START;
            case 3:
                return SiteState.RUNNING;
            case 4:
                return SiteState.REST;
            case 5:
                return SiteState.STOP;

            default:
                return SiteState.PANIC_SWITCH;
        }
    }
}
