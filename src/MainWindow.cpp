#include "MainWindow.h"

#include "DeviceChecker.h"
#include "AdbCommander.h"
#include "fastboot.h"
#include "Util.h"
#include "EventLoop.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QApplication>
#include <QTimer>
#include <qdeclarative.h>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    init();
    connectSignalSlot();
}

MainWindow::~MainWindow()
{
    deviceChecker->setQuitFlag(true);
    deviceChecker->wait();
}

void MainWindow::init()
{
    setWindowTitle(tr("HTC Bootloader Unlock"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setStyleSheet("background-color:transparent");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setResizeMode(SizeViewToRootObject);
    deviceChecker = new DeviceChecker(this);

    rootContext()->setContextProperty("fastboot", new FastBootCommander(this));
    rootContext()->setContextProperty("adb", new AdbCommander(this));
    rootContext()->setContextProperty("eventloop", new EventLoop(this));
    rootContext()->setContextProperty("util", new Util(this));
    rootContext()->setContextProperty("deviceChecker", deviceChecker);
    rootContext()->setContextProperty("qApp", qApp);
    rootContext()->setContextProperty("window", this);

    setSource(QUrl("qrc:/ui/MainWindow.qml"));

    QTimer::singleShot(1000, deviceChecker, SLOT(start()));
}

void MainWindow::connectSignalSlot()
{
}

void MainWindow::focusInEvent(QFocusEvent *event)
{
    QDeclarativeView::focusInEvent(event);
    repaint();
}

void MainWindow::openUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

bool MainWindow::isMac()
{
#ifdef Q_OS_MAC
    return true;
#else
    return false;
#endif
}


