#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDeclarativeView>

class DeviceChecker;
class MainWindow : public QDeclarativeView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();
    void connectSignalSlot();

protected:
    void focusInEvent(QFocusEvent *event);

public slots:
    void openUrl(const QString &url);
    bool isMac();

private:
    DeviceChecker *deviceChecker;
};

#endif // MAINWINDOW_H
