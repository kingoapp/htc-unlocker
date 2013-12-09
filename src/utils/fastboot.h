#ifndef FASTBOOTFLASH_H
#define FASTBOOTFLASH_H
#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QRunnable>
#include <QEventLoop>

#include "defines.h"

class FastBoot : public QRunnable
{
public:
    FastBoot(QObject *commander);

    enum MethodID {
        GETVAR,
        GETVARS,
        GETALLVAR,
        REBOOT,
        RELOCK,
        RAWCOMMAND,
        HTCGETIDENTIFIERTOKEN,
        DEVICELIST,
        FLASH
    };

    QString fashbootPath();
    void run();
    void setCommand(MethodID cmdID, const QVariant &parameter = QVariant());

public:
    void getvar();
    void getvars();
    void getAllVar();

    void reboot();
    void relock();
	
    void rawCommand();
    void htcGetIdentifierToken();
    void deviceList();
    void flash();


private:
    QString getvar(const QString &key);
    QVariantMap getvars(const QStringList &keys);
    bool execute(const QStringList & arguments, QString & outstr, const QString & workdir = "", int timeout = -1);
    QString getCurrentMethodName();

private:
    QString			mFastbootPath;
    QTextCodec*		mTextCodec;

    QObject* m_commander;
    MethodID m_commandID;
    QVariant m_parameter;
};

class FastBootCommander : public QObject
{
    Q_OBJECT
public:
    explicit FastBootCommander(QObject *parent = 0);

signals:
    void message(const QVariant &msg);
    void finished(const QVariant &result);

public slots:
    QString getvar(const QString &key);
    QVariant getvars(const QStringList &keys);
    QVariant getAllVar();

    bool reboot(const QString &type);
    bool relock();

    QString rawCommand(const QStringList &arguments, const QString &workdir = "", int timeout = -1);
    QString htcGetIdentifierToken();
    QStringList deviceList();
    bool flash(const QString &type, const QString &file);

private slots:
    void onFinished(const QVariant &result);

private:
    void createFastBootRunner(int cmdID, const QVariant &parameter = QVariant());

private:
    QEventLoop eventloop;
    QVariant result;
};

#endif // FASTBOOTFLASH_H
