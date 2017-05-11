#include "qgotocqml.h"
#include <QQmlContext>
#include <QtQml>

#include "qgoform.h"
#include "qgolistmodel.h"

QGoToCQml::QGoToCQml(QObject *parent)
    : QObject(parent)
{

}

void QGoToCQml::init(QQmlEngine *engine)
{
    Q_INIT_RESOURCE(gotoc);

    registerObjects(engine);

    registerTypes("GoToC");
}

void QGoToCQml::registerObjects(QQmlEngine* engine)
{
    Q_UNUSED(engine);
}

void QGoToCQml::registerTypes(const char *uri)
{
    qmlRegisterType<QGoForm>(uri, 1, 0, "QGoForm");
    qmlRegisterType<QGoListModel>(uri, 1, 0, "QGoListModel");
}



