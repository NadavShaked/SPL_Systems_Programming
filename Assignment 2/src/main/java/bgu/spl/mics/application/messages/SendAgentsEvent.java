package bgu.spl.mics.application.messages;

import bgu.spl.mics.application.passiveObjects.Result;

import java.util.List;

public class SendAgentsEvent extends Event<Result> {
    private List<String> agents;
    private int duration;

    public SendAgentsEvent(List<String> agents, int duration) {
        this.agents = agents;
        this.duration = duration;
    }

    public void setAgents(List<String> agents) {
        this.agents = agents;
    }

    public List<String> getAgents() {
        return agents;
    }

    public int getDuration() {
        return duration;
    }

    public void setDuration(int duration) {
        this.duration = duration;
    }
}
