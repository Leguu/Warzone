//
// Created by danie on 2022-11-02.
//

#include "LoggingObserver.h"

Subject::Subject() { _observers = new list<Observer *>; }

Subject::~Subject() { _observers->clear(); }

void Subject::Attach(Observer *o) {
  if (o != nullptr) {
	_observers->push_back(o);
  }
};

void Subject::Detach(Observer *o) {
  if (o != nullptr) {
	_observers->remove(o);
  }
};

void Subject::Notify(ILoggable *log) {
  auto i = _observers->begin();
  for (; i != _observers->end(); ++i)
	(*i)->Update(log);
};

Observer::~Observer() = default;

Observer::Observer() = default;
