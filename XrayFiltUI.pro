QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    about.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    about.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    about.ui \
    initialspectrafiledialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    XrayFiltUI_ru_RU.ts
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    resources.qrc
