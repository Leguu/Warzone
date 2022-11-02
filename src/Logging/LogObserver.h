//
// Created by danie on 2022-11-02.
//

#ifndef WARZONE_LOGOBSERVER_H
#define WARZONE_LOGOBSERVER_H
#include "LoggingObserver.h"
class LogObserver : public Observer{
    void Update(ILoggable* log) override;
    static LogObserver *_instance;
public:
    static LogObserver *instance();
    LogObserver();
    ~LogObserver();
};

#endif //WARZONE_LOGOBSERVER_H
