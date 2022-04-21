QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    readfromcamera.cpp \
    send.cpp

HEADERS += \
    mainwindow.h \
    readfromcamera.h \
    send.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#将输出文件直接放到源码目录下的bin目录下，将dll都放在了次目录中，用以解决运行后找不到dll的问#DESTDIR=$$PWD/bin/
contains(QT_ARCH, i386) {
message("32-bit")
DESTDIR = $${PWD}/bin32
} else {
message("64-bit")
DESTDIR = $${PWD}/bin64
}

SOURCES +=
src/main.cpp

win32{

contains(QT_ARCH, i386) {
message("32-bit")
INCLUDEPATH += $$PWD/lib/win32/ffmpeg/include
$$PWD/src

LIBS += -L$$PWD/lib/win32/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

} else {
message("64-bit")
INCLUDEPATH += $$PWD/lib/win64/ffmpeg/include
$$PWD/src

LIBS += -L$$PWD/lib/win64/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
}

}
