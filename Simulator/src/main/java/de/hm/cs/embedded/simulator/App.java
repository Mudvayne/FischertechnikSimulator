package de.hm.cs.embedded.simulator;

import de.hm.cs.embedded.simulator.logic.Logic;
import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.model.CModel;
import de.hm.cs.embedded.simulator.model.ConstructionSite;
import de.hm.cs.embedded.simulator.model.USBModel;
import de.hm.cs.embedded.simulator.view.KeyboardListener;
import de.hm.cs.embedded.simulator.view.Window;

/**
 * Created by qriz on 12/27/15.
 */
public class App {

    public static void main(String arg[]) throws InterruptedException {
        System.out.println(String.format("LibraryPath: %s", System.getProperty("java.library.path")));

        //ConstructionSite constructionSite = new SimpleModel();
        ConstructionSite constructionSite = new USBModel();
        Logic logic = new Simulation(constructionSite);
        logic.initConstructionSite();

        Window window = new Window(logic);
        window.init();
        window.addKeyListener(new KeyboardListener(logic, constructionSite));
        window.setVisible(true);

        long lastTime = System.currentTimeMillis();
        while (true) {
            long currentTime = System.currentTimeMillis();
            int delta = (int) (currentTime-lastTime);

            logic.updateSimulation(delta);
            constructionSite.update();
            window.update();

            lastTime = currentTime;
            Thread.sleep(1000);
        }
    }
}
