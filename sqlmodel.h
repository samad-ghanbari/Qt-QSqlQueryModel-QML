#ifndef SQLMODEL_H
#define SQLMODEL_H
#include <QtSql>
#include <QHash>


class sqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    sqlModel(QObject *parent, QSqlDatabase &db);

    enum Roles {
        abb1 = Qt::UserRole + 1 ,
        code1 = Qt::UserRole + 2,
        abb2 = Qt::UserRole + 3,
        code2 = Qt::UserRole + 4
    };

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool refreshModel();

private:
    QHash<int, QByteArray> roles;
    QSqlDatabase &db;
    const QString queryString;
};

#endif // SQLMODEL_H
