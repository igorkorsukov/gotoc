

include($$PWD/qgotoc.pri)

HEADERS += \
    $$PWD/qgoform.h \
    $$PWD/qgolistmodel.h \
    $$PWD/qgotocqml.h \
    $$PWD/rpc.h

SOURCES += \
    $$PWD/qgoform.cpp \
    $$PWD/qgolistmodel.cpp \
    $$PWD/qgotocqml.cpp \
    $$PWD/rpc.cpp

DISTFILES += \
    $$PWD/GoToC/GoForm.qml

RESOURCES += \
    $$PWD/gotoc.qrc
