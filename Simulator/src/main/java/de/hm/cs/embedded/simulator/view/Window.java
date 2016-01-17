package de.hm.cs.embedded.simulator.view;

import de.hm.cs.embedded.simulator.logic.Logic;
import de.hm.cs.embedded.simulator.logic.objects.*;

import java.awt.*;
import java.util.ArrayList;

/**
 * Created by qriz on 12/27/15.
 */
public class Window extends Frame {
    public static final boolean DISPLAY_LIGHT_SENSOR = true;

    private Logic logic;

    private SiteState currentState;
    private int currentErrorCode;
    private boolean panicButtonPressed;
    private java.util.List<Drawable> drawables;
    private java.util.List<Drawable> components;

    public Window(Logic logic) {
        super("Foobar");
        setSize(1130, 660);
        setLayout(null);
        addWindowListener(new WindowListener());

        this.logic = logic;
        this.currentState = logic.getCurrentSiteState();
        this.panicButtonPressed = false;
        this.currentErrorCode = 0;
        this.drawables = new ArrayList<>();
        this.components = new ArrayList<>();
    }

    public void init() {
        drawables.add(new Drawable(new NullObject(-1, 80, 120, 100, 100)));
        drawables.add(new Drawable(new NullObject(-1, 850, 120, 100, 100)));

        for (SiteObject object : logic.getTools()) {
            drawables.add(new Drawable(object));
        }

        for (Pusher pusher : logic.getPushers()) {
            drawables.add(new Drawable(pusher));
            drawables.add(new Drawable(pusher.getUpperTrigger()));
            drawables.add(new Drawable(pusher.getLowerTrigger()));
        }

        for (SiteObject object : logic.getTreadmills()) {
            drawables.add(new Drawable(object));
        }

        for (SiteObject object : logic.getLightBarriers()) {
            drawables.add(new Drawable(object));
        }

        repaint();
    }

    public void update() {
        boolean someThingChanged = false;
        for (Drawable drawable : drawables) {
            if (drawable.update()) {
                someThingChanged = true;
            }
        }
        for (int i = logic.getComponents().size() - 1; i >= 0; i--) {
            SiteObject object = logic.getComponents().get(i);

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

            for (SiteObject object : logic.getComponents()) {
                if (drawable.getObject() == object) {
                    stillExisting = true;
                }
            }

            if (!stillExisting) {
                someThingChanged = true;
                components.remove(drawable);
            }
        }

        if (currentState != logic.getCurrentSiteState()) {
            currentState = logic.getCurrentSiteState();
            someThingChanged = true;
        }

        if (panicButtonPressed != logic.isPanicButtonPressed()) {
            panicButtonPressed = logic.isPanicButtonPressed();
            someThingChanged = true;
        }

        if (currentErrorCode != logic.getErrorCode()) {
            currentErrorCode = logic.getErrorCode();
            someThingChanged = true;
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

        String siteState = currentState.toString();
        int fontSize = 16;

        graphics.setFont(new Font(Font.MONOSPACED, Font.BOLD, fontSize));
        graphics.setColor(Color.BLACK);
        graphics.drawString(siteState, fontSize, fontSize * 3);
        graphics.drawString(logic.decodeErrorCode(currentErrorCode), fontSize, 2 * (fontSize * 2));

        graphics.setColor(panicButtonPressed ? Color.RED : Color.GREEN);
        graphics.fillOval(880, 40, 40, 40);
        graphics.setColor(Color.BLACK);
        graphics.drawOval(880, 40, 40, 40);

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
}
