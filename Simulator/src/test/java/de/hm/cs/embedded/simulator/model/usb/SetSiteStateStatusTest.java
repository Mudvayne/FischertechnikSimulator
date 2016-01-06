package de.hm.cs.embedded.simulator.model.usb;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static org.junit.Assert.*;

/**
 * Created by qriz on 1/6/16.
 */
public class SetSiteStateStatusTest {

    @org.junit.Test
    public void testToByteBuffer() throws Exception {
        byte[] expected = buildExpected();

        SetSiteStateStatus siteState = new SetSiteStateStatus((byte)1, 2);
        byte[] actual = siteState.toByteBuffer().array();

        assertArrayEquals(expected, actual);
    }

    private byte[] buildExpected() {
        ByteBuffer longBuffer = ByteBuffer.allocate(8);
        longBuffer.order(ByteOrder.LITTLE_ENDIAN);
        longBuffer.putLong(Packet.BRICK_UID);

        ByteBuffer messageBuffer = ByteBuffer.allocate(9);
        messageBuffer.order(ByteOrder.LITTLE_ENDIAN);
        messageBuffer.putInt(0);      //UID
        messageBuffer.put((byte) 9);  //Packet Length
        messageBuffer.put((byte) 82);  //Function ID
        messageBuffer.put((byte) 24); //Sequence Number + Options
        messageBuffer.put((byte) 0);  //Flags

        messageBuffer.put((byte) 2);

        byte[] longArray = longBuffer.array();
        byte[] messageArray = messageBuffer.array();
        messageArray[0] = longArray[0];
        messageArray[1] = longArray[1];
        messageArray[2] = longArray[2];
        messageArray[3] = longArray[3];

        return messageArray;
    }
}