#ifndef QGOTOCQML_H
#define QGOTOCQML_H

#include <QObject>
#include <QQmlEngine>

class QGoToCQml : public QObject
{
    Q_OBJECT
public:

    static QGoToCQml* instance() {
        static QGoToCQml i;
        return &i;
    }

    static void init(QQmlEngine *engine);

    static void registerObjects(QQmlEngine* engine);
    static void registerTypes(const char *uri);

private:
    explicit QGoToCQml(QObject *parent = 0);
};

#endif // QGOTOCQML_H
