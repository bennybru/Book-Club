package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        Database dataBase = new Database(); //one shared object

        if (args[1].equals("tpc")) {
            Server.threadPerClient(
                    Integer.parseInt(args[0]), //port
                    () -> new StompMessagingProtocolImpl(dataBase), //protocol factory
                    StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
        else if (args[1].equals("reactor"))
        {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), //port
                    () ->  new StompMessagingProtocolImpl(dataBase), //protocol factory
                    StompMessageEncoderDecoder::new //message encoder decoder factory
            ).serve();
        }
    }
}
