package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.*;
import org.apache.commons.lang.Validate;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by qriz on 12/27/15.
 */
public abstract class LocalModel implements ConstructionSite {
    public static final int LIGHT_BARRIER_COUNT = 5;
    public static final int TREADMILL_COUNT = 4;
    public static final int PUSHER_COUNT = 2;
    public static final int TOOL_COUNT = 2;

    private final Map<Integer, String> errorCodeMap;

    private List<LightBarrier> lightBarriers;
    private List<Treadmill> treadmills;
    private List<Pusher> pushers;
    private List<Tool> tools;

    private boolean panicSwitchPressed;
    protected SiteState siteState;
    protected int errorCode;

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

    public SiteState getSiteState() {
        return siteState;
    }

    public boolean isPanicSwitchPressed() {
        return panicSwitchPressed;
    }

    public int getErrorCode() {
        return errorCode;
    }

    public LocalModel() {
        errorCodeMap = new HashMap<>();
        errorCodeMap.put(0, "Everything fine.");
        errorCodeMap.put(100, "Panic switch was pressed");
        errorCodeMap.put(200, "Reached not implemented code");

        errorCodeMap.put(1, "Unexpected item in light barrier 1");
        errorCodeMap.put(2, "Unexpected item in light barrier 2");
        errorCodeMap.put(3, "Unexpected item in light barrier 3");
        errorCodeMap.put(4, "Unexpected item in light barrier 4");
        errorCodeMap.put(5, "Unexpected item in light barrier 5");

        errorCodeMap.put(10, "Stage 1 expected an item which never arrived");
        errorCodeMap.put(30, "Stage 3 expected an item which never arrived");
        errorCodeMap.put(40, "Stage 4 expected an item which never arrived");
        errorCodeMap.put(60, "Stage 6 expected an item which never arrived");
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

        this.panicSwitchPressed = false;
        this.siteState = SiteState.DIAGNOSTIC;
        this.errorCode = 0;
    }

    @Override
    public String decodeErrorCode(int errorCode) {
        return errorCodeMap.getOrDefault(errorCode, "Unknown error");
    }

    private void validateListCount(List objects, int count) {
        Validate.notEmpty(objects);
        Validate.isTrue(objects.size() == count);
    }

    protected void updatePanicButtonPressed() {
        Pusher pusherA = getPushers().get(0);
        Pusher pusherB = getPushers().get(1);

        boolean pusherATriggered = pusherA.getUpperTrigger().isTriggered() && pusherA.getLowerTrigger().isTriggered();
        boolean pusherBTriggered = pusherB.getUpperTrigger().isTriggered() && pusherB.getLowerTrigger().isTriggered();

        if(pusherATriggered && pusherBTriggered) {
            pressPanicSwitch();
        } else {
            releasePanicSwitch();
        }
    }

    public void pressPanicSwitch() {
        if(!panicSwitchPressed) {
            blockLowerTrigger(getPushers().get(0));
            blockUpperTrigger(getPushers().get(0));

            blockLowerTrigger(getPushers().get(1));
            blockUpperTrigger(getPushers().get(1));

            panicSwitchPressed = true;
        }
    }

    public void releasePanicSwitch() {
        if(panicSwitchPressed) {
            unblockLowerTrigger(getPushers().get(0));
            unblockUpperTrigger(getPushers().get(0));

            unblockLowerTrigger(getPushers().get(1));
            unblockUpperTrigger(getPushers().get(1));

            panicSwitchPressed = false;
        }
    }
}
