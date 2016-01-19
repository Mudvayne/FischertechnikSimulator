package de.hm.cs.embedded.simulator.model;

import de.hm.cs.embedded.simulator.logic.objects.*;
import de.hm.cs.embedded.simulator.model.usb.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/6/16.
 */
public class USBModel extends LocalModel {
    public static final String HOSTNAME = System.getProperty("hostname", "localhost");
    public static final int PORT = Integer.valueOf(System.getProperty("port", "4223"));
    public static final int TIMEOUT_MS = Integer.valueOf(System.getProperty("timeoutInMS", "1000"));
    public static final int MAX_MESSAGE_COUNT = 20;

    private OutputStream outputStream;
    private InputStream inputStream;

    private int sequenceNumber;

    public USBModel() {
        sequenceNumber = 1;

        try {
            Socket socket = new Socket();
            socket.setSoTimeout(TIMEOUT_MS);
            socket.connect(new InetSocketAddress(HOSTNAME, PORT));

            outputStream = socket.getOutputStream();
            inputStream = socket.getInputStream();

        } catch (Exception e) {
            throw new IllegalStateException("Could not connect to brickd.", e);
        }
    }

    private int calcNewSequenceNumber() {
        sequenceNumber++;
        if(sequenceNumber == 16) {
            sequenceNumber = 1;
        }

        return sequenceNumber;
    }

    private void writePacket(ByteBuffer byteBuffer) {
        try {
            outputStream.write(byteBuffer.array());

        } catch (IOException e) {
            throw new IllegalStateException("Writing a Message failed.", e);
        }
    }

    private void readPacket(int sequenceNumber, Packet packet) {
        try {
            NullPacket headerPacket = new NullPacket();
            byte[] headerArray = new byte[headerPacket.getPacketLength()];
            boolean messageFound = false;
            int msgCount = 0;

            while(!messageFound || msgCount < MAX_MESSAGE_COUNT) {
                inputStream.read(headerArray);
                msgCount++;

                headerPacket.fromByteArray(headerArray);

                if (headerPacket.getSequenceNumber() == sequenceNumber) {
                    messageFound = true;

                    int payloadLength = packet.getPacketLength() - 8;
                    byte[] messageArray;

                    if (payloadLength > 0) {
                        byte[] payloadArray = new byte[payloadLength];

                        inputStream.read(payloadArray);

                        messageArray = new byte[packet.getPacketLength()];
                        int i = 0;
                        for (; i < headerArray.length; i++) {
                            messageArray[i] = headerArray[i];
                        }
                        for(; i < messageArray.length; i++) {
                            messageArray[i] = payloadArray[i-headerArray.length];
                        }
                    } else {
                        messageArray = headerArray;
                    }

                    packet.fromByteArray(messageArray);
                }
            }

        } catch (IOException e) {
            
        }
    }

    public void update() {
        //Construction-Site has its own thread.
        updateValuesOnly();
    }

