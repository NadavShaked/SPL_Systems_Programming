package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol {
    private ConnectionsImpl connectionsInstance;
    private int connectionId;
    private boolean shouldTerminate;

    @Override
    public void start(int connectionId, ConnectionsImpl connections) {
        this.connectionsInstance = connections;
        this.connectionId = connectionId;
        this.shouldTerminate = false;
    }

    @Override
    public void process(Frame message) {
        System.out.println("---NewFrame---");//todo make as command
        System.out.println(message.getStompCommand());
        for (Header header : message.getHeaders()) {
            System.out.println(header.getName() + ":" + header.getValue());
        }
        System.out.println(message.getFrameBody());
        System.out.println("---EndFrame---");


        Map<String, String> headersMap = mapValues(message.getHeaders());
        List<Header> headers;
        String destination;

        if (headersMap.containsKey("receipt")) {
            headers = new LinkedList<>();
            headers.add(new Header("receipt-id", headersMap.get("receipt")));
            Frame receipt = new Frame("RECEIPT", headers, "");
            connectionsInstance.send(connectionId, receipt);
        }

        if (message.getStompCommand().equals("CONNECT")) {
            String ret = ((ConnectionsImpl) connectionsInstance).connectToUser(headersMap.get("login"), headersMap.get("passcode"), connectionId);
            if (ret.equals("Connection Successful")) {
                headers = new LinkedList<>();
                headers.add(new Header("version", headersMap.get("accept-version")));
                Frame frame = new Frame("CONNECTED", headers, "");
                connectionsInstance.send(connectionId, frame);
            } else {
                headers = new LinkedList<>();
                Frame frame = new Frame("ERROR", headers, ret);
                connectionsInstance.send(connectionId, frame);
                //TODO: need to terminate the client thread, and maybe to delete the connectionHandler from connectionHandlerMap
            }
        } else if (message.getStompCommand().equals("SUBSCRIBE")) {
            if (headersMap.containsKey("destination") & headersMap.containsKey("id")) {
                destination = headersMap.get("destination");
                String idString = headersMap.get("id");
                int id = Integer.valueOf(idString); //TODO maybe check exception
                connectionsInstance.subscribeUser(destination, connectionId, id);
            }
        } else if (message.getStompCommand().equals("SEND")) {
            if (headersMap.containsKey("destination")) {
                destination = headersMap.get("destination");
                headers = new LinkedList<>();
                headers.add(new Header("message-id", String.valueOf(connectionsInstance.getNewMessageId())));
                headers.add(new Header("destination", destination));

                Frame frame = new Frame("MESSAGE", headers, message.getFrameBody());
                connectionsInstance.send(destination, frame);
            } else {
                //TODO: maybe send error
            }
        } else if (message.getStompCommand().equals("UNSUBSCRIBE")) {
            (connectionsInstance).unsubscribe(connectionId, headersMap.get("id"));
        } else if (message.getStompCommand().equals("DISCONNECT")) { //todo: remove data from stracture
            connectionsInstance.disconnect(connectionId);
            shouldTerminate = true;
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    private Map<String, String> mapValues(List<Header> headerList) {
        Map<String, String> output = new HashMap<>();
        for (Header header : headerList) {
            output.put(header.getName(), header.getValue());
        }
        return output;
    }
}
