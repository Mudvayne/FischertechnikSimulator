package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class GetWholeSiteStatus extends Packet {
    public static final int FUNCTION_ID = 83;

    public GetWholeSiteStatus(int sequenceNumber) {
        super(0, FUNCTION_ID, sequenceNumber, true);
    }
}
