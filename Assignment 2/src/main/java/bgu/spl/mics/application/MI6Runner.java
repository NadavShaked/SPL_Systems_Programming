package bgu.spl.mics.application;

import bgu.spl.mics.application.passiveObjects.Diary;
import bgu.spl.mics.application.passiveObjects.Inventory;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import bgu.spl.mics.application.passiveObjects.Squad;
import bgu.spl.mics.application.publishers.TimeService;
import bgu.spl.mics.application.subscribers.Intelligence;
import bgu.spl.mics.application.subscribers.M;
import bgu.spl.mics.application.subscribers.Moneypenny;
import bgu.spl.mics.application.subscribers.Q;
import com.google.gson.Gson;
import com.google.gson.stream.JsonReader;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * This is the Main class of the application. You should parse the input file,
 * create the different instances of the objects, and run the system.
 * In the end, you should output serialized objects.
 */
public class MI6Runner {
    public static void main(String[] args) {
        Gson gson = new Gson();
        Inventory inventory = Inventory.getInstance();
        Squad squad = Squad.getInstance();

        try {
            JsonReader reader = new JsonReader(new FileReader(args[0]));
            JsonParser parser = gson.fromJson(reader, JsonParser.class);
            int appDuration = parser.services.time;
            inventory.load(parser.inventory);
            squad.load(parser.squad);
            int numOfMObjects = parser.services.M;
            int numOfMoneyPennyObjects = parser.services.Moneypenny;
            JsonParser.MI6Class.IntelligencesArray[] intelligencesArray = parser.services.intelligence;
            int idCounter = 1;
            int numOfThreads = intelligencesArray.length + numOfMObjects + numOfMoneyPennyObjects + 3; //3 for Q, TimeService and Main
            ExecutorService threadPool = Executors.newFixedThreadPool(numOfThreads);
            for (JsonParser.MI6Class.IntelligencesArray missionsArray : intelligencesArray) {
                List<MissionInfo> missionInfoList = convertFromJsonParserToMissionInfoList(missionsArray);
                threadPool.execute(new Intelligence(idCounter, missionInfoList));
                idCounter++;
            }
            for (int i = 1; i <= numOfMObjects; i++) {
                threadPool.execute(new M(i));
            }
            for (int i = 1; i <= numOfMoneyPennyObjects; i++) {
                threadPool.execute(new Moneypenny(i));
            }
            threadPool.execute(new Q(1));
            threadPool.execute(new TimeService(appDuration));
            threadPool.shutdown();
            while (!threadPool.isTerminated()) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Diary.getInstance().printToFile(args[1]);
            inventory.printToFile(args[2]);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static List<MissionInfo> convertFromJsonParserToMissionInfoList(JsonParser.MI6Class.IntelligencesArray missionsArray) {
        List<MissionInfo> missionInfoList = new LinkedList<>();
        for (JsonParser.MI6Class.Missions mission : missionsArray.missions) {
            MissionInfo missionInfo = new MissionInfo();
            missionInfo.setMissionName(mission.name);
            List<String> serialAgentsNumbers = new LinkedList<>();
            for (String serialAgentsNumber : mission.serialAgentsNumbers) {
                serialAgentsNumbers.add(serialAgentsNumber);
            }
            missionInfo.setSerialAgentsNumbers(serialAgentsNumbers);
            missionInfo.setGadget(mission.gadget);
            missionInfo.setTimeIssued(mission.timeIssued);
            missionInfo.setTimeExpired(mission.timeExpired);
            missionInfo.setDuration(mission.duration);

            missionInfoList.add(missionInfo);
        }
        return missionInfoList;
    }
}
