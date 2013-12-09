#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <QEventLoop>

class EventLoop : public QEventLoop
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit EventLoop(QObject *parent = 0);
    bool running() const;
    void setRunning(bool running);

signals:
    void runningChanged();

public slots:

private:
    bool isRunning;

};

#endif // EVENTLOOP_H
