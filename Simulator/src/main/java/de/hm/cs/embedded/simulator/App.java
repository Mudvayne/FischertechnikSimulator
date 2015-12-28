package de.hm.cs.embedded.simulator;

import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.model.CModel;
import de.hm.cs.embedded.simulator.model.ConstructionSite;
import de.hm.cs.embedded.simulator.model.SimpleModel;
import de.hm.cs.embedded.simulator.view.Window;

/**
 * Created by qriz on 12/27/15.
 */
public class App {

    public static void main(String arg[]) throws InterruptedException {
        //ConstructionSite constructionSite = new SimpleModel();
        ConstructionSite constructionSite = new CModel();
        Simulation simulation = new Simulation(constructionSite);
        simulation.initConstructionSite();

        Window window = new Window(simulation);
        window.init();
        window.setVisible(true);

        long lastTime = System.currentTimeMillis();
        while (true) {
            long currentTime = System.currentTimeMillis();
            int delta = (int) (currentTime-lastTime);

            simulation.updateSimulation(delta);
            constructionSite.update();
            window.update();

            lastTime = currentTime;
            Thread.sleep(10);
        }
    }
}
