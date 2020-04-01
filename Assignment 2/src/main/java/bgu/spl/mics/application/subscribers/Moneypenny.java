package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.ReleaseAgentsEvent;
import bgu.spl.mics.application.messages.SendAgentsEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Result;
import bgu.spl.mics.application.passiveObjects.AgentAvailableResult;
import bgu.spl.mics.application.passiveObjects.Squad;

import java.util.List;

/**
 * Only this type of Subscriber can access the squad.
 * There are several Moneypenny-instances - each of them holds a unique serial number that will later be printed on the report.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Moneypenny extends Subscriber {
    int currentTime;
    int moneypennyId;
    Squad squad;

    public Moneypenny() {
        super("Moneypenny");
        currentTime = 0;
        squad = Squad.getInstance();
    }

    public Moneypenny(int id) {
        super("Moneypenny" + id);
        currentTime = 0;
        moneypennyId = id;
        squad = Squad.getInstance();
    }

    @Override
    protected void initialize() {
        if (moneypennyId % 2 == 0) {
            subscribeEvent(AgentsAvailableEvent.class, (AgentsAvailableEvent event) -> {
                List<String> agentsSerials = event.getAgents();
                AgentAvailableResult result = new AgentAvailableResult(this.moneypennyId, agentsSerials, squad.getAgentsNames(agentsSerials), squad.getAgents(agentsSerials));
                complete(event, result);
            });
        } else {
            subscribeEvent(SendAgentsEvent.class, (SendAgentsEvent event) -> {
                List<String> agentsSerials = event.getAgents();
                squad.sendAgents(agentsSerials, event.getDuration());
                Result result = new Result(true);
                complete(event, result);
            });
            subscribeEvent(ReleaseAgentsEvent.class, (ReleaseAgentsEvent event) -> {
                List<String> agentsSerials = event.getSerialAgentsNumbers();
                squad.releaseAgents(agentsSerials);
                Result result = new Result(true);
                complete(event, result);
            });
        }
        subscribeBroadcast(TickBroadcast.class, (TickBroadcast broadcast) -> {
            currentTime = broadcast.getCurrentTime();
            if (broadcast.isTerminated()) {
                squad.setShouldTerminate(true);
                squad.releaseAll();
                terminate();
            }
        });
    }

    public int getMoneypennySerial() {
        return moneypennyId;
    }
}
