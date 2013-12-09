#ifndef ADBCOMMANDER_H
#define ADBCOMMANDER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRunnable>
#include <QStringList>
#include <QEventLoop>

class AdbRunner : public QRunnable
{
public:
    AdbRunner(QObject *commander);

    enum MethodID {
        REBOOT,
        SHELL,
        PUSHFILE,
        PULLFILE,
        DEVICELIST,
    };

    QString adbPath();
    void run();
    void setCommand(MethodID cmdID, const QVariant &parameter = QVariant());

public:
    void reboot();
    void shell();
    void pushFile();
    void pullFile();
    void deviceList();

private:
    QObject *m_commander;
    MethodID m_commandID;
    QVariant m_parameter;
};

class AdbCommander : public QObject
{
    Q_OBJECT
public:
    explicit AdbCommander(QObject *parent = 0);

signals:
    void message(const QVariant &msg);
    void finished(const QVariant &result);

public slots:
    bool reboot(const QString &mode);
    QString shell(const QString &cmd);
    bool pushFile(const QString & srcFile, const QString &destFile);
    bool pullFile(const QString & phoneFile, const QString &destFile);
    QStringList deviceList();

private slots:
    void onFinished(const QVariant &result);

private:
    void createAdbRunner(int cmdID, const QVariant &parameter = QVariant());

private:
    QEventLoop eventloop;
    QVariant result;
};

#endif // ADBCOMMANDER_H
