package de.hm.cs.embedded.simulator.logic;

import de.hm.cs.embedded.simulator.logic.objects.*;

import java.util.List;

/**
 * Created by qriz on 1/6/16.
 */
public interface Logic {
    List<LightBarrier> getLightBarriers();

    List<Treadmill> getTreadmills();

    List<Pusher> getPushers();

    List<Tool> getTools();

    List<Component> getComponents();

    SiteState getCurrentSiteState();

    void initConstructionSite();

    void addComponent();

    void removeComponent(int id);

    void updateSimulation(int deltaInMs);

    void changeSiteState(SiteState newState);

    boolean isPanicButtonPressed();
}
