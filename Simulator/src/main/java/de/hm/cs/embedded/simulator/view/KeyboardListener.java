package de.hm.cs.embedded.simulator.view;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/**
 * Created by qriz on 12/27/15.
 */
public class KeyboardListener implements KeyListener {
    public Window window;

    public KeyboardListener(Window window) {
        this.window = window;
    }

    public void keyTyped(KeyEvent keyEvent) {

        if (keyEvent.getKeyChar() == ' ') {
            window.addComponent();
        } else if ((int)keyEvent.getKeyChar() >= (int)'0' && (int)keyEvent.getKeyChar() <= (int)'9') {
            window.removeComponent(keyEvent.getKeyChar() - '0');
        }

        //TODO: ab hier hacks!!!
        if (keyEvent.getKeyChar() == 'q') {
            window.pusher(0, 0);

        } else if (keyEvent.getKeyChar() == 'w') {
            window.pusher(0, 1);

        } else if (keyEvent.getKeyChar() == 'a') {
            window.treadmill(0);

        } else if (keyEvent.getKeyChar() == 'e') {
            window.treadmill(1);

        } else if (keyEvent.getKeyChar() == 'r') {
            window.treadmill(2);

        } else if (keyEvent.getKeyChar() == 't') {
            window.pusher(1, 0);

        } else if (keyEvent.getKeyChar() == 'y') {
            window.pusher(1, 1);

        } else if (keyEvent.getKeyChar() == 'h') {
            window.treadmill(3);

        }
    }

    public void keyPressed(KeyEvent keyEvent) {
        //Do nothing
    }

    public void keyReleased(KeyEvent keyEvent) {
        //Do nothing
    }
}
