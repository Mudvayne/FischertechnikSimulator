package de.hm.cs.embedded.simulator.model.usb;

import java.util.BitSet;

/**
 * Created by qriz on 1/6/16.
 */
public class GetWholeSiteStatusReturn extends Packet {
    public static final int FUNCTION_ID = 83;

    private BitSet status;

    public GetWholeSiteStatusReturn() {
        super(4, FUNCTION_ID, 0, false);
    }

    public boolean isFirstTreadmillRunning() {
        return status.get(0);
    }

    public boolean isSecondTreadmillRunning() {
        return status.get(1);
    }

    public boolean isThirdTreadmillRunning() {
        return status.get(2);
    }

    public boolean isFourthTreadmillRunning() {
        return status.get(3);
    }

    public boolean isFirstToolRunning() {
        return status.get(4);
    }

    public boolean isSecondToolRunning() {
        return status.get(5);
    }

    public boolean isFirstLightBarrierBlocked() {
        return status.get(6);
    }

    public boolean isSecondLightBarrierBlocked() {
        return status.get(7);
    }

    public boolean isThirdLightBarrierBlocked() {
        return status.get(8);
    }

    public boolean isFourthLightBarrierBlocked() {
        return status.get(9);
    }

    public boolean isFifthLightBarrierBlocked() {
        return status.get(10);
    }

    public boolean isFirstPusherGoingBackwards() {
        return status.get(11);
    }

    public boolean isFirstPusherGoingForwards() {
        return status.get(12);
    }

    public boolean isFirstPusherFrontActivated() {
        return status.get(13);
    }

    public boolean isFirstPusherBackActivated() {
        return status.get(14);
    }

    public boolean isSecondPusherGoingBackwards() {
        return status.get(15);
    }

    public boolean isSecondPusherGoingForwards() {
        return status.get(16);
    }

    public boolean isSecondPusherFrontActivated() {
        return status.get(17);
    }

    public boolean isSecondPusherBackActivated() {
        return status.get(18);
    }

    @Override
    public void fromByteArray(byte[] packet) {
        super.fromByteArray(packet);

        status = BitSet.valueOf(new byte[]{packet[9], packet[10], packet[11], packet[12]});
    }
}
