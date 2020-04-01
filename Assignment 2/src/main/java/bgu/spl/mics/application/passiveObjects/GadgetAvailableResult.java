package bgu.spl.mics.application.passiveObjects;

public class GadgetAvailableResult extends Result {
    private int qTime;

    public GadgetAvailableResult(int qTime, boolean isSuccessful) {
        super(isSuccessful);
        this.qTime = qTime;
    }

    public int getQTime() {
        return qTime;
    }
}
