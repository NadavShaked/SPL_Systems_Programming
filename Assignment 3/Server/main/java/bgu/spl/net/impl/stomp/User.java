package bgu.spl.net.impl.stomp;

import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class User {
    private String userName;
    private String userPasscode;
    private int connectionId;
    private Map<String, String> subscriptionsMap;

    public User(String userName, String userPasscode, int connectionId) {
        this.userName = userName;
        this.userPasscode = userPasscode;
        this.connectionId = connectionId;
        subscriptionsMap = new ConcurrentHashMap<>();
    }

    public String getUserName() {
        return userName;
    }

    public String getUserPasscode() {
        return userPasscode;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public String getTopicById(String id) {
        return subscriptionsMap.get(id);
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }

    public void subscribeTopic(String topic, String id){
        subscriptionsMap.put(id,topic);
    }

    public void unsubscribeTopic(String id){
        subscriptionsMap.remove(id);
    }

    public LinkedList<String> getTopicsList() {
        LinkedList<String> toReturn = new LinkedList<>();
        for(String pair: subscriptionsMap.values()){
            toReturn.add(pair);
        }
        return toReturn;
    }
}
