#ifndef QGOFORM_H
#define QGOFORM_H

#include <QtQml>
#include <QObject>
#include <QVariant>

#include "rpc.h"

class QGoForm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int key READ key WRITE setKey)
    Q_PROPERTY(bool inited READ inited NOTIFY initedChanged)

public:
    explicit QGoForm(QObject *parent = 0);
    ~QGoForm();

    int key() const;
    bool inited() const;

    Q_INVOKABLE QVariant value(const QString &name);
    Q_INVOKABLE void invoke(const QString &name, const QVariant &args = QVariant());
    Q_INVOKABLE void changed(const QString &name, const QVariant &args = QVariant());
    Q_INVOKABLE void clicked(const QString &name, const QVariant &args = QVariant());

signals:
    void initedChanged(bool arg);
    void notify(const QString &name);

public slots:
    void setKey(int k);

private slots:
    void onRpc(const Rpc &rpc);

private:

    friend class QGoListModel;

    void init(int key);

    QVariant send(const Rpc &rpc);

    QVariantList toList(const QVariant &v) const;
    QStringList toStringList(const QVariant &v) const;

    int m_key;
    bool m_inited;
    RpcChannel* m_channel;

};

QML_DECLARE_TYPE(QGoForm)

#endif // QGOFORM_H
