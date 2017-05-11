#ifndef QGOLISTMODEL_H
#define QGOLISTMODEL_H

#include <QtQml>
#include <QAbstractListModel>

#include "rpc.h"
#include "qgoform.h"

class QGoListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QGoForm* form READ form WRITE setForm NOTIFY formChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit QGoListModel(QObject *parent = 0);
    ~QGoListModel();

    QGoForm* form() const;
    QString name() const;

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    virtual QHash<int,QByteArray> roleNames() const;


public slots:
    void setForm(QGoForm* f);
    void setName(QString name);

signals:
    void formChanged(QGoForm* form);
    void nameChanged(QString name);

private slots:
    void onFormInited(bool arg);
    void onRpc(const Rpc &rpc);

private:

    enum Roles {
        rItem = Qt::UserRole + 1,
        rGroup
    };

    QVariant send(const QString &method, const QString arg) const;
    QVariant send(const QString &method, const QStringList &args = QStringList()) const;

    QHash<int, QByteArray> m_roles;
    mutable QGoForm* m_form;
    QString m_name;
};

QML_DECLARE_TYPE(QGoListModel)

#endif // QGOLISTMODEL_H
