package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;

public class User {
    private String userName = "";
    private String passcode = "";
    private int connectionId = -1;
    private HashMap<String, Integer> subscribe = new HashMap<>();
    private boolean loggedIn = false;

    public User(String userName, String passcode) {
        this.userName = userName;
        this.passcode = passcode;
    }

    public String getUserName() {
        return userName;
    }

    public String getPasscode() {
        return passcode;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public void login(int connectionId) {
        this.connectionId = connectionId;
        loggedIn = true;
    }

    public void logoff() {
        loggedIn = false;
    }

    public HashMap<String, Integer> getSubscribe() {
        return subscribe;
    }

    public void setSubscribe(String genre, Integer sub) {
        subscribe.putIfAbsent(genre, sub);
    }

    public String removeSubscribe(Integer subId) {
        for (Map.Entry<String, Integer> entry : subscribe.entrySet()) {
            if (entry.getValue().equals(subId)) {
                String result = entry.getKey();
                subscribe.remove(entry.getKey());
                return result;
            }
        }
        return null;
    }

    public boolean isLoggedIn() {
        return loggedIn;
    }
}