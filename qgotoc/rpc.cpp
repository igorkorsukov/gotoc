#include "rpc.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "qgotoc.h"

QJsonObject Rpc::toObj() const
{
    QJsonObject obj;
    obj["method"] = method;
    QJsonArray arr;
    foreach (const QString &p, params) {
        arr.append(QJsonValue(p));
    }
    obj["params"] = arr;
    return obj;
}

Rpc Rpc::fromObj(const QJsonObject &obj)
{
    Rpc rpc;
    rpc.method = obj.value("method").toString();
    QJsonArray arr = obj.value("params").toArray();
    foreach (const QJsonValue &val, arr) {
        rpc.params << val.toString();
    }
    return rpc;
}

struct OnGoCallFn : public QGoToC::CFn {
    RpcChannel* ch;
    OnGoCallFn(RpcChannel *c) : ch(c) {}
    virtual ~OnGoCallFn() {}
    QByteArray call(const QByteArray &in_ba) {
        return ch->onGoCall(in_ba);
    }
};

RpcChannel::RpcChannel(int key)
    : m_key(key), m_transport(QGoToC::instance())
{
    m_onGoCall = new OnGoCallFn(this);
    QGoToC::Err err = m_transport->reg(m_key, m_onGoCall);
    if (err) {
        qDebug() << Q_FUNC_INFO << "err:" << err.text;
    }
}

RpcChannel::~RpcChannel()
{
    QGoToC::instance()->unreg(m_key);
    delete m_onGoCall;
}

QVariant RpcChannel::send(const Rpc &rpc)
{
    return toVar(sendRaw(QJsonDocument(rpc.toObj()).toJson(QJsonDocument::Compact)));
}

QVariant RpcChannel::toVar(const QByteArray &data) const
{
    if (data.isEmpty()) {
        return QVariant();
    }

    if (data.startsWith('{') || data.startsWith('[')) {
        QJsonParseError err;
        QVariant val = QJsonDocument::fromJson(data, &err).toVariant();
        if (err.error != QJsonParseError::NoError) {
            qDebug() << Q_FUNC_INFO << "ret data:" << data << ", val:" << val << ", err:" << err.errorString();
        }
        return val;
    }

    if (data.startsWith('"') && data.endsWith('"')) {
        return QString(data.mid(1, data.size() - 2));
    }

    return data.toInt();
}

QByteArray RpcChannel::sendRaw(const QByteArray &in)
{
    qDebug() << Q_FUNC_INFO << "in:" << in;
    QByteArray out = m_transport->goCall(m_key, in);
    qDebug() << Q_FUNC_INFO << "out:" << out;
    return out;
}

QByteArray RpcChannel::onGoCall(const QByteArray &in)
{
    qDebug() << Q_FUNC_INFO << "in:" << in;
    QJsonDocument json = QJsonDocument::fromJson(in);
    Rpc rpc = Rpc::fromObj(json.object());

    emit received(rpc);

    return QByteArray();
}
