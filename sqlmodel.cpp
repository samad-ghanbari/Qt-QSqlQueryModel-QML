#include "sqlmodel.h"

sqlModel::sqlModel(QObject *parent, QSqlDatabase &db) : QSqlQueryModel(parent),
    db(db),
    queryString("SELECT abb1, code1, abb2, code2 FROM red_folder WHERE abb1='DA' AND abb2='MA'")
{
    roles = QSqlQueryModel::roleNames();
    roles[abb1] = "abb1";
    roles[code1] = "code1";
    roles[abb2] = "abb2";
    roles[code2] = "code2";

}


bool sqlModel::refreshModel()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare(queryString);
    if(qry->exec())
    {
        this->setQuery(*qry);
        return true;
    }
    else
        return false;
}


QHash<int, QByteArray> sqlModel::roleNames() const  Q_DECL_OVERRIDE {
    return roles;
}

QVariant sqlModel::data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE {
    if (!index.isValid())
        return QVariant();

    if(role < Qt::UserRole)
    {
        return QSqlQueryModel::data(index, role);
    }
    else
    {
        QString fieldName;
        switch (role) {
        case abb1: fieldName = QStringLiteral("abb1"); break;
        case code1: fieldName = QStringLiteral("code1"); break;
        case abb2: fieldName = QStringLiteral("abb2"); break;
        case code2: fieldName = QStringLiteral("code2"); break;
        }

        if (!this->record().isGenerated(fieldName))
            return QVariant();
        else {
            QModelIndex item = indexInQuery(index);
            if ( !this->query().seek(item.row()) )
                return QVariant();


            return this->query().value(fieldName);
        }
    }
    return QVariant();
}

Qt::ItemFlags sqlModel::flags(const QModelIndex &index) const  Q_DECL_OVERRIDE
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
}

bool sqlModel::setData(const QModelIndex &index, const QVariant &value, int role)  Q_DECL_OVERRIDE
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int column = index.column();
        int row = index.row();
        QString field = headerData(column,Qt::Horizontal, Qt::DisplayRole).toString();
        qDebug() << field << data(this->index(row,0), Qt::DisplayRole).toString() << data(this->index(row, 2), Qt::DisplayRole).toString();
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare(QString("UPDATE RED_FOLDER SET %1 = :value WHERE abb1 = :abb1 AND abb2 = :abb2 and code1='original'").arg(field));
        qry->bindValue(":value", value.toString());
        qry->bindValue(":abb1", data(this->index(row,0), Qt::DisplayRole).toString());
        qry->bindValue(":abb2", data(this->index(row, 2), Qt::DisplayRole).toString());


        if(qry->exec())
        {
            qDebug() << "updated";
            refreshModel();

           //emit(dataChanged(index, index));


            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool sqlModel::insertRows(int position, int rows, const QModelIndex &index)  Q_DECL_OVERRIDE
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);


    endInsertRows();
    return true;
}

bool sqlModel::removeRows(int position, int rows, const QModelIndex &index)  Q_DECL_OVERRIDE
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);


    endInsertRows();
    return true;
}

