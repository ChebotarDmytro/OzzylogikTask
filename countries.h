#pragma once

class QSqlDatabase;
class QSqlQuery;
class QString;

class Countries
{
public:
    Countries(QSqlDatabase &database);

    QSqlQuery readData() const;
    QSqlQuery readData(const QString &mcc);

private:
    QSqlDatabase &m_Database;
};
