package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.LightBarrier;
import de.hm.cs.embedded.simulator.logic.objects.Pusher;
import de.hm.cs.embedded.simulator.logic.objects.Tool;
import de.hm.cs.embedded.simulator.logic.objects.Treadmill;
import org.apache.commons.lang.Validate;

import java.util.Arrays;
import java.util.List;

/**
 * Created by qriz on 12/27/15.
 */
public abstract class LocalModel implements ConstructionSite {
    public static final int LIGHT_BARRIER_COUNT = 5;
    public static final int TREADMILL_COUNT = 4;
    public static final int PUSHER_COUNT = 2;
    public static final int TOOL_COUNT = 2;

    private List<LightBarrier> lightBarriers;
    private List<Treadmill> treadmills;
    private List<Pusher> pushers;
    private List<Tool> tools;

    public List<LightBarrier> getLightBarriers() {
        return lightBarriers;
    }

    public List<Treadmill> getTreadmills() {
        return treadmills;
    }

    public List<Pusher> getPushers() {
        return pushers;
    }

    public List<Tool> getTools() {
        return tools;
    }

    public void init(List<LightBarrier> lightBarriers, List<Treadmill> treadmills, List<Pusher> pushers, List<Tool> tools) {
        validateListCount(lightBarriers, LIGHT_BARRIER_COUNT);
        validateListCount(treadmills, TREADMILL_COUNT);
        validateListCount(pushers, PUSHER_COUNT);
        validateListCount(tools, TOOL_COUNT);

        this.lightBarriers = lightBarriers;
        this.treadmills = treadmills;
        this.pushers = pushers;
        this.tools = tools;
    }

    private void validateListCount(List objects, int count) {
        Validate.notEmpty(objects);
        Validate.isTrue(objects.size() == count);
    }
}
