package de.hm.cs.embedded.simulator.model.usb;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by qriz on 1/6/16.
 */
public class GetWholeSiteStatusTest {

    @Test
    public void testFromByte() {
        GetWholeSiteStatus expected = new GetWholeSiteStatus(9);
        byte[] byteArray = expected.toByteBuffer().array();

        GetWholeSiteStatus actual = new GetWholeSiteStatus(0);
        actual.fromByteArray(byteArray);

        assertEquals(expected.getPacketLength(), actual.getPacketLength());
        assertEquals(expected.getFunctionId(), actual.getFunctionId());
        assertEquals(expected.getSequenceNumber(), actual.getSequenceNumber());
        assertEquals(expected.isResponse(), actual.isResponse());
        assertEquals(expected.getFlags(), actual.getFlags());
    }

}