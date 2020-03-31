#pragma once

#include <initializer_list>

class QSqlDatabase;
class QSqlQuery;
class QString;
class QModelIndex;

class Operators
{
public:
    Operators(QSqlDatabase &database);

    QSqlQuery readData(int mcc);
    void updateData(const QModelIndex &index);
    void insertData(std::initializer_list<QString> data);

private:
    QSqlDatabase &m_Database;
};
