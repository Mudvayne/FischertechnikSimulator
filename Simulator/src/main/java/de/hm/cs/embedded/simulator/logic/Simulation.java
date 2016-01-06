package de.hm.cs.embedded.simulator.logic;

import de.hm.cs.embedded.simulator.logic.objects.*;
import de.hm.cs.embedded.simulator.model.ConstructionSite;

import javax.print.attribute.standard.MediaPrintableArea;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by qriz on 12/27/15.
 */
public class Simulation implements Logic {
    public static final float TREADMILL_SPEED = Float.valueOf(System.getProperty("treadmillSpeed", "0.064"));
    public static final float PUSHER_SPEED = Float.valueOf(System.getProperty("pusherSpeed", "0.0667"));
    public static final float MIN_COMPONENT_DISTANCE = Float.valueOf(System.getProperty("minComponentDistance", "85.0"));

    private ConstructionSite constructionSite;
    private List<Component> components;
    private NullObject siteEnd;
    private NullObject teleportPusherFirst;
    private NullObject teleportPusherSecond;

    private int componentCount = 0;

    public List<LightBarrier> getLightBarriers() {
        return constructionSite.getLightBarriers();
    }

    public List<Treadmill> getTreadmills() {
        return constructionSite.getTreadmills();
    }

    public List<Pusher> getPushers() {
        return constructionSite.getPushers();
    }

    public List<Tool> getTools() {
        return constructionSite.getTools();
    }

    public List<Component> getComponents() {
        return components;
    }

    public SiteState getCurrentSiteState() {
        return constructionSite.getSiteState();
    }

    public Simulation(ConstructionSite constructionSite) {
        this.constructionSite = constructionSite;
    }

    public void initConstructionSite() {
        components = new ArrayList<Component>();
        int id = 0;

        siteEnd = new NullObject(-1, 850, 550, 100, 10);
        teleportPusherFirst = new NullObject(-1, 80, 148, 100, 2);
        teleportPusherSecond = new NullObject(-1, 920, 120, 2, 100);

        List<LightBarrier> lightBarriers = Arrays.asList(new LightBarrier(id++, 190, 500, new LightBarrierSensor(80, 510, true)),
                new LightBarrier(id++, 190, 250, new LightBarrierSensor(80, 260, true)),
                new LightBarrier(id++, 340, 230, new LightBarrierSensor(350, 120, false)),
                new LightBarrier(id++, 670, 230, new LightBarrierSensor(680, 120, false)),
                new LightBarrier(id, 820, 440, new LightBarrierSensor(850, 450, true))
        );

        id = 0;
        List<Treadmill> treadmills = Arrays.asList(new Treadmill(id++, 80, 230, Treadmill.TREADMILL_SHORTER_SIZE, Treadmill.TREADMILL_LONGER_SIZE, Direction.UP),
                new Treadmill(id++, 190, 120, Treadmill.TREADMILL_LONGER_SIZE, Treadmill.TREADMILL_SHORTER_SIZE, Direction.RIGHT),
                new Treadmill(id++, 520, 120, Treadmill.TREADMILL_LONGER_SIZE, Treadmill.TREADMILL_SHORTER_SIZE, Direction.RIGHT),
                new Treadmill(id, 850, 230, Treadmill.TREADMILL_SHORTER_SIZE, Treadmill.TREADMILL_LONGER_SIZE, Direction.DOWN)
        );

        id = 0;
        Trigger firstUpperTrigger = new Trigger(180, 90);
        Trigger firstLowerTrigger = new Trigger(50, 90);
        Trigger secondUpperTrigger = new Trigger(960, 220);
        Trigger secondLowerTrigger = new Trigger(960, 90);
        List<Pusher> pushers = Arrays.asList(new Pusher(id++, 70, 100, Pusher.PUSHER_SHORTER_SIZE, Pusher.PUSHER_LONGER_SIZE, firstUpperTrigger, firstLowerTrigger),
                new Pusher(id, 850, 110, Pusher.PUSHER_LONGER_SIZE, Pusher.PUSHER_SHORTER_SIZE, secondUpperTrigger, secondLowerTrigger)
        );

        id = 0;
        List<Tool> tools = Arrays.asList(new Tool(id++, 300, 10),
                new Tool(id, 630, 10)
        );

        constructionSite.init(lightBarriers, treadmills, pushers, tools);
    }

    public void addComponent() {
        Component newComponent = new Component(componentCount, 90, 470);
        boolean startOccupied = false;
        float smallestDistance = Float.MAX_VALUE;

        for (Component component : getComponents()) {
            float distance = distance(newComponent, component);

            if(distance < smallestDistance) {
                smallestDistance = distance;
            }
            if (component.collision(newComponent)) {
                startOccupied = true;
            }
        }

        if (!startOccupied && smallestDistance > MIN_COMPONENT_DISTANCE) {
            componentCount++;
            componentCount = componentCount % 10;
            components.add(newComponent);
        }
    }

    private float distance(Component one, Component two) {
        float x = one.getX() - two.getX();
        float y = one.getY() - two.getY();
        x = x*x;
        y = y*y;

        return (float) Math.sqrt(x+y);
    }

    public void removeComponent(int id) {
        for(int index = getComponents().size()-1; index >= 0; index--) {
            if(getComponents().get(index).getId() == id) {
                getComponents().remove(index);
                break;
            }
        }
    }

    public void updateSimulation(int deltaInMs) {
        if(constructionSite.isPanicSwitchPressed()) {
            return;
        }

        simulateTreadmill(deltaInMs, getTreadmills());
        simulateLightBarriers(getLightBarriers());
        simulatePushers(deltaInMs, getPushers());
        simulateTeleporter();
    }

