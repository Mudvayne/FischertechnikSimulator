package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class HandleTreadmill extends HandleActor {

    public static final int FUNCTION_ID = 77;

    public HandleTreadmill(int sequenceNumber, int actorId, boolean activate) {
        super(FUNCTION_ID, sequenceNumber, actorId, activate);
    }
}
