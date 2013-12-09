#include "AdbCommander.h"

#include <QProcess>
#include <QThreadPool>
#include <QDir>
#include <QApplication>
#include <QTime>
#include <QTextCodec>
#include <QDebug>

#define ADBPROCESS(command, parameter, timeout) \
    QTextCodec *codec = QTextCodec::codecForLocale(); \
    QProcess process; \
    process.setProcessChannelMode(QProcess::MergedChannels); \
    if (command != "deviceList") \
        qDebug() << "adb:" << parameter; \
    process.start(adbPath(), parameter); \
    if (!process.waitForStarted()) { \
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("Start Adb process failed!"))); \
        QVariantMap result; \
        result["success"] = false; \
        result["command"] = command; \
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result)); \
        return; \
    } \
    QByteArray outbuf; \
    QTime time; \
    time.start(); \
    while (process.state() == QProcess::Running) { \
        if (process.waitForReadyRead(1000)){ \
            outbuf += process.readAll(); \
        } \
        if (timeout > 0 && time.elapsed() > timeout){ \
            QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("Adb timeout!"))); \
            QVariantMap result; \
            result["success"] = false; \
            result["command"] = command; \
            QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result)); \
            return; \
        } \
    } \
    outbuf += process.readAll(); \
    QProcess::ExitStatus exitStatus = process.exitStatus(); \
    if (exitStatus == QProcess::CrashExit) { \
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("Adb process crashed!"))); \
        QVariantMap result; \
        result["success"] = false; \
        result["command"] = command; \
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result)); \
        return; \
    } \
    process.waitForFinished(3000); \
    outbuf += process.readAll(); \
    QString outstr = codec->toUnicode(outbuf); \
    if (command != "deviceList") \
        qDebug() << "adb out:" << parameter;


AdbCommander::AdbCommander(QObject *parent) :
    QObject(parent)
{
}

bool AdbCommander::reboot(const QString &mode)
{
    QStringList args;
    if (mode == "ruu") {
        args << "reboot" << "oem-78";
    }
    else if (mode == "bootloader") {
        args << "reboot-bootloader";
    }
    else {
        args << "reboot" << mode;
    }

    createAdbRunner(AdbRunner::REBOOT, args);

    return result.toMap()["success"].toBool();
}

QString AdbCommander::shell(const QString &cmd)
{
    QStringList args;
    args << "shell" << cmd;
    createAdbRunner(AdbRunner::SHELL, args);

    return result.toMap()["content"].toString();
}

bool AdbCommander::pushFile(const QString &srcFile, const QString &destFile)
{
    QVariantMap parameter;
    parameter["srcFile"] = srcFile;
    parameter["destFile"] = destFile;
    createAdbRunner(AdbRunner::PUSHFILE, parameter);

    return result.toMap()["success"].toBool();
}

bool AdbCommander::pullFile(const QString &srcFile, const QString &destFile)
{
    QVariantMap parameter;
    parameter["srcFile"] = srcFile;
    parameter["destFile"] = destFile;
    createAdbRunner(AdbRunner::PULLFILE, parameter);

    return result.toMap()["success"].toBool();
}

QStringList AdbCommander::deviceList()
{
    createAdbRunner(AdbRunner::DEVICELIST);

    QString strMsg = result.toMap()["content"].toString();
    QStringList lines = strMsg.split(QRegExp("[\\r\\n]"), QString::SkipEmptyParts);
    QStringList attachedList;
    foreach (QString line, lines) {
        if (line.contains("device") && !line.contains("devices")) {
            QStringList lineParts = line.split(QRegExp("[\\t\\s]"), QString::SkipEmptyParts);
            if (lineParts.count() > 0)
                attachedList << lineParts[0];
        }
    }
    return attachedList;
}

void AdbCommander::onFinished(const QVariant &result)
{
    this->result = result;
    eventloop.quit();
}

void AdbCommander::createAdbRunner(int cmdID, const QVariant &parameter)
{
    AdbRunner *adbRunner = new AdbRunner(this);
    adbRunner->setCommand((AdbRunner::MethodID)cmdID, parameter);
    QThreadPool::globalInstance()->start(adbRunner);
    eventloop.exec();
}


AdbRunner::AdbRunner(QObject *commander)
{
    m_commander = commander;
}

QString AdbRunner::adbPath()
{
    QDir dir = qApp->applicationDirPath();
    dir.cd("tools");
    return dir.absoluteFilePath("adb");
}

void AdbRunner::run()
{
    switch (m_commandID) {
    case REBOOT: reboot(); break;
    case SHELL: shell(); break;
    case PUSHFILE: pushFile(); break;
    case PULLFILE: pullFile(); break;
    case DEVICELIST: deviceList(); break;
    default: break;
    }
}

void AdbRunner::setCommand(AdbRunner::MethodID cmdID, const QVariant &parameter)
{
    m_commandID = cmdID;
    m_parameter = parameter;
}

void AdbRunner::reboot()
{
    ADBPROCESS("reboot", m_parameter.toStringList(), 15000);

    QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("Reboot success!")));
    QVariantMap result;
    result["success"] = true;
    result["command"] = "reboot";
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void AdbRunner::shell()
{
    ADBPROCESS("shell", m_parameter.toStringList(), 15000);

    QVariantMap result;
    result["success"] = true;
    result["command"] = "shell";
    result["content"] = outstr;
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void AdbRunner::pushFile()
{
    QVariantMap parameter = m_parameter.toMap();
    QStringList args;
    args << "push" << parameter["srcFile"].toString();
    args << parameter["destFile"].toString();

    ADBPROCESS("pushFile", args, 15000);

    QVariantMap result;
    result["command"] = "pushFile";
    if (!outstr.contains("error")) {
        result["success"] = true;
    }
    else {
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, outstr));
        result["success"] = false;
        result["content"] = outstr;
    }
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void AdbRunner::pullFile()
{
    QVariantMap parameter = m_parameter.toMap();
    QStringList args;
    args << "pull" << parameter["srcFile"].toString();
    args << parameter["destFile"].toString();

    ADBPROCESS("pullFile", args, 15000);

    QVariantMap result;
    result["command"] = "pullFile";
    if (!outstr.contains("error")) {
        result["success"] = true;
    }
    else {
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, outstr));
        result["success"] = false;
        result["content"] = outstr;
    }
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void AdbRunner::deviceList()
{
    QStringList args;
    args << "devices";

    ADBPROCESS("deviceList", args, 15000);

    QVariantMap result;
    result["success"] = true;
    result["command"] = "deviceList";
    result["content"] = outstr;
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}
