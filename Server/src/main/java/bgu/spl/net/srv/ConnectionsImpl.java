package bgu.spl.net.srv;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ConnectionsImpl<T> implements Connections<T> {

    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connections;
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> genres;
    private final AtomicInteger id = new AtomicInteger();
    private final ReentrantReadWriteLock connectionsLock = new ReentrantReadWriteLock();
    private final Lock connectionsRead = connectionsLock.readLock();
    private final Lock connectionsWrite = connectionsLock.writeLock();
    private final ReentrantReadWriteLock genresLock = new ReentrantReadWriteLock();
    private final Lock genresRead = genresLock.readLock();
    private final Lock genresWrite = genresLock.writeLock();

    public ConnectionsImpl() {
        connections = new ConcurrentHashMap<>();
        genres = new ConcurrentHashMap<>();
        id.set(-1);
    }

    @Override
    public boolean send(int connectionId, T msg) {
        connectionsRead.lock();
        ConnectionHandler<T> handler = connections.get(connectionId);
        connectionsRead.unlock();

        if (handler == null) {
            return false;
        }

        handler.send(msg);
        return true;//todo:check when should it return {@code false}
    }

    @Override
    public void send(String channel, T msg) {
        genresRead.lock();
        for (Integer id : genres.get(channel)) {
            connectionsRead.lock();
            ConnectionHandler<T> handler = connections.get(id);
            connectionsRead.unlock();
            if (handler != null) {
                handler.send(msg);
            }
        }
        genresRead.unlock();
    }

    @Override
    public void disconnect(int connectionId) {
        connectionsRead.lock();
        ConnectionHandler<T> handler = connections.get(connectionId);
        connectionsRead.unlock();

        connectionsWrite.lock();
        connections.remove(connectionId);
        connectionsWrite.unlock();

//      TODO: check if needed
//        try {
//            handler.close();
//        } catch (IOException e) {
//        }
    }

    @Override
    public int add(ConnectionHandler<T> handler) {
        int id = this.id.incrementAndGet();
        connectionsWrite.lock();
        connections.putIfAbsent(id, handler);
        connectionsWrite.unlock();
        return id;
    }

    public void removeUserFromGenre(String genre, Integer connectionId) {
        genresWrite.lock();
        genres.get(genre).remove(connectionId);
        if (genres.get(genre).isEmpty()) {
            genres.remove(genre);
        }
        genresWrite.unlock();
    }

    public void addUserToGenre(String genre, Integer connectionId) {
        genresWrite.lock();
        ConcurrentLinkedQueue<Integer> subs = genres.get(genre);

        if (subs != null) {
            if (!subs.contains(connectionId)) {
                subs.add(connectionId);
            }
        } else {
            subs = new ConcurrentLinkedQueue<>();
            subs.add(connectionId);
            genres.put(genre, new ConcurrentLinkedQueue<>());
        }
        genresWrite.unlock();
    }
}
