package bgu.spl.net.srv;

public interface Connections<Frame> {

    boolean send(int connectionId, Frame msg);

    void send(String channel, Frame msg);

    void disconnect(int connectionId);
}
