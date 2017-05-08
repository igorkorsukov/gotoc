#ifndef QGOLISTMODEL_H
#define QGOLISTMODEL_H

#include <QtQml>
#include <QAbstractListModel>

#include "rpc.h"

class QGoListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QGoListModel(QObject *parent = 0);
    ~QGoListModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    virtual QHash<int,QByteArray> roleNames() const;

    void setChannel(RpcChannel *ch);

private slots:
    void onRpc(const Rpc &rpc);

private:

    enum Roles {
        rItem = Qt::UserRole + 1,
        rGroup
    };

    QHash<int, QByteArray> m_roles;
    RpcChannel* m_channel;
};

QML_DECLARE_TYPE(QGoListModel)

#endif // QGOLISTMODEL_H
