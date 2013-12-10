#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QTextCodec>
#include <QSslConfiguration>
#include <QSslCipher>
#if defined(Q_OS_WIN)
#include <Windows.h>
#endif

#if defined(Q_OS_WIN)
HANDLE		g_hMutex = NULL;
#pragma data_seg("Unlocker_Windows_Handle")
HWND		 g_hMainWinHandle = NULL;
#pragma data_seg()
#pragma comment(linker,"/SECTION:Unlocker_Windows_Handle,RWS")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_WIN)
    if (g_hMainWinHandle){
        SendMessage(g_hMainWinHandle, WM_SHOWWINDOW, FALSE, NULL);
        return 0;
    }

    g_hMutex = CreateMutex(NULL, FALSE, L"unlocker_oneinstance");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(g_hMutex);
        g_hMutex = NULL;
        return 0;
    }
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSslConfiguration sslconfig;
    sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslconfig.setProtocol(QSsl::AnyProtocol);
    sslconfig.setCiphers(QSslSocket::supportedCiphers());
    QSslConfiguration::setDefaultConfiguration(sslconfig);


    QSettings settings("Resources", QSettings::IniFormat);
    QCoreApplication::setOrganizationName("Kingo");
    QCoreApplication::setOrganizationDomain(settings.value("Update/Pid", "com.kingoapp.unlocker.htc").toString());
    QCoreApplication::setApplicationName("One-Click Unlocker");
    QCoreApplication::setApplicationVersion(settings.value("Application/Version", "0.1").toString());


    MainWindow w;
    w.show();
#if defined(Q_OS_WIN)
    g_hMainWinHandle = w.winId();

    int ret = a.exec();
    CloseHandle(g_hMutex);
    g_hMainWinHandle = NULL;

    return ret;
#else
    return a.exec();
#endif
}
