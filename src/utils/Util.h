#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QDesktopServices>

class Util : public QObject
{
    Q_OBJECT
public:
    explicit Util(QObject *parent = 0);

signals:

public slots:
    void saveBuffer(const QString &path, const QString &content);
    void saveBufferFromBase64(const QString &path, const QString &content);
    QString base64decode(const QString &data);
    QString base64encode(const QString &data);
    bool removeFile(const QString &filePath);
    QString storageLocation(int type);
    void log(const QString &msg);
};

#endif // UTIL_H
