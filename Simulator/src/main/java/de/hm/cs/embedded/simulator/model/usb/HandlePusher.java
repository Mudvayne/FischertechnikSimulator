package de.hm.cs.embedded.simulator.model.usb;

import de.hm.cs.embedded.simulator.logic.objects.Pusher;

import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/6/16.
 */
public class HandlePusher extends Packet {

    private int pusherId;
    private Pusher.State state;

    public HandlePusher(int sequenceNumber, int pusherId, Pusher.State state) {
        super(4, 79, sequenceNumber, false);

        this.pusherId = pusherId;
        this.state = state;
    }

    @Override
    public ByteBuffer toByteBuffer() {
        ByteBuffer messageBuffer = super.toByteBuffer();

        messageBuffer.put((byte) pusherId);
        messageBuffer.put(state == Pusher.State.GOING_FORWARD ? TRUE_BYTE : FALSE_BYTE);
        messageBuffer.put(state == Pusher.State.GOING_BACKWARD ? TRUE_BYTE : FALSE_BYTE);
        messageBuffer.put(state == Pusher.State.NOT_MOVING ? TRUE_BYTE : FALSE_BYTE);

        return messageBuffer;
    }
}
