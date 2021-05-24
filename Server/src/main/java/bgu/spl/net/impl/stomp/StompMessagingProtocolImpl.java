package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<Frame> {

    private boolean shouldTerminate = false;
    private ConnectionsImpl<Frame> connections;
    private int connectionId;
    private Database dataBase;

    public StompMessagingProtocolImpl(Database dataBase) {
        this.dataBase = dataBase;
    }

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl<Frame>) connections;
    }

    @Override
    public void process(Frame frame) {
        String stompCommand = frame.getStompCommand();
        String body = frame.getBody();

        //Connect
        if ("CONNECT".equals(stompCommand)) {
            String name = frame.getHeader("login");
            String pass = frame.getHeader("passcode");
            if (!dataBase.isUserExists(name)) {
                dataBase.addUser(new User(name, pass));
            }
            Frame answer = null;
            User user = dataBase.getUserByName(name);
            if (user.isLoggedIn()) {
                answer = new Frame().errorFrame("Login failed", frame, "User already logged in");
                connections.send(connectionId, answer);
                    //TODO: implement shouldTerminate mechanism
                    //  shouldTerminate = true;
            } else if (!user.getPasscode().equals(pass)) {
                answer = new Frame().errorFrame("Login Failed", frame, "Wrong password");
                shouldTerminate = true;
                connections.send(connectionId, answer);
            } else {
                user.login(connectionId);
                answer = new Frame().connectedFrame(frame.getHeader("accept-version"));
                connections.send(connectionId, answer);
            }

            //Send
        } else if ("SEND".equals(stompCommand)) {
            User user = dataBase.getUserById(connectionId);
            if (user == null || !user.isLoggedIn()) {
                connections.send(connectionId, frame.addReceiptToFrame(new Frame().errorFrame("User not connected", frame, "Need to login before send command")));
            } else {
                String destination = frame.getHeader("destination");
                connections.send(destination, new Frame().messageFrame(user.getSubscribe().get(destination), dataBase.incMessageId(), destination, body));

                //TODO: choose if to use receipt
                sendReciept(frame);
            }
            //Subscribe
        } else if ("SUBSCRIBE".equals(stompCommand)) {
            User user = dataBase.getUserById(connectionId);
            if (user == null || !user.isLoggedIn()) {
                connections.send(connectionId, frame.addReceiptToFrame(new Frame().errorFrame("User not connected", frame, "Need to login before subscribe command")));
            } else {
                String destination = frame.getHeader("destination");
                Integer subId = Integer.parseInt(frame.getHeader("id"));
                user.setSubscribe(destination, subId);
                connections.addUserToGenre(destination, connectionId);

                sendReciept(frame);
            }
            //Unsubscribe
        } else if ("UNSUBSCRIBE".equals(stompCommand)) {
            User user = dataBase.getUserById(connectionId);
            if (user == null || !user.isLoggedIn()) {
                connections.send(connectionId, frame.addReceiptToFrame(new Frame().errorFrame("User not connected", frame, "Need to login before unsubscribe command")));
            } else {
                Integer subId = Integer.parseInt(frame.getHeader("id"));
                String genre = user.removeSubscribe(subId);
                connections.removeUserFromGenre(genre, connectionId);

                sendReciept(frame);
            }
            //Disconnect
        } else if ("DISCONNECT".equals(stompCommand)) {
            User user = dataBase.getUserById(connectionId);
            if (user == null || !user.isLoggedIn()) {
                connections.send(connectionId, frame.addReceiptToFrame(new Frame().errorFrame("User not connected", frame, "Need to login before disconnect command")));
            } else {
                sendReciept(frame);
                disconnect();
            }
        }
    }

    public void disconnect() {
        User user = dataBase.getUserById(connectionId);
        if (user != null) {
            for (Map.Entry<String, Integer> entry : user.getSubscribe().entrySet()) {
                connections.removeUserFromGenre(entry.getKey(), connectionId);
                user.getSubscribe().remove(entry.getKey());
            }
            connections.disconnect(connectionId);
            user.logoff();
            //TODO: implement shouldTerminate mechanism
            //  shouldTerminate = true;
        }
    }

    private void sendReciept(Frame frame) {
        String receiptId = frame.getHeader("receipt-id");
        if (receiptId != null) {
            connections.send(connectionId, new Frame().receiptFrame(receiptId));
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
