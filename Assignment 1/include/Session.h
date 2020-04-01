#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "../include/Action.h"
#include "../include/json.hpp"
#include "fstream"

class User;
class Watchable;

class Session{
public:
    // regular constructor
    Session(const std::string &configFilePath);

    // copy constructor
    Session(const Session &other);

    // move constructor
    Session(Session &&other);

    // copy assignment operator
    Session& operator = (const Session & other);

    // move assignment operator
    Session& operator = (Session &&other);

    //destructor
    ~Session();

    // methods
    void start();
    std::string createUser(CreateUser &action);
    std::string changeActiveUser(ChangeActiveUser &action);
    std::string deleteUser(DeleteUser &action);
    std::string duplicateUser(DuplicateUser &action);
    void printContentList(PrintContentList &action);
    void printWatchHistory();
    void printActionLog();
    void addUserToMap(User* user);
    Watchable* findcontentByGenre(std::string genre);
    Watchable * getContentAtIndex(int i);
    Watchable * getClosestTimeWatchable(double avg, LengthRecommenderUser* user);
    std::string watchContentById(Watch &action);
    User* getActiveUser();
    bool is_number(const std::string& s);

private:
    // fields
    std::string def;
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;


};
#endif