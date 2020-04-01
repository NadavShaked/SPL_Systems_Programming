//
// Created by danyard@wincs.cs.bgu.ac.il on 21/11/2019.
//
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <limits>

// methods
void Session::start() {
  //  Session("../config2.json");
    std::string command;
    std::cout << "insert a command";
    std::getline(std::cin, command);
    while (command != "exit") {

        std::string commandType = command.substr(0, command.find_first_of(' '));
        if (commandType == "createuser") {
            std::string afterFirstWord = command.substr(command.find(' ') + 1);
            std::string userName = afterFirstWord.substr(0, afterFirstWord.find_first_of(' '));
            std::string reccomendAlgo = afterFirstWord.substr(afterFirstWord.find(' ') + 1);

            CreateUser* action = new CreateUser(userName, reccomendAlgo);
            actionsLog.push_back(action);
            action->act(*this);
        }
        else if (commandType == "changeuser") {
                std::string afterFirstWord = command.substr(command.find(' ') + 1);
                std::string userName = afterFirstWord.substr(0, afterFirstWord.find_first_of(' '));

                ChangeActiveUser* action = new ChangeActiveUser(userName);
                actionsLog.push_back(action);
                action->act(*this);
        }
        else if (commandType == "deleteuser") {
                std::string afterFirstWord = command.substr(command.find(' ') + 1);
                std::string userName = afterFirstWord.substr(0, afterFirstWord.find_first_of(' '));

                DeleteUser* action = new DeleteUser(userName);
                actionsLog.push_back(action);
                action->act(*this);
        }
        else if (commandType == "dupuser") {
            std::string afterFirstWord = command.substr(command.find(' ') + 1);
            std::string originUserName = afterFirstWord.substr(0, afterFirstWord.find_first_of(' '));
            std::string newUserName = afterFirstWord.substr(afterFirstWord.find(' ') + 1);
            if((newUserName != "") & (originUserName != "")){
                DuplicateUser* action = new DuplicateUser(originUserName, newUserName);
                actionsLog.push_back(action);
                action->act(*this);
            }
        }
        else if (commandType == "content") {
            PrintContentList* action = new PrintContentList();
            actionsLog.push_back(action);
            action->act(*this);
        }
        else if (commandType == "watchlist") {
            PrintWatchHistory* action = new PrintWatchHistory();
            actionsLog.push_back(action);
            action->act(*this);
        }
        else if (commandType == "watch") {
            std::string afterFirstWord = command.substr(command.find(' ') + 1);
            std::string contentId = afterFirstWord.substr(0, afterFirstWord.find_first_of(' '));

            if (is_number(contentId)) { // check if the content id is ok
                int id = stoi(contentId);
                if (id % 1 == 0) {
                    Watch* action = new Watch(id);
                    actionsLog.push_back(action);
                    action->act(*this);
                }
                else {
                    std::cout << "wrong Id" << std::endl;
                }
            }
            else
            {
                std::cout << "wrong Id" << std::endl;
            }
        }
        else if(commandType == "log"){
            PrintActionsLog* action = new PrintActionsLog();
            actionsLog.push_back(action);
            action->act(*this);
            action->setStatus(COMPLETED);
        }
        else if (command != ""){
            std::cout << "unrecognized command \n";
        }
        std::cout << "insert a command";
        std::getline(std::cin, command);

    }
    if(command == "exit"){
        Exit *action = new Exit();
        action->act(*this);
        actionsLog.push_back(action);
    }
    else{
        std::cout << "unrecognized command";
    }
}
void Session::printContentList(PrintContentList &action) {
    int i = 1;
    for (auto &element : content) {
        std::cout << std::to_string(i) + ". " + element->toString() + element->printLengthAndTags() << std::endl;
        i++;
    }
}
void Session::printWatchHistory() {
    int i = 1;
    std::cout << "Watch history for " + activeUser->getName() << std::endl;
    for (auto &content : activeUser->get_history()) {
        std::cout << std::to_string(i) + ". " + content->toString() << std::endl;
        i++;
    }
}
void Session::printActionLog(){
    for (int i = actionsLog.size()-2; i >= 0; i--) {
        std::cout << (actionsLog.at(i))->toString() << std::endl;
    }
}
void Session::addUserToMap(User* user) {
    userMap.insert({user->getName(), user});
}
std::string Session::duplicateUser(DuplicateUser &action) {
    std::string oldUserName = action.getOriginUserName();
    std::string newUserName = action.getNewUserName();

    // if old user exist
    if (userMap.find(oldUserName) != userMap.end()) {

        // if new user doesn't exist
        if(userMap.find(newUserName) == userMap.end()){
            // duplicate
            addUserToMap(&(userMap.find(oldUserName)->second->cloneUser(newUserName)));

            return "";
        }
        else{
            return "new user name is taken";
        }
    }
    else {
        return "user to duplicate doesn't exist";
    }
}
std::string Session::watchContentById(Watch &action) {
    if((unsigned)action.getContentId() <= (unsigned)content.size()) {
        long index = action.getContentId()-1;
        if((index >= 0) & (index <(unsigned) content.size())){
            Watchable* cont = content.at(index);
            std::cout << "Watching " + cont->toString() << std::endl;
            activeUser->addToHistory(cont);

            Watchable* suggestion = cont->getNextWatchable(*this);
            if(suggestion) {
                std::string command;
                std::cout << "We recommend watching " + suggestion->toString() + " continue watching?[y/n]" << std::endl;
                std::getline(std::cin, command);
                if ((command == "y") | (command == "Y")) {
                    Watch *action = new Watch(suggestion->getContentId());
                    actionsLog.push_back(action);
                    action->act(*this);
                }
            }
        }
        else {
            //TODO:: what if no content
        }
        return "";
    }
    else {
        return "Wrong ID";
    }
}
std::string Session::createUser(CreateUser &action) {
    // check if user exist already
    if (userMap.find(action.getUserName()) != userMap.end()) {
        return "username is already in use";
    }
    else {
        std::string typeOfUser = action.getUserRecAlgo();
        std::string userName = action.getUserName();

        if (typeOfUser == "len") {
            LengthRecommenderUser *user = new LengthRecommenderUser(userName);
            userMap.insert({userName, user});
        } else if (typeOfUser == "rer") {
            RerunRecommenderUser *user = new RerunRecommenderUser(userName);
            userMap.insert({userName, user});
        } else if (typeOfUser == "gen") {
            GenreRecommenderUser *user = new GenreRecommenderUser(userName);
            userMap.insert({userName, user});
        }
        else{
            return "recommendation algorithm is invalid";
        }
        return "";
    }
}
std::string Session::changeActiveUser(ChangeActiveUser &action) {
    std::string userName = action.getUserName();
    if (userMap.find(userName) != userMap.end()) {
        activeUser = userMap.at(userName);

        return "";
    }
    else {
        return "User Doesn't Exist";
    }
}
std::string Session::deleteUser(DeleteUser &action) {
    std::string userName = action.getUserName();
    if (userMap.find(userName) != userMap.end()) {
        auto usr = userMap.find(userName);
        delete usr->second;
        userMap.erase(userName);
        return "";
    }
    else {
        action.setStatus(ERROR);
        return "User Doesn't Exist";
    }
}
Watchable * Session::getContentAtIndex(int i) {
    return content[i];
}
Watchable *Session::getClosestTimeWatchable(double avg, LengthRecommenderUser* user) {
    int closestLength = std::numeric_limits<int>::max();
    //user->get_history();
    int index = 0;
    int chosen = -1;
    for(auto& item : content){
        if(!user->userWatched(item)){
            if(abs(item->getLength()-avg) < closestLength){
                chosen = index;
                closestLength = abs(item->getLength()-avg);
            }
        }
        index ++;
    }
    return content.at(chosen);
}
Watchable* Session::findcontentByGenre(std::string genre) {

    for(auto& watchable : content){
        if(watchable->hasTag(genre) && !activeUser->userWatched(watchable)){
            return watchable;
        }
    }
    return  nullptr;
}
bool Session::is_number(const std::string& s) { // check if string is a number
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
User *Session::getActiveUser() {
    return activeUser;
}

// regular constructor
Session::Session(const std::string &configFilePath): def("default"), content(), actionsLog(), userMap(), activeUser(new LengthRecommenderUser(def)){
    userMap.insert({activeUser->getName(),activeUser});
    // read a JSON file
    using json = nlohmann::json;
    std::ifstream i(configFilePath);
    json j;
    i >> j;

    long id = 1;

    // insert all movies to watchable vector
    for (auto &element : j["movies"].items()) {
        std::vector<std::string> tags;

        // get movie tags
        for (auto &subElement : element.value()["tags"].items()) {
            tags.push_back(subElement.value());
        }

        Movie* movie =  new Movie(id, element.value()["name"],element.value()["length"], tags);

        content.push_back(movie);
        id++;
    }

    // insert all tv-series to watchable vector
    for (auto &element : j["tv_series"].items()) {
        std::vector<std::string> tags;

        // get series tags
        for (auto &subElement : element.value()["tags"].items()) {
            tags.push_back(subElement.value());
        }

        int numOfSeasons = element.value()["seasons"].size();
        for (int season = 1; season <= numOfSeasons; season++) {
            int numOfEpisodes = element.value()["seasons"][season-1];
            for (int episode = 1; episode <= numOfEpisodes ; episode++) {
                Episode* newEpisode = new Episode(id, element.value()["name"], element.value()["episode_length"],
                                                  season, episode, tags);
                content.push_back(newEpisode);

                if((season == numOfSeasons) & (episode == numOfEpisodes)){
                    newEpisode->setNextWatchableId(-1);
                }
                else{
                    newEpisode->setNextWatchableId(id+1);
                }
                id++;
            }
        }
    }
}

// rule of 5 below
// copy constructor
Session::Session(const Session &other) : def("default"), content(), actionsLog(), userMap(), activeUser() { // deep copy without deleting existing other
    // deep copy content
    for(auto& watchable : other.content){
        Watchable &newWatchable = watchable->cloneWatchable();
        this->content.push_back(&newWatchable);
    }

    // deep copy actions
    for(auto& action : other.actionsLog){
        BaseAction &newAction = action->cloneAction();
        this->actionsLog.push_back(&newAction);
    }

    // deep copy of users
    for(auto& user : other.userMap){
        User &newUser = user.second->cloneUser(user.second->getName());
        newUser.clearHistory();

        // copy user history to the new content vector
        for(auto& otherWatchable_ptr : user.second->get_history()){
            long id = otherWatchable_ptr->getContentId() - 1;
            newUser.addToHistory(this->content.at(id));
        }
        this->userMap.insert({newUser.getName(),&newUser});
    }
    this->activeUser = this->userMap.find(other.activeUser->getName())->second;
}

// copy assignment operator
Session &Session::operator=(const Session &other) { // delete my previous stuff and copy the new stuff
    if( &other == this) {
        return *this;
    }

    // delete the stuff this has
    for(auto& item: this->content){
        delete item;
    }
    for(auto& item: this->actionsLog){
        delete item;
    }
    for(auto& item: this->userMap){
        delete item.second;
    }

    this->content.clear();
    this->actionsLog.clear();
    this->userMap.clear();
    this->activeUser = nullptr;

    // deep copy content
    for(auto& watchable : other.content){
        Watchable &newWatchable = watchable->cloneWatchable();
        this->content.push_back(&newWatchable);
    }

    // deep copy actions
    for(auto& action : other.actionsLog){
        BaseAction &newAction = action->cloneAction();
        this->actionsLog.push_back(&newAction);
    }

    // deep copy of users
    for(auto& user : other.userMap){
        User &newUser = user.second->cloneUser(user.second->getName());
        newUser.clearHistory();

        // copy user history to the new content vector
        for(auto& otherWatchable_ptr : user.second->get_history()){
            long id = otherWatchable_ptr->getContentId();
            newUser.addToHistory(this->content.at(id));
        }
        this->userMap.insert({newUser.getName(),&newUser});
    }

    this->activeUser = this->userMap.find(other.activeUser->getName())->second;
    return *this;
}

// move constructor
Session::Session(Session &&other) : def("default"), content(), actionsLog(), userMap(), activeUser() { // data will just point to where the other data is pointing to
    // steal other's resources
    this->content = other.content;
    this->actionsLog = other.actionsLog;
    this->userMap = other.userMap;
    this->activeUser = other.activeUser;

    // delete other's resources (this is just a deletion of pointers, actual values seat somewhere else and now "this" points to them)
    for(auto& item: other.content){
        item = nullptr;
    }
    for(auto& item: other.actionsLog){
        item = nullptr;
    }
    for(auto& item: other.userMap){
        item.second = nullptr;
    }
    other.activeUser = nullptr;
}

// move assignment operator
Session &Session::operator=(Session &&other) {
   if(&other == this){
       return *this;
   }
    // delete the stuff this has
    for(auto& item: this->content){
        delete item;
    }
    this->content.clear();
    for(auto& item: this->actionsLog){
        delete item;
    }
    this->actionsLog.clear();
    for(auto& item: this->userMap){
        delete item.second;
    }
    this->userMap.clear();

    this->activeUser = nullptr;

    // steal other's resources
    this->content = other.content;
    this->actionsLog = other.actionsLog;
    this->userMap = other.userMap;
    this->activeUser = other.activeUser;

    // delete other's resources (this is just a deletion of pointers, actual values seat somewhere else and now "this" points to them)
    for(auto& item: other.content){
        item = nullptr;
    }
    other.content.clear();
    for(auto& item: other.actionsLog){
        item = nullptr;
    }
    other.actionsLog.clear();
    for(auto& item: other.userMap){
        item.second = nullptr;
    }
    other.userMap.clear();
    other.activeUser = nullptr;

    return *this;
}

// destructor
Session::~Session() {
    // delete users
    for(auto& item : userMap){
        delete item.second;
        item.second = nullptr;
    }
    // delete actions
    for(auto& item : actionsLog){
        delete item;
        item = nullptr;
    }
    // delete washables
    for(auto& item : content){
        delete item;
        item = nullptr;
    }
    userMap.clear();
    actionsLog.clear();
    content.clear();
    activeUser = nullptr;
}