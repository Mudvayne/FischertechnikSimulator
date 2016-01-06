package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class GetSiteStateStatus extends Packet {

    public GetSiteStateStatus(int sequenceNumber) {
        super(0, 81, sequenceNumber, true);
    }
}
