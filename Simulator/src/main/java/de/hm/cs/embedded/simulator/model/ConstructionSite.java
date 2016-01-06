package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.*;

import java.util.List;

/**
 * Created by qriz on 12/27/15.
 */
public interface ConstructionSite {

    List<LightBarrier> getLightBarriers();

    List<Treadmill> getTreadmills();

    List<Pusher> getPushers();

    List<Tool> getTools();

    SiteState getSiteState();

    boolean isPanicSwitchPressed();

    void init(List<LightBarrier> lightBarriers, List<Treadmill> treadmills, List<Pusher> pushers, List<Tool> tools);

    void update();

    void updateValuesOnly();

    void blockUpperTrigger(Pusher pusher);

    void unblockUpperTrigger(Pusher pusher);

    void blockLowerTrigger(Pusher pusher);

    void unblockLowerTrigger(Pusher pusher);

    void blockLightBarrier(LightBarrier lightBarrier);

    void unblockLightBarrier(LightBarrier lightBarrier);

    void changeSiteState(SiteState siteState);

    void handleTreadmill(Treadmill treadmill, boolean activate);

    void handleTool(Tool tool, boolean activate);

    void handlePusher(Pusher pusher, Pusher.State state);

    void pressPanicSwitch();

    void releasePanicSwitch();
}
