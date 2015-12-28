package de.hm.cs.embedded.simulator.logic;

import de.hm.cs.embedded.simulator.logic.objects.*;
import de.hm.cs.embedded.simulator.model.ConstructionSite;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by qriz on 12/27/15.
 */
public class Simulation {
    public static final float TREADMILL_SPEED = 50.0f / 5000.0f;
    public static final float PUSHER_SPEED = 16.0f / 1500.0f;

    private ConstructionSite constructionSite;
    private List<Component> components;
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

    public Simulation(ConstructionSite constructionSite) {
        this.constructionSite = constructionSite;
    }

    public void initConstructionSite() {
        components = new ArrayList<Component>();
        int id = 0;


        List<LightBarrier> lightBarriers = Arrays.asList(new LightBarrier(id++, 26, 78, new LightBarrierSensor(6, 80, true)),
                new LightBarrier(id++, 26, 48, new LightBarrierSensor(6, 50, true)),
                new LightBarrier(id++, 49, 40, new LightBarrierSensor(51, 20, false)),
                new LightBarrier(id++, 99, 40, new LightBarrierSensor(101, 20, false)),
                new LightBarrier(id, 146, 58, new LightBarrierSensor(126, 60, true)));

        id = 0;
        List<Treadmill> treadmills = Arrays.asList(new Treadmill(id++, 6, 40, Treadmill.TREADMILL_SHORTER_SIZE, Treadmill.TREADMILL_LONGER_SIZE, Direction.UP),
                new Treadmill(id++, 26, 20, Treadmill.TREADMILL_LONGER_SIZE, Treadmill.TREADMILL_SHORTER_SIZE, Direction.RIGHT),
                new Treadmill(id++, 76, 20, Treadmill.TREADMILL_LONGER_SIZE, Treadmill.TREADMILL_SHORTER_SIZE, Direction.RIGHT),
                new Treadmill(id, 126, 40, Treadmill.TREADMILL_SHORTER_SIZE, Treadmill.TREADMILL_LONGER_SIZE, Direction.DOWN));

        id = 0;
        Trigger firstUpperTrigger = new Trigger(21, 13);
        Trigger firstLowerTrigger = new Trigger(-5, 13);
        Trigger secondUpperTrigger = new Trigger(147, 35);
        Trigger secondLowerTrigger = new Trigger(147, 8);
        List<Pusher> pushers = Arrays.asList(new Pusher(id++, -6, 10, Pusher.PUSHER_SHORTER_SIZE, Pusher.PUSHER_LONGER_SIZE, firstUpperTrigger, firstLowerTrigger),
                new Pusher(id, 126, 12, Pusher.PUSHER_LONGER_SIZE, Pusher.PUSHER_SHORTER_SIZE, secondUpperTrigger, secondLowerTrigger));

        id = 0;
        List<Tool> tools = Arrays.asList(new Tool(id++, 41, 0),
                new Tool(id, 91, 0));

        constructionSite.init(lightBarriers, treadmills, pushers, tools);
    }

    public void addComponent() {
        Component newComponent = new Component(componentCount, 6, 70);
        boolean startOccupied = false;

        for (Component component : getComponents()) {
            if (component.collision(newComponent)) {
                startOccupied = true;
            }
        }

        if (!startOccupied) {
            componentCount++;
            componentCount = componentCount % 10;
            components.add(newComponent);
        }
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
        simulateTreadmill(deltaInMs, getTreadmills());
        simulateLightBarriers(getLightBarriers());
        simulatePushers(deltaInMs, getPushers());
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
