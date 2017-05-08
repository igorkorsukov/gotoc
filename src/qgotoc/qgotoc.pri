
LIBS += \
    $$GOTOC_PATH/gotoc.a

INCLUDEPATH += \
    $$PWD/. \
    $$GOTOC_PATH

HEADERS += \
    $$PWD/qgotoc.h \
    $$PWD/qgoform.h \
    $$PWD/qgolistmodel.h \
    $$PWD/qgotocqml.h \
    $$PWD/rpc.h

SOURCES += \
    $$PWD/qgotoc.cpp \
    $$PWD/qgoform.cpp \
    $$PWD/qgolistmodel.cpp \
    $$PWD/qgotocqml.cpp \
    $$PWD/rpc.cpp

DISTFILES += \
    $$PWD/GoToC/GoForm.qml

RESOURCES += \
    $$PWD/gotoc.qrc
