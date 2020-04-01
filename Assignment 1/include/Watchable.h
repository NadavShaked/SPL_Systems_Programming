#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    // constructors
    Watchable(long id, int length, const std::vector<std::string>& tags);
    Watchable(const Watchable &other);

    //destructor
    virtual ~Watchable();

    //virtual methods
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session& sess) const =0;
    virtual int getLength();
    virtual Watchable& cloneWatchable() = 0;


    // regular methods
    std::string printLengthAndTags() const;
    std::string printTags() const;
    bool hasTag(std::string tag);

    // getters
    int getContentId() const;
    std::vector<std::string> getTags();

private:
    // fields
    const long id;
    int length;
    std::vector<std::string> tags;

};

class Movie : public Watchable{
public:
    // constructors
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie &other);

    // virtual methods
    virtual std::string toString() const;
    Watchable* getNextWatchable(Session& sess) const ;
    virtual Movie& cloneWatchable();

private:
    // fields
    std::string name;
};


class Episode: public Watchable{
public:
    // constructors
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode &other);

    // virtual methods
    virtual std::string toString() const;
    Watchable* getNextWatchable(Session& sess) const ;
    virtual Episode& cloneWatchable();

    // setters
    void setNextWatchableId(long nextId);

private:
    // fields
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif