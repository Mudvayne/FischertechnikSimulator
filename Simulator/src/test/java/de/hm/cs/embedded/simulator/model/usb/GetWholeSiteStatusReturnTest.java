package de.hm.cs.embedded.simulator.model.usb;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by qriz on 1/17/16.
 */
public class GetWholeSiteStatusReturnTest {

    @Test
    public void testPacket() {
        GetWholeSiteStatusReturn expected = new GetWholeSiteStatusReturn(5);
        GetWholeSiteStatusReturn actual = new GetWholeSiteStatusReturn(5);
        actual.fromByteArray(expected.toByteBuffer().array());

        assertEquals(expected.isFirstLightBarrierBlocked(), actual.isFirstLightBarrierBlocked());
        assertEquals(expected.isSecondLightBarrierBlocked(), actual.isSecondLightBarrierBlocked());
        assertEquals(expected.isThirdLightBarrierBlocked(), actual.isThirdLightBarrierBlocked());
        assertEquals(expected.isFourthLightBarrierBlocked(), actual.isFourthLightBarrierBlocked());
        assertEquals(expected.isFifthLightBarrierBlocked(), actual.isFifthLightBarrierBlocked());

        assertEquals(expected.isFirstToolRunning(), actual.isFirstToolRunning());
        assertEquals(expected.isSecondToolRunning(), actual.isSecondToolRunning());

        assertEquals(expected.isFirstTreadmillRunning(), actual.isFirstTreadmillRunning());
        assertEquals(expected.isSecondTreadmillRunning(), actual.isSecondTreadmillRunning());
        assertEquals(expected.isThirdTreadmillRunning(), actual.isThirdTreadmillRunning());
        assertEquals(expected.isFourthTreadmillRunning(), actual.isFourthTreadmillRunning());

        assertEquals(expected.isFirstPusherGoingForwards(), actual.isFirstPusherGoingForwards());
        assertEquals(expected.isFirstPusherGoingBackwards(), actual.isFirstPusherGoingBackwards());
        assertEquals(expected.isFirstPusherFrontActivated(), actual.isFirstPusherFrontActivated());
        assertEquals(expected.isFirstPusherBackActivated(), actual.isFirstPusherBackActivated());
        assertEquals(expected.isSecondPusherGoingForwards(), actual.isSecondPusherGoingForwards());
        assertEquals(expected.isSecondPusherGoingBackwards(), actual.isSecondPusherGoingBackwards());
        assertEquals(expected.isSecondPusherFrontActivated(), actual.isSecondPusherFrontActivated());
        assertEquals(expected.isSecondPusherBackActivated(), actual.isSecondPusherBackActivated());

        assertEquals(expected.getErrorCode(), actual.getErrorCode());
    }
}