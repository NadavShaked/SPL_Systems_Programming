#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "User.h"

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction& cloneAction()=0;
    void setStatus(ActionStatus status);
    void setErrorMsg(std::string errorMsg);

protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser : public BaseAction {
public:
    CreateUser(std::string userName, std::string reccomendAlgo);
    ~CreateUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual CreateUser& cloneAction();
    std::string getUserName() const;
    std::string getUserRecAlgo() const;

private:
    std::string userName;
    std::string reccomendAlgo;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(std::string userName);
    ~ChangeActiveUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ChangeActiveUser& cloneAction();
    std::string getUserName() const;

private:
    std::string userName;
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string userName);
    ~DeleteUser();
    virtual void act(Session & sess);
    virtual DeleteUser& cloneAction();
    virtual std::string toString() const;
    std::string getUserName() const;

private:
    std::string userName;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(std::string originalUserName, std::string newUserName);
    ~DuplicateUser();
    virtual DuplicateUser& cloneAction();
    virtual void act(Session & sess);
    virtual std::string toString() const;
    std::string getNewUserName() const;
    std::string getOriginUserName() const;

private:
    std::string newUserName;
    std::string originalUserName;
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    ~PrintContentList();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintContentList& cloneAction();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    ~PrintWatchHistory();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual PrintWatchHistory& cloneAction();
};


class Watch : public BaseAction {
public:
    Watch(int id);
    ~Watch();
    virtual Watch& cloneAction();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    int getContentId();

private:
    int id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    ~PrintActionsLog();
    virtual PrintActionsLog& cloneAction();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual Exit& cloneAction();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    ~Exit();
};
#endif