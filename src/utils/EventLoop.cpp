#include "EventLoop.h"

EventLoop::EventLoop(QObject *parent) :
    QEventLoop(parent)
{
    isRunning = false;
}

bool EventLoop::running() const
{
    return isRunning;
}

void EventLoop::setRunning(bool running)
{
    if (isRunning != running) {
        isRunning = running;
        emit runningChanged();
    }
    if (isRunning) {
        exec();
    }
    else {
        quit();
    }
}
