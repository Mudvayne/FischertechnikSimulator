package de.hm.cs.embedded.simulator.model.usb;

import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/7/16.
 */
public class HandleActor extends Packet {
    int actorId;
    boolean activate;

    public int getActorId() {
        return actorId;
    }

    public boolean isActivate() {
        return activate;
    }

    public HandleActor(int functionId, int sequenceNumber, int actorId, boolean activate) {
        super(2, functionId, sequenceNumber, false);
        this.actorId = actorId;
        this.activate = activate;
    }

    @Override
    public ByteBuffer toByteBuffer() {
        ByteBuffer messageBuffer = super.toByteBuffer();

        messageBuffer.put((byte) actorId);
        messageBuffer.put(activate ? TRUE_BYTE : FALSE_BYTE);

        return messageBuffer;
    }
}
