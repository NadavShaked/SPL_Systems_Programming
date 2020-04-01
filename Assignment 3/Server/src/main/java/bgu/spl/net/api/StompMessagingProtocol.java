package bgu.spl.net.api;

import bgu.spl.net.impl.stomp.ConnectionsImpl;
import bgu.spl.net.impl.stomp.Frame;

public interface StompMessagingProtocol {
    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     **/
    void start(int connectionId, ConnectionsImpl connections);

    void process(Frame message);

    /**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
