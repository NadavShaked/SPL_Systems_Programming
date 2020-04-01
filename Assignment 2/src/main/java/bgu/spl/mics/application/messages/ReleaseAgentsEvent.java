package bgu.spl.mics.application.messages;

import bgu.spl.mics.application.passiveObjects.Result;

import java.util.List;

public class ReleaseAgentsEvent extends Event<Result> {
    private List<String> serialAgentsNumbers;

    public ReleaseAgentsEvent(List<String> serialAgentsNumbers) {
        this.serialAgentsNumbers = serialAgentsNumbers;
    }

    public void setSerialAgentsNumbers(List<String> serialAgentsNumbers) {
        this.serialAgentsNumbers = serialAgentsNumbers;
    }

    public List<String> getSerialAgentsNumbers() {
        return serialAgentsNumbers;
    }
}
