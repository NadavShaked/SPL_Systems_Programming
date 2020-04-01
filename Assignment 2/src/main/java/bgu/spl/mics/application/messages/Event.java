package bgu.spl.mics.application.messages;

public class Event<T> implements bgu.spl.mics.Event {
    private T result;

    public synchronized void setResult(T result) {
        this.result = result;
    }

    public synchronized T getResult() {
        return this.result;
    }
}
