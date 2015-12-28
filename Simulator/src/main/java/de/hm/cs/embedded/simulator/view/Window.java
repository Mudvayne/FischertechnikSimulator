package de.hm.cs.embedded.simulator.view;

import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.logic.objects.*;

import java.awt.*;
import java.util.ArrayList;

/**
 * Created by qriz on 12/27/15.
 */
public class Window extends Frame {
    public static final boolean DISPLAY_LIGHT_SENSOR = true;

    private Simulation simulation;
    private java.util.List<Drawable> drawables;
    private java.util.List<Drawable> components;


    public Window(Simulation simulation) {
        super("Foobar");
        setSize(1130, 660);
        setLayout(null);
        addKeyListener(new KeyboardListener(this));
        addWindowListener(new WindowListener());

        this.simulation = simulation;
        this.drawables = new ArrayList<Drawable>();
        this.components = new ArrayList<Drawable>();
    }

    public void init() {
        drawables.add(new Drawable(new NullObject(-1, 80, 120, 100, 100)));
        drawables.add(new Drawable(new NullObject(-1, 850, 120, 100, 100)));

        for (SiteObject object : simulation.getTools()) {
            drawables.add(new Drawable(object));
        }

        for (Pusher pusher : simulation.getPushers()) {
            drawables.add(new Drawable(pusher));
            drawables.add(new Drawable(pusher.getUpperTrigger()));
            drawables.add(new Drawable(pusher.getLowerTrigger()));
        }

        for (SiteObject object : simulation.getTreadmills()) {
            drawables.add(new Drawable(object));
        }

        for (SiteObject object : simulation.getLightBarriers()) {
            drawables.add(new Drawable(object));
        }

        drawables.add(new Drawable(new NullObject(-1, 80, 147, 100, 2)));
        drawables.add(new Drawable(new NullObject(-1, 920, 120, 2, 100)));

        repaint();
    }

    public void update() {
        boolean someThingChanged = false;
        for (Drawable drawable : drawables) {
            if (drawable.update()) {
                someThingChanged = true;
            }
        }
        for (int i = simulation.getComponents().size() - 1; i >= 0; i--) {
            SiteObject object = simulation.getComponents().get(i);

            Drawable drawable = null;
            for (Drawable draw : components) {
                if (draw.getObject() == object) {
                    drawable = draw;
                    break;
                }
            }

            if (drawable == null) {
                drawable = new Drawable(object);
                components.add(drawable);
                someThingChanged = true;
            }

            if (drawable.update()) {
                someThingChanged = true;
            }
        }

        for (int i = components.size() - 1; i >= 0; i--) {
            Drawable drawable = components.get(i);
            boolean stillExisting = false;

            for (SiteObject object : simulation.getComponents()) {
                if (drawable.getObject() == object) {
                    stillExisting = true;
                }
            }

            if (!stillExisting) {
                someThingChanged = true;
                components.remove(drawable);
            }
        }

        if (someThingChanged) {
            this.repaint();
        }
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        paintComponents(g);
    }

    @Override
    public void paintComponents(Graphics graphics) {
        super.paintComponents(graphics);

        for (Drawable object : drawables) {
            paintDrawable(graphics, object);

            if (DISPLAY_LIGHT_SENSOR) {
                if (object.getObject() instanceof LightBarrier) {
                    paintDrawable(graphics, new Drawable(((LightBarrier) object.getObject()).getSensor()));
                }
            }
        }

        for (Drawable object : components) {
            paintDrawable(graphics, object);
        }

        Toolkit.getDefaultToolkit().sync();
    }

    private void paintDrawable(Graphics graphics, Drawable object) {
        int x = object.getX();
        int y = object.getY();
        int width = object.getWidth();
        int height = object.getHeight();

        graphics.setColor(Color.BLACK);
        if (object.getObject() instanceof de.hm.cs.embedded.simulator.logic.objects.Component) {
            de.hm.cs.embedded.simulator.logic.objects.Component component = (de.hm.cs.embedded.simulator.logic.objects.Component) object.getObject();
            graphics.fillOval(x, y, width, height);
            graphics.setColor(Color.WHITE);
            graphics.drawString(String.valueOf(component.getId()), x + width / 2, y + height / 2);

        } else {
            if (object.isTriggeredOrActivated()) {
                graphics.setColor(Color.GREEN);
                graphics.fillRect(x, y, width, height);
            }

            graphics.setColor(Color.BLACK);
            graphics.drawRect(x, y, width, height);
        }
    }

    public void addComponent() {
        simulation.addComponent();
    }

    public void pusher(int index, int action) {
        Pusher pusher = simulation.getPushers().get(index);
        pusher.setState(action == 0 ? Pusher.State.GOING_BACKWARD : Pusher.State.GOING_FORWARD);
    }

    public void treadmill(int index) {
        Treadmill treadmill = simulation.getTreadmills().get(index);
        treadmill.setActivated(!treadmill.isActivated());
    }

    public void removeComponent(int id) {
        simulation.removeComponent(id);
    }
}
