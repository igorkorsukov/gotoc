#ifndef QGOFORM_H
#define QGOFORM_H

#include <QObject>
#include <QJSValue>
#include <QJsonObject>
#include <QVariant>

#include "rpc.h"
#include "qgolistmodel.h"

class QGoForm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int key READ key WRITE setKey)
    Q_PROPERTY(QGoListModel* model READ model NOTIFY modelChanged)

public:
    explicit QGoForm(QObject *parent = 0);
    ~QGoForm();

    int key() const;
    QGoListModel* model() const;

    Q_INVOKABLE QVariant value(const QString &name);
    Q_INVOKABLE void invoke(const QString &name, const QVariant &args = QVariant());
    Q_INVOKABLE void changed(const QString &name, const QVariant &args = QVariant());
    Q_INVOKABLE void clicked(const QString &name, const QVariant &args = QVariant());

signals:
    void notify(const QString &name);

    void modelChanged();

public slots:
    void setKey(int k);

private slots:
    void onRpc(const Rpc &rpc);

private:

    void init(int key);

    QVariant sendRpc(const Rpc &rpc);

    QVariantList toList(const QVariant &v) const;
    QStringList toStringList(const QVariant &v) const;

    int m_key;
    RpcChannel* m_channel;
    QGoListModel* m_model;
};

QML_DECLARE_TYPE(QGoForm)

#endif // QGOFORM_H
