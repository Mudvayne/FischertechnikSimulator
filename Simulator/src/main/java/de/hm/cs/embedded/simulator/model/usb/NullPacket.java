package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class NullPacket extends Packet {

    public NullPacket() {
        super(0, 0, 0, false);
    }
}
