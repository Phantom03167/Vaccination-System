QT += core gui sql
RC_FILE = logo.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    login.cpp \
    main.cpp \
    mainwidget.cpp \
    vaccinetraccodeinput.cpp

HEADERS += \
    head.h \
    login.h \
    mainwidget.h \
    vaccinetraccodeinput.h

FORMS += \
    login.ui \
    mainwidget.ui \
    vaccinetraccodeinput.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../core_common
include($$PWD/../core_common/core_common.pri)

INCLUDEPATH += $$PWD/../tableitemediter
include($$PWD/../tableitemediter/tableitemediter.pri)

RESOURCES += \
    resource.qrc
