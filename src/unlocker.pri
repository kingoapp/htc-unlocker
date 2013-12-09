QT += webkit

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/utils/DeviceChecker.cpp \
    src/utils/AdbCommander.cpp \
    src/utils/fastboot.cpp \
    src/utils/EventLoop.cpp \
    src/utils/Util.cpp

HEADERS  += src/MainWindow.h \
    src/utils/DeviceChecker.h \
    src/utils/AdbCommander.h \
    src/defines.h \
    src/utils/fastboot.h \
    src/utils/EventLoop.h \
    src/utils/Util.h

FORMS    +=

RESOURCES += \
    src/ui.qrc
