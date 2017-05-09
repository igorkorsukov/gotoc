#include "qgolistmodel.h"
#include <QDebug>

QGoListModel::QGoListModel(QObject *parent)
    : QAbstractListModel(parent), m_form(0)
{
    m_roles.insert(rItem, "item");
    m_roles.insert(rGroup, "group");
}

QGoListModel::~QGoListModel()
{

}

void QGoListModel::onFormInited(bool arg)
{
    if (arg && m_form) {
        connect(m_form->m_channel, SIGNAL(received(Rpc)), this, SLOT(onRpc(Rpc)));
    }
}

QVariant QGoListModel::send(const QString &method, const QString arg) const
{
    return send(method, QStringList(arg));
}

QVariant QGoListModel::send(const QString &method, const QStringList &args) const
{
    if (!(m_form && m_form->inited())) {
        return QVariant();
    }

    Rpc rpc(method);
     qDebug() << Q_FUNC_INFO << "1" << rpc.method << rpc.params;
    rpc.params.append(m_name);
    rpc.params.append(args);
    qDebug() << Q_FUNC_INFO << "2" << rpc.method << rpc.params;
    return m_form->send(rpc);
}

void QGoListModel::onRpc(const Rpc &rpc)
{
    if (rpc.params.value(0) != m_name) {
        return; //! NOTE Не для этой модели
    }

    if (rpc.method == "rowchanged") {
        int row = rpc.params.value(1).toInt();
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

    return send("rowdata", QString::number(index.row()));
}

int QGoListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return send("rowcount").toInt();
}

void QGoListModel::setForm(QGoForm* f)
{
    if (m_form == f)
        return;

    m_form = f;
    emit formChanged(f);

    if (m_form) {
        if (m_form->inited()) {
            onFormInited(true);
        } else {
            connect(m_form, SIGNAL(initedChanged(bool)), this, SLOT(onFormInited(bool)));
        }
    }
}

QGoForm* QGoListModel::form() const
{
    return m_form;
}

void QGoListModel::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

QString QGoListModel::name() const
{
    return m_name;
}