    public void changeSiteState(SiteState newState) {
        constructionSite.changeSiteState(newState);
    }

    public boolean isPanicButtonPressed() {
        return constructionSite.isPanicSwitchPressed();
    }

    private void simulateTeleporter() {
        for(Component component: getComponents()) {
            if(!component.isTeleportOne() && component.collision(teleportPusherFirst)) {
                component.setTeleportOne(true);
                component.setX(90);
                component.setY(130);
            } else if (!component.isTeleportTwo() && component.collision(teleportPusherSecond)) {
                component.setTeleportTwo(true);
                component.setX(860);
                component.setY(130);
            }
        }
    }

    private void simulateLightBarriers(List<LightBarrier> lightBarriers) {

        for (LightBarrier lightBarrier : lightBarriers) {
            boolean lightBarrierBlocked = false;
            for (Component component : components) {
                if (lightBarrier.getSensor().collision(component)) {
                    lightBarrierBlocked = true;
                    break;
                }
            }

            if (lightBarrierBlocked) {
                constructionSite.blockLightBarrier(lightBarrier);
            } else {
                constructionSite.unblockLightBarrier(lightBarrier);
            }
        }
    }

    private void simulateTreadmill(int deltaInMs, List<Treadmill> treadmills) {
        for (Component component : getComponents()) {
            component.setMovingDirection(Direction.RESTING);
        }

        for (Treadmill treadmill : treadmills) {
            for (Component component : getComponents()) {
                if (treadmill.isActivated() && treadmill.collision(component)) {
                    handle(deltaInMs, component, treadmill);
                }
            }
        }
    }

    private void handle(int deltaInMs, Component component, Treadmill treadmill) {
        float oldX = component.getX();
        float oldY = component.getY();

        if (component.getMovingDirection() != Direction.RESTING) {
            return;
        } else {
            component.setMovingDirection(treadmill.getDirection());
        }

        component.setX(component.getX() + treadmill.getDirection().getDirectionX() * TREADMILL_SPEED * deltaInMs);
        component.setY(component.getY() + treadmill.getDirection().getDirectionY() * TREADMILL_SPEED * deltaInMs);

        //Collision with other Component should be easy
        //Just reset position.
        for (Component otherComponent : getComponents()) {
            if (component.getId() == otherComponent.getId()) {
                continue;
            }

            if (component.collision(otherComponent)) {
                component.setX(oldX);
                component.setY(oldY);
            }
        }

        //Collision with pusher is harder
        //Depending on Collision Site, a push is necessary
        for (Pusher pusher : getPushers()) {
            if (pusher.getId() == 0 && component.collision(pusher)) {
                handleFirstPusherCollision(pusher, component, oldX, oldY);
            } else if (pusher.getId() == 1 && component.collision(pusher)) {
                handleSecondPusherCollision(pusher, component, oldX, oldY);
            }
        }

        //If it reached the end of site stop it.
        if(siteEnd.collision(component)) {
            component.setX(oldX);
            component.setY(oldY);
        }
    }

    private void handleSecondPusherCollision(Pusher pusher, Component component, float oldX, float oldY) {
        if (component.getMovingDirection() == Direction.RIGHT) {
            if (component.getX() - pusher.getX() > 2) {
                //TODO Errorhandling.
                System.err.println("Unerlaubter Zustand bei Pusher 2");
            }

            component.setX(oldX);
            component.setY(oldY);
        }
    }

    private void handleFirstPusherCollision(Pusher pusher, Component component, float oldX, float oldY) {
        if (component.getMovingDirection() == Direction.UP) { // In case the component presses against the pusher, reset position.
            if ((pusher.getY() + pusher.getHeight()) - component.getY() > 2) {
                //TODO Errorhandling.
                System.err.println("Unerlaubter Zustand bei Pusher 1");
            }

            component.setX(oldX);
            component.setY(oldY);
        }
    }

    private void simulatePushers(int deltaInMs, List<Pusher> pushers) {
        for (Pusher pusher : pushers) {

            if (pusher.getId() == 0) {
                if (pusher.getState() == Pusher.State.GOING_BACKWARD) {
                    pusher.setX(pusher.getX() - PUSHER_SPEED * deltaInMs);

                } else if (pusher.getState() == Pusher.State.GOING_FORWARD) {
                    pusher.setX(pusher.getX() + PUSHER_SPEED * deltaInMs);

                }
            } else if (pusher.getId() == 1) {
                if (pusher.getState() == Pusher.State.GOING_BACKWARD) {
                    pusher.setY(pusher.getY() - PUSHER_SPEED * deltaInMs);

                } else if (pusher.getState() == Pusher.State.GOING_FORWARD) {
                    pusher.setY(pusher.getY() + PUSHER_SPEED * deltaInMs);

                }
            }

            if (pusher.collision(pusher.getLowerTrigger())) {
                constructionSite.blockLowerTrigger(pusher);
            } else {
                constructionSite.unblockLowerTrigger(pusher);
            }

            if (pusher.collision(pusher.getUpperTrigger())) {
                constructionSite.blockUpperTrigger(pusher);
            } else {
                constructionSite.unblockUpperTrigger(pusher);
            }

            for (Component component : getComponents()) {
                if (component.getMovingDirection() == Direction.RESTING && pusher.collision(component)) {
                    if (pusher.getId() == 0) {
                        float diffX = (pusher.getX() + pusher.getWidth()) - component.getX();
                        component.setX(component.getX() + diffX);
                    } else if (pusher.getId() == 1) {
                        float diffY = (pusher.getY() + pusher.getHeight()) - component.getY();
                        component.setY(component.getY() + diffY);
                    }
                }
            }
        }
    }
}
