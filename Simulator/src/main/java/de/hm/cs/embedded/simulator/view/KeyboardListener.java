package de.hm.cs.embedded.simulator.view;

import de.hm.cs.embedded.simulator.logic.Logic;
import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.logic.objects.SiteState;
import de.hm.cs.embedded.simulator.model.ConstructionSite;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/**
 * Created by qriz on 12/27/15.
 */
public class KeyboardListener implements KeyListener {
    public Logic logic;
    public ConstructionSite site;

    public KeyboardListener(Logic logic, ConstructionSite site) {
        this.logic = logic;
        this.site = site;
    }

    public void keyTyped(KeyEvent keyEvent) {

        if (keyEvent.getKeyChar() == ' ') {
            logic.addComponent();
        } else if ((int) keyEvent.getKeyChar() >= (int) '0' && (int) keyEvent.getKeyChar() <= (int) '9') {
            logic.removeComponent(keyEvent.getKeyChar() - '0');
        }

        if (keyEvent.getKeyChar() == 'z') {
            logic.changeSiteState(SiteState.PANIC_SWITCH);

        } else if (keyEvent.getKeyChar() == 'x') {
            logic.changeSiteState(SiteState.DIAGNOSTIC);

        } else if (keyEvent.getKeyChar() == 'c') {
            logic.changeSiteState(SiteState.START);

        } else if (keyEvent.getKeyChar() == 'v') {
            logic.changeSiteState(SiteState.RUNNING);

        } else if (keyEvent.getKeyChar() == 'b') {
            logic.changeSiteState(SiteState.REST);

        } else if (keyEvent.getKeyChar() == 'n') {
            logic.changeSiteState(SiteState.STOP);
        }

        //Panic-Switch
        if(keyEvent.getKeyChar() == 'p') {
            if(site.isPanicSwitchPressed()) {
                site.releasePanicSwitch();
            } else {
                site.pressPanicSwitch();
            }
        }

        if (keyEvent.getKeyChar() == 'a') {
            site.handleTreadmill(site.getTreadmills().get(0), !site.getTreadmills().get(0).isActivated());

        } else if (keyEvent.getKeyChar() == 'q') {
            site.handlePusher(site.getPushers().get(0), Pusher.State.GOING_BACKWARD);

        } else if (keyEvent.getKeyChar() == 'w') {
            site.handlePusher(site.getPushers().get(0), Pusher.State.NOT_MOVING);

        } else if (keyEvent.getKeyChar() == 'e') {
            site.handlePusher(site.getPushers().get(0), Pusher.State.GOING_FORWARD);

        } else if (keyEvent.getKeyChar() == 'r') {
            site.handleTreadmill(site.getTreadmills().get(1), !site.getTreadmills().get(1).isActivated());

        } else if (keyEvent.getKeyChar() == 'd') {
            site.handleTool(site.getTools().get(0), !site.getTools().get(0).isActivated());

        } else if (keyEvent.getKeyChar() == 't') {
            site.handleTreadmill(site.getTreadmills().get(2), !site.getTreadmills().get(2).isActivated());

        } else if (keyEvent.getKeyChar() == 'f') {
            site.handleTool(site.getTools().get(1), !site.getTools().get(1).isActivated());

        } else if (keyEvent.getKeyChar() == 'y') {
            site.handlePusher(site.getPushers().get(1), Pusher.State.GOING_BACKWARD);

        } else if (keyEvent.getKeyChar() == 'u') {
            site.handlePusher(site.getPushers().get(1), Pusher.State.NOT_MOVING);

        } else if (keyEvent.getKeyChar() == 'i') {
            site.handlePusher(site.getPushers().get(1), Pusher.State.GOING_FORWARD);

        } else if (keyEvent.getKeyChar() == 'j') {
            site.handleTreadmill(site.getTreadmills().get(3), !site.getTreadmills().get(3).isActivated());
        }

        if (keyEvent.getKeyChar() == 'k') {
            site.blockLightBarrier(site.getLightBarriers().get(0));
        } else if (keyEvent.getKeyChar() == 'l') {
            site.blockLightBarrier(site.getLightBarriers().get(1));
        } else if (keyEvent.getKeyChar() == 'ö' || keyEvent.getKeyChar() == ';') {
            site.blockLightBarrier(site.getLightBarriers().get(2));
        } else if (keyEvent.getKeyChar() == 'ä' || keyEvent.getKeyChar() == '\'') {
            site.blockLightBarrier(site.getLightBarriers().get(3));
        } else if (keyEvent.getKeyChar() == '#' || keyEvent.getKeyChar() == '\\') {
            site.blockLightBarrier(site.getLightBarriers().get(4));
        }
    }

    public void keyPressed(KeyEvent keyEvent) {
        //Do nothing
    }

    public void keyReleased(KeyEvent keyEvent) {
        //Do nothing
    }
}
