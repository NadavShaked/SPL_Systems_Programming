//
// Created by danyard@wincs.cs.bgu.ac.il on 21/11/2019.
//
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

// USER type User
User::User( std::string &name) : history(), name(name){
    // make sure that the initialized vector is made in stack and copy to history by deep copy
    history = std::vector<Watchable*>();
}

// copy constructor
User::User(const User &other) : history(other.history), name(other.name) {}

// copy assignment operator
User& User::operator=(const User& other) {
    if(this != &other){
        history.clear();
    }
    name = other.name;
    history = other.history;

    return *this;
}

// move constructor
User::User(User &&other) : history(other.history), name(other.name) {
    other.name = nullptr;
    other.history.clear();
}

// move assignment operator
User& User::operator=(User &&other) {
    name = other.name;
    history = other.history;
    other.name = nullptr;
    other.history.clear();

    return *this;
}

// distractor
User::~User() {
    history.clear();
}

// add watched to history
void User::addToHistory(Watchable* watchable) {
    history.push_back(watchable);
}

std::string User::getName() {
    return name;
}

// get history vector
std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::setName(std::string nameToSet) {
    name =nameToSet;
}

// methods
bool User::userWatched(Watchable *watchable) {
    std::vector<Watchable*>::iterator iterator;
    iterator = std::find(history.begin(),history.end(),watchable);

// if watched
    if(iterator != history.end() ){
        return true;
    }
    return false;
}

std::pair<std::string, int> User::getMaxFromMap(std::map<std::string, int>* map) {
    std::pair<std::string, int> maxPair = {"", 0};
    for (auto &item : *map) {
        if (item.second > maxPair.second) {
            maxPair = item;
        } else if ((item.second == maxPair.second) & (item.first < maxPair.first)) {
            maxPair = item;
        }
    }
    map->erase(maxPair.first);
    return maxPair;
}

void User::clearHistory() {
    history.clear();
}

//
// LENGTH type user

// regular constructor
LengthRecommenderUser::LengthRecommenderUser(std::string &name) : User(name), avg() {}

// copy constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other) : User(other), avg(other.avg) {}

// move constructor
LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&other) : User(other), avg(other.avg) {
    other.avg = 0;
}

// return new user of type length
LengthRecommenderUser &LengthRecommenderUser::cloneUser(std::string newName){
    LengthRecommenderUser* userClone = new LengthRecommenderUser(*this);
    userClone->setName(newName);
    return *userClone;
}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int sum = 0;
    for(auto& item: history){
        sum = sum + item->getLength();
    }
    double average = sum/(history.size());

    return s.getClosestTimeWatchable(average,this);
}
//
// RERUN type user

// constructor
RerunRecommenderUser::RerunRecommenderUser( std::string &name) :  User(name), lastRecIndex(-1) {}

// copy constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other) : User(other), lastRecIndex(other.lastRecIndex) {}

// move constructor
RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser &&other) : User(other), lastRecIndex(other.lastRecIndex) {
    other.lastRecIndex = 0;
}

int RerunRecommenderUser:: getLastRecIndex() {
    return lastRecIndex;
}

// methods
Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    if(lastRecIndex == -1) {
        return history.at(0);
    }
    else {
        int nextIndexToWatch = (lastRecIndex+1) % history.size();
        lastRecIndex = nextIndexToWatch;
        return history.at(nextIndexToWatch);
    }
}

RerunRecommenderUser &RerunRecommenderUser::cloneUser(std::string newName){
    RerunRecommenderUser* userClone = new RerunRecommenderUser(*this);
    userClone->setName(newName);
    return *userClone;
}

// GENRE user type
// constructor
GenreRecommenderUser::GenreRecommenderUser(std::string &name) : User(name) {}

// copy constructor
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other) : User(other) {}

// move constructor
GenreRecommenderUser::GenreRecommenderUser(GenreRecommenderUser &&other) : User(other) {}

// methods
Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    std::map<std::string,int> genreMap;

    // build map by frequency of tag appearance.
    for(auto& watchable: history){
        for(auto& tag: watchable->getTags()){
            auto iterator = genreMap.find(tag);
            if(iterator != genreMap.end()){
                iterator->second ++ ;
            }
            // didn't see this genre before
            else{
                genreMap.insert({tag,1});
            }
        }
    }

    Watchable* watchable = s.findcontentByGenre(getMaxFromMap(&genreMap).first);

    while ((!genreMap.empty()) & (watchable == nullptr)) {
        watchable = s.findcontentByGenre(getMaxFromMap(&genreMap).first);
    }

    if (watchable == nullptr) {
        // return - error
    }
    return watchable;
}

GenreRecommenderUser &GenreRecommenderUser::cloneUser(std::string newName){
    GenreRecommenderUser* userClone = new GenreRecommenderUser(*this);
    userClone->setName(newName);
    return *userClone;
}