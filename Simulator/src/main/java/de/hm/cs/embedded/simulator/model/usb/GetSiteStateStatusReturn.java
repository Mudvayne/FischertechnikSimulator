package de.hm.cs.embedded.simulator.model.usb;

import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/6/16.
 */
public class GetSiteStateStatusReturn extends Packet {

    int status;

    public int getCurrentStatus() {
        return status;
    }

    public GetSiteStateStatusReturn(int sequenceNumber) {
        super(1, 81, sequenceNumber, false);
    }

    @Override
    public void fromByteArray(byte[] packet) {
        super.fromByteArray(packet);

        status = packet[8];
    }

    @Override
    public ByteBuffer toByteBuffer() {
        ByteBuffer messageBuffer = super.toByteBuffer();

        messageBuffer.put((byte) 9);

        return messageBuffer;
    }
}
