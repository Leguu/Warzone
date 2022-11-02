//
// Created by danie on 2022-11-02.
//

#include "LogObserver.h"

void LogObserver::Update(ILoggable *log) {
    log->stringToLog();
}
