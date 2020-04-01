package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Future;
import bgu.spl.mics.MessageBroker;
import bgu.spl.mics.MessageBrokerImpl;
import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.*;
import bgu.spl.mics.application.passiveObjects.*;

import java.util.List;
import java.util.concurrent.TimeUnit;

/**
 * M handles ReadyEvent - fills a report and sends agents to mission.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class M extends Subscriber {
    private int currentTime;
    private int mId;

    public M() {
        super("M");
        currentTime = 0;
    }

    public M(int mId) {
        super("M" + mId);
        currentTime = 0;
        this.mId = mId;
    }

    @Override
    protected void initialize() {
        subscribeEvent(MissionReceivedEvent.class, (MissionReceivedEvent event) -> {
            MissionInfo missionInfo = event.getMissionInfo();
            List<String> agents = missionInfo.getSerialAgentsNumbers();
            Diary diary = Diary.getInstance();
            Result result = new Result(false);
            AgentsAvailableEvent agentsAvailableEvent = new AgentsAvailableEvent(agents);
            Future agentsAvailableEventFuture = getSimplePublisher().sendEvent(agentsAvailableEvent);

            if (agentsAvailableEventFuture == null) {
                diary.incrementTotal();
                return;
            }

            Result agentsAvailableEventFutureResult = (Result) agentsAvailableEventFuture.get(100*(missionInfo.getTimeExpired()-currentTime), TimeUnit.MILLISECONDS);
            if (agentsAvailableEventFutureResult == null) {
                diary.incrementTotal();
                return;
            }


            if(!(agentsAvailableEventFutureResult instanceof AgentAvailableResult)){
                diary.incrementTotal();
                return;
            }

            if (agentsAvailableEventFutureResult.isSuccessful()) {
                GadgetAvailableEvent gadgetAvailableEvent = new GadgetAvailableEvent(missionInfo.getGadget());
                Future gadgetAvailableEventFuture = getSimplePublisher().sendEvent(gadgetAvailableEvent);
                if (gadgetAvailableEventFuture == null) {
                    diary.incrementTotal();
                    return;
                }
                Result gadgetAvailableEventFutureResult = (Result) gadgetAvailableEventFuture.get(100*(missionInfo.getTimeExpired()-currentTime), TimeUnit.MILLISECONDS);
                if (gadgetAvailableEventFutureResult == null) {
                    diary.incrementTotal();
                    return;
                }

                if(!(gadgetAvailableEventFutureResult instanceof GadgetAvailableResult)){
                    diary.incrementTotal();
                    return;
                }
                if (gadgetAvailableEventFutureResult.isSuccessful() && ((GadgetAvailableResult) gadgetAvailableEventFutureResult).getQTime() <= missionInfo.getTimeExpired()) {
                    SendAgentsEvent sendAgentsEvent = new SendAgentsEvent(agents, missionInfo.getDuration());
                    Future sendAgentsEventFuture = getSimplePublisher().sendEvent(sendAgentsEvent);
                    if (sendAgentsEventFuture == null) {
                        diary.incrementTotal();
                        return;
                    }
                    //sendAgentsEventFuture.get();
                    Report report = createReport(missionInfo, (AgentAvailableResult) agentsAvailableEventFutureResult, (GadgetAvailableResult) gadgetAvailableEventFutureResult);
                    diary.addReport(report);
                    result.setIsSuccessful(true);
                } else {
                    diary.incrementTotal();
                    ReleaseAgentsEvent releaseAgentsEvent = new ReleaseAgentsEvent(missionInfo.getSerialAgentsNumbers());
                    getSimplePublisher().sendEvent(releaseAgentsEvent);
                }
            } else {
                diary.incrementTotal();
            }
            complete(event, result);
        });

        subscribeBroadcast(TickBroadcast.class, (TickBroadcast broadcast) -> {
            currentTime = broadcast.getCurrentTime();
            if (broadcast.isTerminated()) {
                terminate();
            }
        });
    }

    private Report createReport(MissionInfo missionInfo, AgentAvailableResult agentsAvailableEventFutureResult, GadgetAvailableResult gadgetAvailableEventFutureResult) {
        Report report = new Report();
        report.setName(missionInfo.getMissionName());
        report.setM(mId);
        report.setMoneypenny(agentsAvailableEventFutureResult.getMoneypenny());
        report.setAgentsSerialNumbers(missionInfo.getSerialAgentsNumbers());
        report.setAgentsNames(agentsAvailableEventFutureResult.getAgentsNames());
        report.setGadgetName(missionInfo.getGadget());
        report.setTimeIssued(missionInfo.getTimeIssued());
        report.setQTime(gadgetAvailableEventFutureResult.getQTime());
        report.setTimeCreated(currentTime);
        return report;
    }
}
