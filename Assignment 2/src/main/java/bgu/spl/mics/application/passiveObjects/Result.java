package bgu.spl.mics.application.passiveObjects;

public class Result {
    private boolean isSuccessful;

    public Result() {
    }

    public Result(boolean isSuccessful) {
        this.isSuccessful = isSuccessful;
    }

    public Result(Boolean isSuccessful) {
        this.isSuccessful = isSuccessful;
    }

    public boolean isSuccessful() {
        return isSuccessful;
    }

    public void setIsSuccessful(boolean resolved) {
        isSuccessful = resolved;
    }
}
