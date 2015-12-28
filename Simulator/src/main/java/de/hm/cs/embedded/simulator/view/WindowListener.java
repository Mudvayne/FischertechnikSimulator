package de.hm.cs.embedded.simulator.view;

import java.awt.event.WindowEvent;

/**
 * Created by qriz on 12/28/15.
 */
public class WindowListener implements java.awt.event.WindowListener {
    public void windowOpened(WindowEvent windowEvent) {
        //Do nothing.
    }

    public void windowClosing(WindowEvent windowEvent) {
        System.exit(0);
    }

    public void windowClosed(WindowEvent windowEvent) {
        //Do nothing.
    }

    public void windowIconified(WindowEvent windowEvent) {
        //Do nothing.
    }

    public void windowDeiconified(WindowEvent windowEvent) {
        //Do nothing.
    }

    public void windowActivated(WindowEvent windowEvent) {
        //Do nothing.
    }

    public void windowDeactivated(WindowEvent windowEvent) {
        //Do nothing.
    }
}
