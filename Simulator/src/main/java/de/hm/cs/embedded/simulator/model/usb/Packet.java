package de.hm.cs.embedded.simulator.model.usb;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.BitSet;

/**
 * Created by qriz on 1/6/16.
 */
public abstract class Packet {
    //Possible UID's    3493402161    3568702513
    public static final Long BRICK_UID = Long.valueOf(System.getProperty("brickUID", "3568702513"));
    public static final byte HEADER_LENGTH = 8;
    public static final byte TRUE_BYTE = 1;
    public static final byte FALSE_BYTE = 0;
    public static final BitSet NULL_BITSET = new BitSet(8);

    private long uid;
    private byte packetLength;
    private byte functionId;
    private byte sequenceNumber;    // 4 Bits.
    private boolean response;       // 4 Bits, 1-Bit Response other 3 unused.
    private byte flags;             // first 2 Bit: 0 OK, 1 Invalid Parameter 2 Function noct supported.

    private BitSet bitSet;

    public long getUid() {
        return uid;
    }

    public int getPacketLength() {
        return packetLength;
    }

    public int getFunctionId() {
        return functionId;
    }

    public int getSequenceNumber() {
        return sequenceNumber;
    }

    public boolean isResponse() {
        return response;
    }

    public byte getFlags() {
        return flags;
    }

    public Packet(int packetLength, int functionId, int sequenceNumber, boolean response) {
        this.uid = BRICK_UID;
        this.packetLength = (byte)(HEADER_LENGTH + packetLength);
        this.functionId = (byte) functionId;
        this.sequenceNumber = (byte) sequenceNumber;
        this.response = response;
        this.flags = 0;

        bitSet = new BitSet(8);
        updateBitSet();
    }

    private void updateBitSet() {
        //| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |

        BitSet bitSeq = BitSet.valueOf(new byte[]{sequenceNumber});

        bitSet.clear();
        bitSet.set(7, bitSeq.get(3));
        bitSet.set(6, bitSeq.get(2));
        bitSet.set(5, bitSeq.get(1));
        bitSet.set(4, bitSeq.get(0));

        bitSet.set(3, response);
    }

    private void updateSequenceAndResponse() {
        BitSet bitSeq = new BitSet(8);

        bitSeq.set(3, bitSet.get(7));
        bitSeq.set(2, bitSet.get(6));
        bitSeq.set(1, bitSet.get(5));
        bitSeq.set(0, bitSet.get(4));

        response = bitSeq.get(3);

        if(bitSeq.equals(NULL_BITSET)) {
            sequenceNumber = 0;
        } else {
            sequenceNumber = bitSeq.toByteArray()[0];
        }
    }

    public ByteBuffer toByteBuffer() {
        ByteBuffer uidBuffer = ByteBuffer.allocate(8);
        uidBuffer.order(ByteOrder.LITTLE_ENDIAN);
        uidBuffer.putLong(uid);
        byte[] uidArray = uidBuffer.array();


        ByteBuffer messageBuffer = ByteBuffer.allocate(packetLength);
        messageBuffer.order(ByteOrder.LITTLE_ENDIAN);

        messageBuffer.put(uidArray[0]);
        messageBuffer.put(uidArray[1]);
        messageBuffer.put(uidArray[2]);
        messageBuffer.put(uidArray[3]);

        messageBuffer.put(packetLength);
        messageBuffer.put(functionId);

        //In case that all bits are 0 the BitSet class will not return an byteArray.
        if(bitSet.equals(NULL_BITSET)) {
            messageBuffer.put((byte) 0);
        } else {
            messageBuffer.put(bitSet.toByteArray()[0]);
        }

        messageBuffer.put(flags);

        return messageBuffer;
    }

    public void fromByteArray(byte[] packet) {
        ByteBuffer messageBuffer = ByteBuffer.wrap(packet);
        messageBuffer.order(ByteOrder.LITTLE_ENDIAN);

        packetLength = messageBuffer.get(4);
        functionId = messageBuffer.get(5);
        bitSet = BitSet.valueOf(new byte[]{messageBuffer.get(6)});
        flags = messageBuffer.get(7);

        updateSequenceAndResponse();
    }
}
