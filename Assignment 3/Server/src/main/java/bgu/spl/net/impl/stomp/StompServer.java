package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args[1].equals("reactor")){
            Server.reactor(10, Short.valueOf(args[0]), () -> new StompMessagingProtocolImpl<>(), () -> new StompMessageEncoderDecoder()).serve(); //TODO: check num of threads
        }
        else if (args[1].equals("tpc")){
            Server.threadPerClient(Short.valueOf(args[0]), ()-> new StompMessagingProtocolImpl<>(), () -> new StompMessageEncoderDecoder()).serve();
        }
    }
}
