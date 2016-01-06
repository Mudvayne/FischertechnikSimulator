package de.hm.cs.embedded.simulator.model.usb;

/**
 * Created by qriz on 1/6/16.
 */
public class GetSiteStateStatusReturn extends Packet {

    int status;

    public int getCurrentStatus() {
        return status;
    }

    public GetSiteStateStatusReturn() {
        super(0, 81, 0, false);
    }

    @Override
    public void fromByteArray(byte[] packet) {
        super.fromByteArray(packet);

        status = packet[9];
    }
}
