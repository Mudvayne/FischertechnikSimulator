package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class GetSiteStateStatus extends Packet {

    public static final int FUNCTION_ID = 81;

    public GetSiteStateStatus(int sequenceNumber) {
        super(0, FUNCTION_ID, sequenceNumber, true);
    }
}
