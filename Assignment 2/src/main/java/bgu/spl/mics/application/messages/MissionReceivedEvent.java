package bgu.spl.mics.application.messages;

import bgu.spl.mics.application.passiveObjects.MissionInfo;
import bgu.spl.mics.application.passiveObjects.Result;

public class MissionReceivedEvent extends Event<Result> {
    private MissionInfo missionInfo;

    //Constructors
    public MissionReceivedEvent(MissionInfo missionInfo) {
        this.missionInfo = missionInfo;
    }

    //Setters
    public void setMissionInfo(MissionInfo missionInfo) {
        this.missionInfo = missionInfo;
    }

    //Getters
    public MissionInfo getMissionInfo() {
        return missionInfo;
    }
}
