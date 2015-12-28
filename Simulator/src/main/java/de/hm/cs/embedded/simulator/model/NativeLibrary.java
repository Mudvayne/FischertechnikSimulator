package de.hm.cs.embedded.simulator.model;

/**
 * Created by qriz on 12/28/15.
 */
public class NativeLibrary {
    public static final String WUBER_TECHNIK_LIB = "Wuber";

    static {
        System.loadLibrary(WUBER_TECHNIK_LIB);
    }

    native void callUpdate();
    native boolean callLightBarrierBlocked(int id);
    native boolean callPusherUpperBlocked(int id);
    native boolean callPusherLowerBlocked(int id);
    native boolean callPusherRunningBackwards(int id);
    native boolean callPusherRunningForwards(int id);
    native boolean callPusherInactive(int id);
    native boolean callToolRunning(int id);
    native boolean callTreadmillRunning(int id);
    native void callBlockUpperTrigger(int id);
    native void callUnblockUpperTrigger(int id);
    native void callBlockLowerTrigger(int id);
    native void callUnblockLowerTrigger(int id);
    native void callBlockLightBarrier(int id);
    native void callUnblockLightBarrier(int id);
}
