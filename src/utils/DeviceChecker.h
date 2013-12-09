#ifndef DEVICECHECKER_H
#define DEVICECHECKER_H

#include <QThread>
#include <QVariant>
#include <QEventLoop>

#include "defines.h"

class AdbCommander;
class FastBootCommander;
class DeviceChecker : public QThread
{
    Q_OBJECT
public:
    explicit DeviceChecker(QObject *parent = 0);
    ~DeviceChecker();

private:
    void init();

protected:
    void run();

signals:
    void message(const QVariant &msg);
    void deviceStateChanged(int state);
    void deviceConnected();

public slots:
    void checkDeviceState();
    void enableCheckDevice(bool enabled);
    void waitForDevice();
    void setQuitFlag(bool flag);

private slots:
    void waitForDevice_p();

private:
    AdbCommander *adbCommander;
    FastBootCommander *fastbootCommander;
    DeviceState currentDeviceState;
    bool enableCheck;
    QEventLoop eventloop;
    bool quitFlag;
};

#endif // DEVICECHECKER_H
