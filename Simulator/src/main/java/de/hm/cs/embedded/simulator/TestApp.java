package de.hm.cs.embedded.simulator;

import de.hm.cs.embedded.simulator.logic.Logic;
import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.logic.objects.SiteState;
import de.hm.cs.embedded.simulator.logic.objects.Tool;
import de.hm.cs.embedded.simulator.model.CModel;
import de.hm.cs.embedded.simulator.model.ConstructionSite;
import de.hm.cs.embedded.simulator.model.USBModel;

/**
 * Created by qriz on 1/7/16.
 */
public class TestApp {

    public static void main(String[] args) {
        ConstructionSite site = new USBModel();
        Logic logic = new Simulation(site);
        logic.initConstructionSite();

//        site.handleTool(site.getTools().get(0), true);
//        site.handleTool(site.getTools().get(0), false);
//
//        site.handlePusher(site.getPushers().get(0), Pusher.State.GOING_FORWARD);
//        site.handlePusher(site.getPushers().get(0), Pusher.State.NOT_MOVING);
//        site.handlePusher(site.getPushers().get(0), Pusher.State.GOING_BACKWARD);
//        site.handlePusher(site.getPushers().get(0), Pusher.State.NOT_MOVING);
//
//        site.handleTreadmill(site.getTreadmills().get(0), true);
//        site.handleTreadmill(site.getTreadmills().get(0), false);
//
//        site.changeSiteState(SiteState.PANIC_SWITCH);
//        site.changeSiteState(SiteState.DIAGNOSTIC);

        site.updateValuesOnly();
    }
}
