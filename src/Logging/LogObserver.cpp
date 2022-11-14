//
// Created by danie on 2022-11-02.
//

#include "LogObserver.h"

LogObserver *LogObserver::_instance = nullptr;

void LogObserver::Update(ILoggable *log) { log->stringToLog(); }

LogObserver::LogObserver() { _instance = this; }

LogObserver *LogObserver::instance() { return _instance; }

LogObserver::~LogObserver() { _instance = nullptr; }
