#ifndef RPC_H
#define RPC_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>

struct Rpc {
    QString method;
    QStringList params;
    Rpc() {}
    Rpc(const QString &m, const QString &p = QString()) : method(m), params(p) {}
    Rpc(const QString &m, const QStringList &ps) : method(m), params(ps) {}

    QJsonObject toObj() const;
    static Rpc fromObj(const QJsonObject &obj);
};

class QGoToC;
class OnGoCallFn;
class RpcChannel: public QObject
{
    Q_OBJECT
public:
    RpcChannel(int key);
    ~RpcChannel();

    QVariant send(const Rpc &rpc);
    QByteArray sendRaw(const QByteArray &in);

signals:
    void received(const Rpc &rpc);

private:

    QVariant toVar(const QByteArray &data) const;

    friend class OnGoCallFn;
    QByteArray onGoCall(const QByteArray &in);

    int m_key;
    QGoToC* m_transport;
    OnGoCallFn* m_onGoCall;
};


#endif // RPC_H
