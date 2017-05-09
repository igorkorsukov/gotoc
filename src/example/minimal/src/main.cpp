#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "qgotoc.h"

void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    static QTextStream ts(stdout);
    ts << msg << "\n";
    ts.flush();
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageHandler);
    QCoreApplication a(argc, argv);

    QGoToC *g = QGoToC::instance();

    g->echo();

    qDebug() << "\nFrom go ----------------";
    qDebug() << "int from go:" << g->goCall(101, QString::number(42).toLatin1()).toInt();
    qDebug() << "string from go:" << QString(g->goCall(102, QByteArray("hello from qt")));
    qDebug() << "slice from go:" << QString(g->goCall(103)).split(",");

    QJsonObject obj = QJsonDocument::fromJson(g->goCall(104)).object();
    qDebug() << "struct from go:" << obj.value("id").toInt() << obj.value("name").toString();



    qDebug() << "\nFrom qt -----------------";
    struct IntFn: public QGoToC::CFn {
        QByteArray call(const QByteArray &in_ba) {
            return QString::number(42 + in_ba.toInt()).toLatin1();
        }
    };
    IntFn intFn;
    g->reg(201, &intFn);

    struct StringFn: public QGoToC::CFn {
        QByteArray call(const QByteArray &in_ba) {
            return QString("%1 -> hello from qt").arg(QString(in_ba)).toUtf8();
        }
    };
    StringFn stringFn;
    g->reg(202, &stringFn);

    struct SliceFn: public QGoToC::CFn {
        QByteArray call(const QByteArray &in_ba) {
            return QStringList({"bob", "tod", "alice", "anna"}).join(',').toLatin1();
        }
    };
    SliceFn sliceFn;
    g->reg(203, &sliceFn);

    struct StructFn: public QGoToC::CFn {
        QByteArray call(const QByteArray &in_ba) {
            QJsonObject obj;
            obj["id"] = 22;
            obj["name"] = "Alice";
            return QJsonDocument(obj).toJson(QJsonDocument::Compact);
        }
    };
    StructFn structFn;
    g->reg(204, &structFn);

    g->goCall(200, QString::number(201).toLatin1());
    g->goCall(200, QString::number(202).toLatin1());
    g->goCall(200, QString::number(203).toLatin1());
    g->goCall(200, QString::number(204).toLatin1());

    qDebug() << "\n-------------------------";


    g->benchGoAll();




    return a.exec();
}
