package de.hm.cs.embedded.simulator.model.usb;

import com.sun.xml.internal.bind.v2.runtime.reflect.Lister;

import java.nio.ByteBuffer;

/**
 * Created by qriz on 1/6/16.
 */
public class HandleTool extends HandleActor {

    public static final int FUNCTION_ID = 78;

    public HandleTool(int sequenceNumber, int actorId, boolean activate) {
        super(FUNCTION_ID, sequenceNumber, actorId, activate);
    }

}
