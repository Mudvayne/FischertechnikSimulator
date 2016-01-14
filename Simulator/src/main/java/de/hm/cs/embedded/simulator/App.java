package de.hm.cs.embedded.simulator;

import de.hm.cs.embedded.simulator.logic.Logic;
import de.hm.cs.embedded.simulator.logic.Simulation;
import de.hm.cs.embedded.simulator.model.CModel;
import de.hm.cs.embedded.simulator.model.ConstructionSite;
import de.hm.cs.embedded.simulator.model.SimpleModel;
import de.hm.cs.embedded.simulator.model.USBModel;
import de.hm.cs.embedded.simulator.view.KeyboardListener;
import de.hm.cs.embedded.simulator.view.Window;

import java.io.IOException;
import java.util.Properties;

/**
 * Created by qriz on 12/27/15.
 */
public class App {
    public static String MODEL = System.getProperty("model", "C");
    public static long DELTA = Long.valueOf(System.getProperty("delta", "10"));

    public static String COMPONENTS = null;

    static {
        try {
            Properties properties = new Properties();
            properties.load(Properties.class.getResourceAsStream("./../../components.properties"));

            COMPONENTS = properties.getProperty("components", null);
        } catch (Exception e) {

            COMPONENTS = null;
        }
    }

    public static void main(String arg[]) throws InterruptedException {
        System.out.println(String.format("LibraryPath: %s", System.getProperty("java.library.path")));
        System.out.println(String.format("Model: %s", MODEL));
        System.out.println(String.format("DeltaStep: %d", DELTA));

        ConstructionSite constructionSite = null;
        if (MODEL.equals("USB")) {
            constructionSite = new USBModel();
        } else if (MODEL.equals("C")) {
            constructionSite = new CModel();
        } else {
            constructionSite = new SimpleModel();
        }

        Logic logic = new Simulation(constructionSite);
        logic.initConstructionSite(COMPONENTS);

        Window window = new Window(logic);
        window.init();
        window.addKeyListener(new KeyboardListener(logic, constructionSite));
        window.setVisible(true);

        long lastTime = System.currentTimeMillis();
        while (true) {
            long currentTime = System.currentTimeMillis();
            int delta = (int) (currentTime - lastTime);

            logic.updateSimulation(delta);
            constructionSite.update();
            window.update();

            lastTime = currentTime;
            Thread.sleep(DELTA);
        }
    }
}
