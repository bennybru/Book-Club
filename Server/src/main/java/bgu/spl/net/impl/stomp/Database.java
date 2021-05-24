package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Database {
    private ConcurrentLinkedQueue<User> users = new ConcurrentLinkedQueue<>();
    private AtomicInteger messageId = new AtomicInteger(0);
    private final ReentrantReadWriteLock usersLock = new ReentrantReadWriteLock();
    private final Lock usersRead = usersLock.readLock();
    private final Lock usersWrite = usersLock.writeLock();

    public User getUserByName(String name) {
        usersRead.lock();
        for (User user : users) {
            if (user.getUserName().equals(name)) {
                usersRead.unlock();
                return user;
            }
        }
        usersRead.unlock();
        return null;
    }

    public User getUserById(int id) {
        usersRead.lock();
        for (User user : users) {
            if (user.getConnectionId() == id) {
                usersRead.unlock();
                return user;
            }
        }
        usersRead.unlock();
        return null;
    }

    public boolean isUserExists(String name) {
        return getUserByName(name) != null;
    }

    public void addUser(User user) {
        usersWrite.lock();
        users.add(user);
        usersWrite.unlock();
    }

    public Integer incMessageId() {
        return messageId.incrementAndGet();
    }

}