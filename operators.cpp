#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QModelIndex>
#include <QDebug>

#include "operators.h"
#include "customitemmodel.h"

Operators::Operators(QSqlDatabase &database)
    : m_Database(database)
{

}

QSqlQuery Operators::readData(int mcc)
{
    QSqlQuery query(m_Database);
    if(!query.exec(QString("SELECT * FROM operators"
                           " WHERE Mcc='%1';").arg(mcc)))
    {
        qDebug() << Q_FUNC_INFO << "Unable to execute query";
    }

    return query;
}

void Operators::updateData(const QModelIndex &index)
{
    QSqlQuery query(m_Database);
    auto command = QString("UPDATE operators "
                           "SET Name = \'%1\' "
                           "WHERE Mcc = \'%2\' "
                           "AND Mnc = \'%3\';")
                   .arg(index.data(CustomItemModel::Name).toString())
                   .arg(index.data(CustomItemModel::Mcc).toString())
                   .arg(index.data(CustomItemModel::Mnc).toString());

    if(!query.exec(command))
    {
        qDebug() << Q_FUNC_INFO << "Unable to make edit operation";
    }
}

void Operators::insertData(std::initializer_list<QString> data)
{
    QSqlQuery query(m_Database);
    query.prepare("INSERT INTO operators (Mcc, Mnc, Name)"
                  "VALUES(?, ?, ?);");

    for(const auto& info : data)
    {
        query.addBindValue(info);
    }

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Unable to make insert operation";
    }
}
