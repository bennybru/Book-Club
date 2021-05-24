package bgu.spl.net.impl.stomp;

import java.util.HashMap;;

public class Frame {

    private String stompCommand;
    private HashMap<String,String> headers;
    private String body;

    public Frame() {
        stompCommand = new String();
        headers = new HashMap<>();
        body = new String();
    }

    public Frame(String msg) {
        headers = new HashMap<>();
        String[] splitted = msg.split("\n");
        stompCommand = splitted[0];


        int ctr = 1;
        while (!splitted[ctr].equals("")) {
            String helper = splitted[ctr];
            String[] tmp = helper.split(":");
            headers.put(tmp[0],tmp[1]);
            ctr++;
        }

        body = "";
        while (ctr < splitted.length-2)
        {
            ctr++;

            if (ctr != splitted.length-2)
                body += splitted[ctr] +'\n';
            else
                body += splitted[ctr];
        }
    }
    public String getHeader(String header){
        return headers.get(header);
    }

    @Override
    public String toString() {
        String result = stompCommand + '\n';
        for (String key : headers.keySet())
            result += key + ":" + headers.get(key) + '\n';

        result += '\n';
        if (!body.isEmpty()) {
            result +=  body +'\n';
        }
        result += '\u0000';
        return result;
    }

    public String getStompCommand() {
        return stompCommand;
    }

    public String getBody() {
        return body;
    }

    public Frame connectedFrame(String acceptVer) {
        return new Frame("CONNECTED" + '\n' + "Version:" + acceptVer + '\n' + '\n' + '\u0000');
    }

    public Frame errorFrame(String errorType, Frame malformedMsg, String body) {
        return new Frame("ERROR" + '\n' +
                "message:" + errorType +'\n' +'\n' +
                "The message:" + '\n' +
                "-----" +  '\n' +
                (malformedMsg.toString()).substring(0,malformedMsg.toString().length()-2) + '\n' +
                "-----" + '\n' +
                body + '\n' + '\u0000');
    }

    public Frame receiptFrame(String receipt){
        return new Frame("RECEIPT" + '\n' +
                "receipt-id:" + receipt + '\n' +
                '\n' + '\u0000');
    }

    public Frame messageFrame(Integer sub, Integer messageId, String destination,String body){
        return new Frame("MESSAGE" + '\n' +
                "subscription:" + sub + '\n' +
                "Message-id:" + messageId + '\n' +
                "destination:" + destination + '\n' +
                '\n' + body + '\u0000');
    }

    public Frame addReceiptToFrame(Frame result){
        if (headers.get("receipt-id") != null) {
            result.headers.put("receipt-id",headers.get("receipt-id"));
        }
        return result;
    }


//TODO: remove this lines!
//    public static void main(String[] args) {
//        Frame maformed = new Frame("MSG" + '\n' + "h:1" + '\n' + '\n'  + '\u0000');
//        System.out.println(new Frame().errorFrame("error", maformed, "sdfasdfasdf"));
//    }

}