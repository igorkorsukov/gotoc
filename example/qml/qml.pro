TEMPLATE = app

QT += qml quick
CONFIG += c++11

BIN_PATH = $$PWD/build/bin
OBJ_PATH = $$PWD/build/obj
GOTOC_PATH = $$PWD

include($$PWD/../../qgotoc/qgotocqml.pri)

DESTDIR = $$BIN_PATH
OBJECTS_DIR = $$OBJ_PATH
MOC_DIR = $$OBJECTS_DIR
OBJMOC = $$OBJECTS_DIR
RCC_DIR = $$OBJECTS_DIR

DEPENDPATH += \
    $$GOTOC_PATH

PRE_TARGETDEPS += \
    $$GOTOC_PATH/gotoc.a

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
