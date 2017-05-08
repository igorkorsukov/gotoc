TEMPLATE = app

QT += qml quick
CONFIG += c++11

GOTOC_PATH = $$PWD/../

include($$PWD/../../../qgotoc/qgotoc.pri)

DEPENDPATH += \
    $$PWD/..

PRE_TARGETDEPS += \
    $$PWD/../gotoc.a

win32: {
    LIBS += -lwinmm -lWs2_32
}

RESOURCES += \
    qml/qml.qrc

DISTFILES += \
    qml/Login.qml \
    qml/Main.qml \
    qml/Points.qml

SOURCES += \
    main.cpp
