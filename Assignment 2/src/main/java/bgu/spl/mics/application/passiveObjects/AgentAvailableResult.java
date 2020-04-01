package bgu.spl.mics.application.passiveObjects;

import java.util.List;

public class AgentAvailableResult extends Result {
    private int moneypenny;
    private List<String> agentsSerials;
    private List<String> agentsNames;

    public AgentAvailableResult(int moneypenny, List<String> agentsSerials, List<String> agentsNames, boolean isSuccessful) {
        super(isSuccessful);
        this.moneypenny = moneypenny;
        this.agentsSerials = agentsSerials;
        this.agentsNames = agentsNames;
    }

    public void setMoneypenny(int moneypenny) {
        this.moneypenny = moneypenny;
    }

    public int getMoneypenny() {
        return moneypenny;
    }

    public List<String> getAgentsNames() {
        return agentsNames;
    }

    public List<String> getAgentsSerials() {
        return agentsSerials;
    }

    public void setAgentsNames(List<String> agentsNames) {
        this.agentsNames = agentsNames;
    }

    public void setAgentsSerials(List<String> serials) {
        this.agentsSerials = serials;
    }
}
