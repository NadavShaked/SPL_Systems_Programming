//
// Created by danyard@wincs.cs.bgu.ac.il on 21/11/2019.
//
#include "../include/Watchable.h"
#include "../include/Session.h"
//
// WATCHABLE
//
// constractors
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(tags) {}

// copy constructor
Watchable::Watchable(const Watchable &other) : id(other.id), length(other.length), tags(other.tags) {}

// destructor
Watchable::~Watchable() { tags.clear(); }

// methods
std::string Watchable::printLengthAndTags() const {
    std::string len = std::to_string(length);
    std::string tags = printTags();
    std::string ret = " " + len + " minutes " + tags;
    return ret;
}
std::string Watchable::printTags() const {
    std::string ret = "[";
    for (auto &tag : tags) {
        ret += tag + ", ";
    }
    return ret.substr(0, ret.length() - 2) + "]";
}

// getters
int Watchable::getContentId() const { return id; }

int Watchable::getLength() {
    return length;
}

std::vector<std::string> Watchable::getTags() {
    return tags;
}

bool Watchable::hasTag(std::string tag) {
    for(auto& item : tags){
        if(item == tag){
            return true;
        }
    }
    return false;
}

//
// MOVIE
//

// constructors
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags) : Watchable(id, length, tags), name(name) {}

// copy constructor
Movie::Movie(const Movie &other) : Watchable(other), name(other.name) {}

// methods
std::string Movie::toString() const {
    std::string movieName = this -> name ;
    std::string ret = movieName;

    return ret;
}

Watchable* Movie::getNextWatchable(Session &sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}

Movie &Movie::cloneWatchable() {
    std::vector<std::string> const newTagVector = this->getTags();
    std::string const nameForNew = this->name;
    long id = this->getContentId();
    int length = this->getLength();
    Movie *movie = new Movie(id,nameForNew,length,newTagVector);
    return *movie;
}

//
// EPISODE
//

// constructors
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) : Watchable(id, length, tags), seriesName (seriesName), season (season), episode(episode), nextEpisodeId(id + 1) {}

// copy constructor
Episode::Episode(const Episode &other) : Watchable(other), seriesName(other.seriesName), season(other.season), episode(other.episode), nextEpisodeId(other.nextEpisodeId) {}

// methods
std::string Episode::toString() const {
    std::string seriesName = this -> seriesName ;
    std::string numOfSeason = std::to_string(season);
    std::string numOfEpisode = std::to_string(episode);

    if (season < 10)
        numOfSeason = "0" + numOfSeason;
    if (episode < 10)
        numOfEpisode = "0" + numOfEpisode;

    std::string ret = seriesName + " S" + numOfSeason + "E" + numOfEpisode;

    return ret;
}

Watchable* Episode::getNextWatchable(Session &sess) const {
    // if it is the last episode of the series use recommendation algorithm
    if(this->nextEpisodeId == (-1)){
        return sess.getActiveUser()->getRecommendation(sess);
    }
    else{// fetch next episode
        return sess.getContentAtIndex(nextEpisodeId-1);
    }
}

Episode &Episode::cloneWatchable() {
    std::vector<std::string> const newTagVector = this->getTags();
    std::string const nameForNew = this->seriesName;
    long id = this->getContentId();
    int length = this->getLength();
    Episode *episode = new Episode(id,nameForNew,length,this->season,this->episode,newTagVector);
    return *episode;
}

// setters
void Episode::setNextWatchableId(long nextId) {
    nextEpisodeId = nextId;
}