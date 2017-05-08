#include "qgoform.h"
#include <QVariant>
#include <QString>
#include <QDebug>

QGoForm::QGoForm(QObject *parent)
    : QObject(parent), m_key(0), m_channel(NULL), m_model(NULL)
{

}

QGoForm::~QGoForm()
{
    sendRpc(Rpc("destroy"));

    delete m_channel;
    delete m_model;
}

void QGoForm::init(int key)
{
    if (key <= 0) {
        return;
    }

    m_channel = new RpcChannel(key);
    connect(m_channel, SIGNAL(received(Rpc)), this, SLOT(onRpc(Rpc)));

    m_model = new QGoListModel(this);
    m_model->setChannel(m_channel);
    emit modelChanged();

    sendRpc(Rpc("new"));
}

void QGoForm::onRpc(const Rpc &rpc)
{
    if (rpc.method == "notify") {
        emit notify(rpc.params.value(0));
    }
}

QVariant QGoForm::sendRpc(const Rpc &rpc)
{
    return m_channel->send(rpc);
}

QVariant QGoForm::value(const QString &name)
{
    return sendRpc(Rpc("value", name));
}

void QGoForm::invoke(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    ps.append(toStringList(args));
    sendRpc(Rpc("invoke", ps));
}

void QGoForm::changed(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    ps.append(toStringList(args));
    sendRpc(Rpc("changed", ps));
}

void QGoForm::clicked(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    args.toList();
    ps.append(toStringList(args));
    sendRpc(Rpc("clicked", ps));
}

QVariantList QGoForm::toList(const QVariant &v) const
{
    if (v.canConvert(QVariant::List)) {
        return v.toList();
    } else {
        QVariantList list;
        list.append(v);
        return list;
    }
}

QStringList QGoForm::toStringList(const QVariant &v) const
{
    QStringList ret;
    QVariantList list = toList(v);
    foreach (const QVariant &obj, list) {
        ret.append(obj.toString());
    }
    return ret;
}

void QGoForm::setKey(int k)
{
    m_key = k;
    init(m_key);
}

int QGoForm::key() const
{
    return m_key;
}

QGoListModel* QGoForm::model() const
{
    return m_model;
}
