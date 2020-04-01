package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.NonBlockingConnectionHandler;

import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl implements Connections<Frame> {
    private AtomicInteger connectionIdCounter;
    private AtomicInteger msgIdCounter;
    private Map<Integer, ConnectionHandler> connectionHandlerMap;
    private Map<Integer, User> userMap;
    private Map<String, LinkedBlockingQueue<Pair>> topicMap;

    private static class ConnectionsHolder {
        private static ConnectionsImpl instance = new ConnectionsImpl();
    }

    private ConnectionsImpl() {
        connectionIdCounter = new AtomicInteger(1);
        msgIdCounter = new AtomicInteger(1);
        userMap = new ConcurrentHashMap<>();
        topicMap = new ConcurrentHashMap<>();
        connectionHandlerMap = new ConcurrentHashMap<>();
    }

    public static ConnectionsImpl getInstance() {
        return ConnectionsHolder.instance;
    }

    @Override
    public boolean send(int connectionId, Frame msg) {
        if (!connectionHandlerMap.containsKey(connectionId))
            return false;
        connectionHandlerMap.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, Frame msg) {
        if(topicMap.containsKey(channel)){
            LinkedBlockingQueue<Pair> usersList = topicMap.get(channel);
            for (Pair pair : usersList) {
                ConnectionHandler connectionHandler = connectionHandlerMap.get(pair.getUser().getConnectionId());
                msg.addHeader("subscription", String.valueOf(pair.getSubscriptionId()));//todo: return
                connectionHandler.send(msg);
                msg.removeHeader("subscription");
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        User user = userMap.get(connectionId);
        userMap.remove(connectionId);
        userMap.put(-connectionId, user);

        LinkedList<String> userTopics = user.getTopicsList();
        for (String topic : userTopics) {
            //remove user from topicMap
            LinkedBlockingQueue<Pair> usersSubscribedToTopicQueue = topicMap.get(topic);
            for (Pair userInfo : usersSubscribedToTopicQueue) {
                if (userInfo.getUser().getUserName().equals(user.getUserName())) {
                    usersSubscribedToTopicQueue.remove(userInfo);
                }
            }
        }

    }

    public <T> void addConnectionHandler(int connectionId, BlockingConnectionHandler handler) {
        connectionHandlerMap.put(connectionId, handler);
    }

    public <T> void addConnectionHandler(int connectionId, NonBlockingConnectionHandler handler) {
        connectionHandlerMap.put(connectionId, handler);
    }

    public void subscribeUser(String destination, int connectionId, int id) {
        String idStr = String.valueOf(id);
        if (topicMap.containsKey(destination)) {
            topicMap.get(destination).add(new Pair(userMap.get(connectionId), id));
        } else {
            topicMap.put(destination, new LinkedBlockingQueue<Pair>());
            topicMap.get(destination).add(new Pair(userMap.get(connectionId), id));
        }
        userMap.get(connectionId).subscribeTopic(destination, idStr);
    }

    public void unsubscribe(int connectionId, String id) {
        User user = userMap.get(connectionId);
        String topic = user.getTopicById(id);
        LinkedBlockingQueue<Pair> usersSubscribeToTopicQueue = topicMap.get(topic);
        for (Pair userInfo : usersSubscribeToTopicQueue) {
            if (userInfo.getUser().getUserName().equals(user.getUserName())) {
                usersSubscribeToTopicQueue.remove(userInfo);
                break;
            }
        }
        user.unsubscribeTopic(id);
    }

    public String connectToUser(String userName, String userPasscode, int connectionId) {
        for (Map.Entry<Integer, User> entry : userMap.entrySet()) { //Check if user exist and if already connected
            User tmpUser = entry.getValue();
            if (tmpUser.getUserName().equals(userName)) {
                Integer conId = entry.getKey();
                if (conId < 0) {
                    if (tmpUser.getUserPasscode().equals(userPasscode)) {
                        userMap.remove(conId);
                        tmpUser.setConnectionId(connectionId);
                        userMap.put(connectionId, tmpUser);
                        return "Connection Successful";
                    } else
                        return "Wrong password";
                } else
                    return "User already connected";
            }
        }
        User newUser = new User(userName, userPasscode, connectionId);
        userMap.put(connectionId, newUser);
        return "Connection Successful";
    }

    public int getNewConnectionId() {
        return connectionIdCounter.getAndIncrement();
    }

    public int getNewMessageId() {
        return msgIdCounter.getAndIncrement();
    }
}
