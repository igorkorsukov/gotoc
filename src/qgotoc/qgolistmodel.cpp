#include "qgolistmodel.h"
#include <QDebug>

QGoListModel::QGoListModel(QObject *parent)
    : QAbstractListModel(parent), m_channel(0)
{
    m_roles.insert(rItem, "item");
    m_roles.insert(rGroup, "group");
}

QGoListModel::~QGoListModel()
{

}

void QGoListModel::onRpc(const Rpc &rpc)
{
    qDebug() << Q_FUNC_INFO << rpc.method;
    if (rpc.method == "rowchanged") {
        int row = rpc.params.value(0).toInt();
        QModelIndex index = this->index(row);
        emit dataChanged(index, index);
    }
}

QHash<int,QByteArray> QGoListModel::roleNames() const
{
    return m_roles;
}

QVariant QGoListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return m_channel->send(Rpc("rowdata", QString::number(index.row())));
}

int QGoListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_channel->send(Rpc("rowcount")).toInt();
}

void QGoListModel::setChannel(RpcChannel *ch)
{
    m_channel = ch;
    connect(m_channel, SIGNAL(received(Rpc)), this, SLOT(onRpc(Rpc)));
}
