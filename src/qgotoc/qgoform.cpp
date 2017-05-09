#include "qgoform.h"
#include <QVariant>
#include <QString>
#include <QDebug>

QGoForm::QGoForm(QObject *parent)
    : QObject(parent), m_key(0), m_inited(false), m_channel(NULL)
{

}

QGoForm::~QGoForm()
{
    send(Rpc("destroy"));

    delete m_channel;
}

void QGoForm::init(int key)
{
    if (key <= 0) {
        return;
    }

    m_channel = new RpcChannel(key);
    connect(m_channel, SIGNAL(received(Rpc)), this, SLOT(onRpc(Rpc)));

    send(Rpc("new"));

    m_inited = true;
    emit initedChanged(true);
}

void QGoForm::onRpc(const Rpc &rpc)
{
    if (rpc.method == "notify") {
        emit notify(rpc.params.value(0));
    }
}

QVariant QGoForm::send(const Rpc &rpc)
{
    return m_channel->send(rpc);
}

QVariant QGoForm::value(const QString &name)
{
    return send(Rpc("value", name));
}

void QGoForm::invoke(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    ps.append(toStringList(args));
    send(Rpc("invoke", ps));
}

void QGoForm::changed(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    ps.append(toStringList(args));
    send(Rpc("changed", ps));
}

void QGoForm::clicked(const QString &name, const QVariant &args)
{
    QStringList ps(name);
    args.toList();
    ps.append(toStringList(args));
    send(Rpc("clicked", ps));
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

bool QGoForm::inited() const
{
    return m_inited;
}