    public void updateValuesOnly() {
        int sequenceNumber = calcNewSequenceNumber();
        GetWholeSiteStatus status = new GetWholeSiteStatus(sequenceNumber);
        writePacket(status.toByteBuffer());
        GetWholeSiteStatusReturn response = new GetWholeSiteStatusReturn(sequenceNumber);
        readPacket(sequenceNumber, response);

        sequenceNumber = calcNewSequenceNumber();
        GetSiteStateStatus siteStateStatus = new GetSiteStateStatus(sequenceNumber);
        writePacket(siteStateStatus.toByteBuffer());
        GetSiteStateStatusReturn siteStateResponse = new GetSiteStateStatusReturn(sequenceNumber);
        readPacket(sequenceNumber, siteStateResponse);

        getTreadmills().get(0).setActivated(response.isFirstTreadmillRunning());
        getTreadmills().get(1).setActivated(response.isSecondTreadmillRunning());
        getTreadmills().get(2).setActivated(response.isThirdTreadmillRunning());
        getTreadmills().get(3).setActivated(response.isFourthTreadmillRunning());

        getTools().get(0).setActivated(response.isFirstToolRunning());
        getTools().get(1).setActivated(response.isSecondToolRunning());

        getLightBarriers().get(0).setBlocked(response.isFirstLightBarrierBlocked());
        getLightBarriers().get(1).setBlocked(response.isSecondLightBarrierBlocked());
        getLightBarriers().get(2).setBlocked(response.isThirdLightBarrierBlocked());
        getLightBarriers().get(3).setBlocked(response.isFourthLightBarrierBlocked());
        getLightBarriers().get(4).setBlocked(response.isFifthLightBarrierBlocked());

        if(response.isFirstPusherGoingBackwards()) {
            getPushers().get(0).setState(Pusher.State.GOING_BACKWARD);
        } else if (response.isFirstPusherGoingForwards()) {
            getPushers().get(0).setState(Pusher.State.GOING_FORWARD);
        } else {
            getPushers().get(0).setState(Pusher.State.NOT_MOVING);
        }
        getPushers().get(0).getUpperTrigger().setTriggered(response.isFirstPusherFrontActivated());
        getPushers().get(0).getLowerTrigger().setTriggered(response.isFirstPusherBackActivated());

        if(response.isSecondPusherGoingBackwards()) {
            getPushers().get(1).setState(Pusher.State.GOING_BACKWARD);
        } else if (response.isSecondPusherGoingForwards()) {
            getPushers().get(1).setState(Pusher.State.GOING_FORWARD);
        } else {
            getPushers().get(1).setState(Pusher.State.NOT_MOVING);
        }
        getPushers().get(1).getUpperTrigger().setTriggered(response.isSecondPusherFrontActivated());
        getPushers().get(1).getLowerTrigger().setTriggered(response.isSecondPusherBackActivated());

        siteState = convertIntToState(siteStateResponse.getCurrentStatus());

        updatePanicButtonPressed();
        errorCode = response.getErrorCode();
    }

    public void blockUpperTrigger(Pusher pusher) {
        //Do nothing.
    }

    public void unblockUpperTrigger(Pusher pusher) {
        //Do nothing.
    }

    public void blockLowerTrigger(Pusher pusher) {
        //Do nothing.
    }

    public void unblockLowerTrigger(Pusher pusher) {
        //Do nothing.
    }

    public void blockLightBarrier(LightBarrier lightBarrier) {
        //Do nothing.
    }

    public void unblockLightBarrier(LightBarrier lightBarrier) {
        //Do nothing.
    }

    public void changeSiteState(SiteState siteState) {
        SetSiteStateStatus status = new SetSiteStateStatus(calcNewSequenceNumber(), convertStateToInt(siteState));
        writePacket(status.toByteBuffer());

        this.siteState = siteState;
    }

    public void handleTreadmill(Treadmill treadmill, boolean activate) {
        HandleTreadmill handle = new HandleTreadmill(calcNewSequenceNumber(), treadmill.getId(), activate);
        writePacket(handle.toByteBuffer());

        treadmill.setActivated(activate);
    }

    public void handleTool(Tool tool, boolean activate) {
        HandleTool handle = new HandleTool(calcNewSequenceNumber(), tool.getId(), activate);
        writePacket(handle.toByteBuffer());

        tool.setActivated(activate);
    }

    public void handlePusher(Pusher pusher, Pusher.State state) {
        HandlePusher handle = new HandlePusher(calcNewSequenceNumber(), pusher.getId(), state);
        writePacket(handle.toByteBuffer());

        pusher.setState(state);
    }

    private int convertStateToInt(SiteState siteState) {
        switch (siteState) {
            case PANIC_SWITCH:
                return 0;
            case DIAGNOSTIC:
                return 1;
            case START:
                return 2;
            case RUNNING:
                return 3;
            case REST:
                return 4;
            case STOP:
                return 5;

            default:
                return 0;
        }
    }

    private SiteState convertIntToState(int state) {
        switch (state) {
            case 0:
                return SiteState.PANIC_SWITCH;
            case 1:
                return SiteState.DIAGNOSTIC;
            case 2:
                return SiteState.START;
            case 3:
                return SiteState.RUNNING;
            case 4:
                return SiteState.REST;
            case 5:
                return SiteState.STOP;

            default:
                return SiteState.PANIC_SWITCH;
        }
    }
}
