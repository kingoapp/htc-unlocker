#include "Util.h"

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDir>

#include <QDebug>

Util::Util(QObject *parent) :
    QObject(parent)
{
}

void Util::saveBuffer(const QString &path, const QString &content)
{
    QFile file(path);
    file.open(QFile::WriteOnly);
    file.write(content.toLocal8Bit());
    file.close();
}

void Util::saveBufferFromBase64(const QString &path, const QString &content)
{
    QFile file(path);
    file.open(QFile::WriteOnly);
    file.write(QByteArray::fromBase64(content.toLocal8Bit()));
    file.close();
}

QString Util::base64decode(const QString &data)
{
    return QByteArray::fromBase64(data.toLocal8Bit());
}

QString Util::base64encode(const QString &data)
{
    return data.toLocal8Bit().toBase64();
}

bool Util::removeFile(const QString &filePath)
{
    return QFile::remove(filePath);
}

QString Util::storageLocation(int type)
{
//    return QDesktopServices::storageLocation((QDesktopServices::StandardLocation)type);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return QStandardPaths::writableLocation((QStandardPaths::StandardLocation)type);
#else
    return QDesktopServices::storageLocation((QDesktopServices::StandardLocation)type);
#endif
}

void Util::log(const QString &msg)
{
    qDebug() << msg;
}
