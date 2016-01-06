package de.hm.cs.embedded.simulator.model.usb;

import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/6/16.
 */
public class SetSiteStateStatus extends Packet {
    private byte newStatus;

    public SetSiteStateStatus(int sequenceNumber, int newStatus) {
        super(1, 82, sequenceNumber, true);

        this.newStatus = (byte) newStatus;
    }


    @Override
    public ByteBuffer toByteBuffer() {
        ByteBuffer messageBuffer = super.toByteBuffer();

        messageBuffer.put(newStatus);

        return messageBuffer;
    }
}
