#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTextCodec>

#include "qgotocqml.h"

int main(int argc, char *argv[])
{
    QTextCodec *textCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(textCodec);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QGoToCQml::init(&engine);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
