#include "DeviceChecker.h"
#include "AdbCommander.h"
#include "fastboot.h"

#include <QTimer>
#include <QProcess>
#include <QRegExp>
#include <QStringList>
#include <QThreadPool>
#include <QDebug>

DeviceChecker::DeviceChecker(QObject *parent) :
    QThread(parent)
{
    init();
}

DeviceChecker::~DeviceChecker()
{
    if (adbCommander != NULL) {
        delete adbCommander;
        adbCommander = NULL;
    }
    QThreadPool::globalInstance()->waitForDone(3000);
}

void DeviceChecker::init()
{
    adbCommander = NULL;
    enableCheck = true;
    quitFlag = false;
    currentDeviceState = DS_Disconnected;
    fastbootCommander = new FastBootCommander(this);
    connect(fastbootCommander, SIGNAL(message(QVariant)), this, SIGNAL(message(QVariant)));
}

void DeviceChecker::run()
{
    adbCommander = new AdbCommander();
    connect(adbCommander, SIGNAL(message(QVariant)), this, SIGNAL(message(QVariant)));

    QTimer timer;
    timer.setInterval(5000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkDeviceState()), Qt::DirectConnection);
    timer.start();
    exec();
}

void DeviceChecker::checkDeviceState()
{
    if (adbCommander != NULL && enableCheck) {
        QStringList devices = adbCommander->deviceList();
        if (devices.count() > 0) {
            currentDeviceState = DS_Connected;
            emit deviceStateChanged(DS_Connected);
        }
        else {
            currentDeviceState = DS_Disconnected;
            emit deviceStateChanged(DS_Disconnected);
        }
    }
    if (quitFlag) {
        disconnect(this);
        quit();
    }
}

void DeviceChecker::enableCheckDevice(bool enabled)
{
    enableCheck = enabled;
}

void DeviceChecker::waitForDevice()
{
    QTimer timer;
    timer.setInterval(3000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(waitForDevice_p()));
    timer.start();

    eventloop.exec();
}

void DeviceChecker::setQuitFlag(bool flag)
{
    quitFlag = flag;
}

void DeviceChecker::waitForDevice_p()
{
    QStringList devices = fastbootCommander->deviceList();
    if (devices.count() > 0) {
        currentDeviceState = DS_Connected;
        emit deviceStateChanged(DS_Connected);
        emit deviceConnected();
        eventloop.quit();
    }
    else {
        currentDeviceState = DS_Disconnected;
        emit deviceStateChanged(DS_Disconnected);
    }
    if (quitFlag) {
        disconnect(this);
        quit();
    }
}
