package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.ConnectionsImpl;
import bgu.spl.net.impl.stomp.StompMessageEncoderDecoder;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMessagingProtocolImpl> protocolFactory;
    private final Supplier<StompMessageEncoderDecoder> encdecFactory;
    private ServerSocket sock;
    private final ConnectionsImpl connectionsImplInstance;

    public BaseServer(
            int port,
            Supplier<StompMessagingProtocolImpl> protocolFactory,
            Supplier<StompMessageEncoderDecoder> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.sock = null;
        connectionsImplInstance = ConnectionsImpl.getInstance();
    }

    @Override
    public void serve() {
        try (ServerSocket serverSock = new ServerSocket(port)) {
            System.out.println("Server started");
            this.sock = serverSock; //just to be able to close
            while (!Thread.currentThread().isInterrupted()) {
                Socket clientSock = serverSock.accept();
                StompMessagingProtocolImpl stompMessagingProtocol = protocolFactory.get();
                int connectionId = connectionsImplInstance.getNewConnectionId();
                stompMessagingProtocol.start(connectionId, connectionsImplInstance);
                BlockingConnectionHandler handler = new BlockingConnectionHandler(clientSock, encdecFactory.get(), stompMessagingProtocol);
                connectionsImplInstance.addConnectionHandler(connectionId, handler);
                execute(handler);
            }
        } catch (IOException ex) {
            System.out.println(ex);
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
        if (sock != null)
            sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler handler);

}
