package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class StompMessageEncoderDecoder implements MessageEncoderDecoder<Frame> {

    private byte[] bytes = new byte[1<<10];
    private int len = 0;

    @Override
    public Frame decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            return new Frame(popString());
        }

        pushByte(nextByte);
        return null;
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private String popString() {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        result += '\u0000';
        return result;
    }

    @Override
    public byte[] encode(Frame message) {
        return (message.toString()).getBytes();
    }
}