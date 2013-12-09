#include "fastboot.h"
#include "utils/json.h"

#include <QTime>
#include <QTemporaryFile>
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QThreadPool>

#include <QDebug>

FastBoot::FastBoot(QObject *commander)
{
    m_commander = commander;
	mTextCodec = QTextCodec::codecForLocale();
    mFastbootPath = fashbootPath();
}

QString FastBoot::fashbootPath()
{
    QDir dir = qApp->applicationDirPath();
    dir.cd("tools");
    return dir.absoluteFilePath("fastboot");
}

void FastBoot::run()
{
    switch (m_commandID) {
    case GETVAR: getvar(); break;
    case GETVARS: getvars(); break;
    case GETALLVAR: getAllVar(); break;
    case REBOOT: reboot(); break;
    case RELOCK: relock(); break;
    case RAWCOMMAND: rawCommand(); break;
    case HTCGETIDENTIFIERTOKEN: htcGetIdentifierToken(); break;
    case DEVICELIST: deviceList(); break;
    case FLASH: flash(); break;
    default: break;
    }
}

void FastBoot::setCommand(FastBoot::MethodID cmdID, const QVariant &parameter)
{
    m_commandID = cmdID;
    m_parameter = parameter;
}

void FastBoot::relock()
{
	QStringList args;
	args << "oem" << "lock";
	QString out;
    if (!execute(args, out, "", 15000)) {
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = out;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = out.contains("Lock successfully") || out.contains("already locked!");
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::getvar()
{
    QString key = m_parameter.toString();
    QStringList args;
    args << "getvar" << key;
    QString out;
    if (!execute(args, out, "", 15000) || out.contains("FAILED")) {
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = out;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QStringList sl = out.split("\n");
    foreach(QString str, sl){
            QStringList kv = str.split(":");
            QString k = kv[0].trimmed();
            QString v = kv.size() == 2 ? kv[1].trimmed() : "";
            if (key == k) {
                QVariantMap result;
                result["success"] = true;
                result["command"] = getCurrentMethodName();
                result["content"] = v;
                QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
                return;
            }
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = "";
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

QString FastBoot::getvar(const QString &key)
{
    QStringList args;
    args << "getvar" << key;
    QString out;
    if (!execute(args, out, "", 15000) || out.contains("FAILED"))
        return "";
    QStringList sl = out.split("\n");
    foreach(QString str, sl){
            QStringList kv = str.split(":");
            QString k = kv[0].trimmed();
            QString v = kv.size() == 2 ? kv[1].trimmed() : "";
            if (key == k)
                return v;
    }
    return "";
}

void FastBoot::getvars()
{
    QStringList keys = m_parameter.toStringList();
    QVariantMap vars;
    foreach(QString key, keys){
        vars[key] = getvar(key);
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = vars;
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

QVariantMap FastBoot::getvars(const QStringList &keys)
{
    QVariantMap vars;
    foreach(QString key, keys){
        vars[key] = getvar(key);
    }
    return vars;
}

void FastBoot::getAllVar()
{
    QStringList keys;
    keys << "version" << "version-bootloader" << "version-main"
         << "serialno" << "product" << "modelid" << "cidnum"
//         << "battery-status" << "battery-voltage"
         << "build-mode" << "boot-mode" << "security" << "imei";

    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = getvars(keys);
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::reboot()
{
    QString type = m_parameter.toString();
    QStringList args;
    if (type == "bootloader")
        args << "reboot-bootloader";
    else if (type == "ruu")
        args << "oem" << "rebootRUU";
    else
        args << "reboot";
    QString out;
    if (!execute(args, out, "", 15000)) {
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = out;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    if (out.contains("FAILED")) {
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = out;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = out;
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::htcGetIdentifierToken()
{
    QStringList args;
    args << "oem" << "get_identifier_token";
    QString out;
    if (!execute(args, out, "", 15000)){
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = "";
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    if (!out.contains("Identifier Token Start") || !out.contains("OKAY")){
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = "";
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QStringList sl = out.split("\n");
    QStringList sl2;
    int startPos;
    bool bStart = false;
    foreach(QString str, sl){
        if (str.contains("Identifier Token Start")){
            bStart = true;
            startPos = str.indexOf("<<");
        }
        if (bStart){
            sl2 << str.mid(startPos);
        }
        if (str.contains("Identifier Token End"))
            break;
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = sl2.join("\n");
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::deviceList()
{
    QStringList args;
    args << "devices";

    QString out;
    execute(args, out, "", 15000);

    QVariantMap result;
    result["success"] = true;
    result["command"] = "deviceList";
    result["content"] = out;
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::flash()
{
    QString out;
    if (!execute(m_parameter.toStringList(), out, "", 15000)) {
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        result["content"] = false;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QVariantMap result;
    result["success"] = true;
    result["command"] = getCurrentMethodName();
    result["content"] = out.contains("success") || out.contains("already locked");
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

void FastBoot::rawCommand()
{
    QVariantMap cmd = m_parameter.toMap();
    QString out;
    if (execute(cmd["arguments"].toStringList(), out, cmd["workdir"].toString(), cmd["timeout"].toInt())) {
        QVariantMap result;
        result["success"] = true;
        result["command"] = getCurrentMethodName();
        result["content"] = out;
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return;
    }
    QVariantMap result;
    result["success"] = false;
    result["command"] = getCurrentMethodName();
    result["content"] = "";
    QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
}

bool FastBoot::execute( const QStringList & arguments, QString & outstr, const QString & workdir /*= ""*/ , int timeout /*= -1*/)
{
    QProcess process;
    QStringList args;

    args.append(arguments);
    process.setProcessChannelMode(QProcess::MergedChannels);
    if (!workdir.isEmpty())
        process.setWorkingDirectory(workdir);

    qDebug() << "fastboot: " << args;
    process.start(mFastbootPath, args);
    if (!process.waitForStarted()){
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("Start FastBoot process failed!")));
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return false;
    }
    QByteArray outbuf;
    QTime time;
    time.start();
    while (process.state() == QProcess::Running){
        if (process.waitForReadyRead(1000)){
            outbuf += process.readAll();
            break;
        }
        if (timeout > 0 && time.elapsed() > timeout){
            qDebug() << "fastboot timeout";
            QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("FastBoot timeout!")));
            QVariantMap result;
            result["success"] = false;
            result["command"] = getCurrentMethodName();
            QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
            return false;
        }
    }
    outbuf += process.readAll();

    QProcess::ExitStatus exitStatus = process.exitStatus();
    if (exitStatus == QProcess::CrashExit){
        qDebug() << "fastboot crashed";
        QMetaObject::invokeMethod(m_commander, "message", Q_ARG(QVariant, QObject::tr("FastBoot process crashed!")));
        QVariantMap result;
        result["success"] = false;
        result["command"] = getCurrentMethodName();
        QMetaObject::invokeMethod(m_commander, "onFinished", Q_ARG(QVariant, result));
        return false;
    }
    process.waitForFinished(3000);
    outbuf += process.readAll();
    outstr = mTextCodec->toUnicode(outbuf);
    qDebug() << "fastboot out: " << outstr;

    return true;
}

QString FastBoot::getCurrentMethodName()
{
    switch (m_commandID) {
    case GETVAR: return "getvar";
    case GETVARS: return "getvars";
    case GETALLVAR: return "getAllVar";
    case REBOOT: return "reboot";
    case RELOCK: return "relock";
    case RAWCOMMAND: return "rawCommand";
    case HTCGETIDENTIFIERTOKEN: return "htcGetIdentifierToken";
    case DEVICELIST: return "deviceList";
    case FLASH: return "flash";
    default: return "";
    }
}


FastBootCommander::FastBootCommander(QObject *parent)
    : QObject(parent)
{

}

QString FastBootCommander::getvar(const QString &key)
{
    createFastBootRunner(FastBoot::GETVAR, key);
    return result.toMap()["content"].toString();
}

QVariant FastBootCommander::getvars(const QStringList &keys)
{
    createFastBootRunner(FastBoot::GETVARS, keys);
    return result.toMap()["content"];
}

QVariant FastBootCommander::getAllVar()
{
    createFastBootRunner(FastBoot::GETALLVAR);
    return result.toMap()["content"];
}

bool FastBootCommander::reboot(const QString &type)
{
    createFastBootRunner(FastBoot::REBOOT, type);
    return result.toMap()["success"].toBool();
}

bool FastBootCommander::relock()
{
    createFastBootRunner(FastBoot::RELOCK);
    return result.toMap()["success"].toBool();
}

QString FastBootCommander::rawCommand(const QStringList &arguments, const QString &workdir, int timeout)
{
    QVariantMap parameter;
    parameter["arguments"] = arguments;
    parameter["workdir"] = workdir;
    parameter["timeout"] = timeout;

    createFastBootRunner(FastBoot::RAWCOMMAND, parameter);
    return result.toMap()["content"].toString();
}

QString FastBootCommander::htcGetIdentifierToken()
{
    qDebug() << "FastBootCommander::htcGetIdentifierToken()";
    createFastBootRunner(FastBoot::HTCGETIDENTIFIERTOKEN);
    return result.toMap()["content"].toString();
}

QStringList FastBootCommander::deviceList()
{
    createFastBootRunner(FastBoot::DEVICELIST);

    QString strMsg = result.toMap()["content"].toString();
    QStringList lines = strMsg.split(QRegExp("[\\r\\n]"), QString::SkipEmptyParts);
    QStringList attachedList;
    foreach (QString line, lines) {
        if (line.contains("fastboot")) {
            QStringList lineParts = line.split(QRegExp("[\\t\\s]"), QString::SkipEmptyParts);
            if (lineParts.count() > 0)
                attachedList << lineParts[0];
        }
    }
    return attachedList;
}

bool FastBootCommander::flash(const QString &type, const QString &file)
{
    QStringList args;
    args << "flash" << type << file;
    createFastBootRunner(FastBoot::FLASH, args);
    return result.toMap()["success"].toBool() && result.toMap()["content"].toBool();
}

void FastBootCommander::onFinished(const QVariant &result)
{
    this->result = result;
    eventloop.quit();
}

void FastBootCommander::createFastBootRunner(int cmdID, const QVariant &parameter)
{
    FastBoot *fastbootRunner = new FastBoot(this);
    fastbootRunner->setCommand((FastBoot::MethodID)cmdID, parameter);
    QThreadPool::globalInstance()->start(fastbootRunner);
    eventloop.exec();
}
