#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>

class Watchable;
class Session;

class User{
public:
    // constructors
    User(std::string& name);
    User(const User &other);
    User(User&& other);
    User& operator=(const User& other);
    User& operator=(User&& other);

    // destructor
    virtual ~User();

    // regular methods
    bool userWatched(Watchable* watchable);
    std::pair <std::string, int> getMaxFromMap(std::map<std::string,int>* genreMap);
    void clearHistory();

    // virtual methods
    virtual void addToHistory(Watchable* watchable);
    virtual Watchable* getRecommendation(Session& s)=0;
    virtual User& cloneUser(std::string newName)=0;

    //getters
    std::string getName();
    std::vector<Watchable*> get_history() const;

protected:
    //fields
    std::vector<Watchable*> history;

    // setters
    void setName(std::string nameToSet);

private:
    // fields
    std::string name;
};

class LengthRecommenderUser : public User {
public:
    // constructors
    LengthRecommenderUser(std::string& name);
    LengthRecommenderUser(const LengthRecommenderUser &other);
    LengthRecommenderUser(LengthRecommenderUser &&other);
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other);
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other);

    // virtual methods
    virtual Watchable* getRecommendation(Session& s);
    virtual LengthRecommenderUser& cloneUser(std::string newName);


protected:

private:
    // fields
    double avg;
};

class RerunRecommenderUser : public User {
public:
    // constructors
    RerunRecommenderUser(std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser &other);
    RerunRecommenderUser(RerunRecommenderUser &&other);
    RerunRecommenderUser& operator=(const RerunRecommenderUser& other);
    RerunRecommenderUser& operator=(RerunRecommenderUser&& other);

    // virtual methods
    virtual Watchable* getRecommendation(Session& s);
    virtual RerunRecommenderUser& cloneUser(std::string newName);


    // getters
    int getLastRecIndex();

private:
    int lastRecIndex;
};

class GenreRecommenderUser : public User {
public:
    // constructors
    GenreRecommenderUser(std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser &other);
    GenreRecommenderUser(GenreRecommenderUser &&other);
    GenreRecommenderUser& operator=(const GenreRecommenderUser& other);
    GenreRecommenderUser& operator=(GenreRecommenderUser&& other);

    // virtual methods
    virtual Watchable* getRecommendation(Session& s);
    virtual GenreRecommenderUser& cloneUser(std::string newName);

private:
};

#endif