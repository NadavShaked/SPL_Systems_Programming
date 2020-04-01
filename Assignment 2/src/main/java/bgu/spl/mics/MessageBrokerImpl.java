package bgu.spl.mics;

import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.Result;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.SynchronousQueue;

/**
 * The {@link MessageBrokerImpl class is the implementation of the MessageBroker interface.
 * Write your implementation here!
 * Only private fields and methods can be added to this class.
 */
public class MessageBrokerImpl implements MessageBroker {
    private Map<Subscriber, LinkedBlockingQueue<Message>> subscribersMap;
    private Map<String, LinkedBlockingQueue<Subscriber>> eventsMap;
    private Map<Message, Future> eventFutureMap;

    private static class MessageBrokerHolder {
        private static MessageBroker instance = new MessageBrokerImpl();
    }

    /**
     * Retrieves the single instance of this class.
     */
    public static MessageBroker getInstance() {
        return MessageBrokerHolder.instance;
    }

    private MessageBrokerImpl() {
        subscribersMap = new ConcurrentHashMap<>();
        eventsMap = new ConcurrentHashMap<>();
        eventsMap.put(AgentsAvailableEvent.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(Event.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(GadgetAvailableEvent.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(MissionReceivedEvent.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(ReleaseAgentsEvent.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(SendAgentsEvent.class.getName(), new LinkedBlockingQueue<>());
        eventsMap.put(TickBroadcast.class.getName(), new LinkedBlockingQueue<>());
        eventFutureMap = new ConcurrentHashMap<>();
    }

    @Override
    public <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber m) {
        if (!eventsMap.containsKey(type.getName()))
            eventsMap.put(type.getName(), new LinkedBlockingQueue<>());
        synchronized (eventsMap.get(type.getName())) {
            if (!eventsMap.get(type.getName()).contains(m))
                try {
                    eventsMap.get(type.getName()).put(m);
                } catch (Exception e) {
                }
        }
    }

    @Override
    public void subscribeBroadcast(Class<? extends Broadcast> type, Subscriber m) {
        if (!eventsMap.containsKey(type.getName()))
            eventsMap.put(type.getName(), new LinkedBlockingQueue<>());
        synchronized (eventsMap.get(type.getName())) {
            if (!eventsMap.get(type.getName()).contains(m))
                try {
                    eventsMap.get(type.getName()).put(m);
                } catch (Exception e) {
                }
        }
    }

    @Override
    public <T> void complete(Event<T> e, T result) {
        Future future = eventFutureMap.get(e);
        future.resolve(result);
    }

    @Override
    public void sendBroadcast(Broadcast b) {
        LinkedBlockingQueue<Subscriber> list = eventsMap.get(b.getClass().getName());
        for (Subscriber subscriber : list) {
            try {
                subscribersMap.get(subscriber).put(b);
            } catch (Exception e) {
            }
        }
    }

    @Override
    public <T> Future<T> sendEvent(Event<T> e) {
        if (!eventsMap.containsKey(e.getClass().getName())) {
            return null;
        }
        synchronized (eventsMap.get(e.getClass().getName())) {
            LinkedBlockingQueue<Subscriber> list = eventsMap.get(e.getClass().getName());
            if (eventsMap.get(e.getClass().getName()).isEmpty())
                return null;
            Future<T> future = new Future<>();
            eventFutureMap.put(e, future);
            try {
                Subscriber subscriber = list.take();
                list.add(subscriber);
                if (subscribersMap.containsKey(subscriber))
                    subscribersMap.get(subscriber).add(e);
            } catch (Exception ex) {
            }


            return future;
        }
    }

    @Override
    public void register(Subscriber m) {
        subscribersMap.put(m, new LinkedBlockingQueue<>());
    }

    @Override
    public void unregister(Subscriber m) {
        if (subscribersMap.containsKey(m)) {
            for (Message message : subscribersMap.get(m)) {
                synchronized (eventFutureMap.get(message)) {
                    if (eventFutureMap.get(message) != null) {
                        eventFutureMap.get(message).resolve(new Result());
                    }
                }
            }
            synchronized (m) {
                subscribersMap.remove(m);
            }
            for (Map.Entry<String, LinkedBlockingQueue<Subscriber>> eventType : eventsMap.entrySet()) {
                synchronized (eventType) {
                    if (eventType.getValue().contains(m)) {
                        eventType.getValue().remove(m);
                    }
                }
            }
        }
    }

    @Override
    public Message awaitMessage(Subscriber m) throws InterruptedException {
        if (subscribersMap.get(m) != null) {
            Message message = subscribersMap.get(m).take();
            return message;
        }
        return null;
    }
}
