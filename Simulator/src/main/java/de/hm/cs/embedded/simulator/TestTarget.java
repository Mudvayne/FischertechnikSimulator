package de.hm.cs.embedded.simulator;

import de.hm.cs.embedded.simulator.model.usb.*;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by qriz on 1/7/16.
 */
public class TestTarget {

    public static void main(String[] args) throws IOException, InterruptedException {
        ServerSocket serverSocket = new ServerSocket(4223);

        NullPacket headerPacket = new NullPacket();


        while(true) {
            Socket socket = serverSocket.accept();

            while(socket.isConnected()) {
                byte[] headerArray = new byte[8];
                socket.getInputStream().read(headerArray);
                headerPacket.fromByteArray(headerArray);

                if(headerPacket.getPacketLength() - 8 > 0) {
                    byte[] payload = new byte[headerPacket.getPacketLength() - 8];
                    socket.getInputStream().read(payload);
                }

                GetSiteStateStatusReturn sitestate = new GetSiteStateStatusReturn(headerPacket.getSequenceNumber());
                GetWholeSiteStatusReturn wholeState = new GetWholeSiteStatusReturn(headerPacket.getSequenceNumber());

                if(headerPacket.getFunctionId() == GetSiteStateStatus.FUNCTION_ID) {
                    byte[] array = sitestate.toByteBuffer().array();
                    socket.getOutputStream().write(array);
                } else if (headerPacket.getFunctionId() == GetWholeSiteStatus.FUNCTION_ID){
                    byte[] array = wholeState.toByteBuffer().array();
                    socket.getOutputStream().write(array);
                }
            }
        }
    }
}
