//
// Created by danie on 2022-11-02.
//

#ifndef WARZONE_LOGGINGOBSERVER_H
#define WARZONE_LOGGINGOBSERVER_H
#include <list>
#include <string>
using std::list;
using std::string;

class ILoggable {
public:
  virtual std::string stringToLog() = 0;
  virtual ~ILoggable() = default;
};

class Observer {
public:
  ~Observer();
  virtual void Update(ILoggable *log) = 0;

protected:
  Observer();
};

class Subject {
public:
  virtual void Attach(Observer *o);
  virtual void Detach(Observer *o);
  virtual void Notify(ILoggable *log);
  Subject();
  ~Subject();

private:
  list<Observer *> *_observers;
};

#endif // WARZONE_LOGGINGOBSERVER_H
