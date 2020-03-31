#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString &path)
    : m_Database(std::make_unique<QSqlDatabase>(
                     QSqlDatabase::addDatabase("QSQLITE"))),
      m_Countries(*m_Database),
      m_Operators(*m_Database)
{
    m_Database->setDatabaseName(path);

    if(!m_Database->open())
    {
        qDebug() << "Can`t open the database:" << m_Database->lastError();
    }
}

DatabaseManager::~DatabaseManager()
{
    m_Database->close();
}

Countries &DatabaseManager::countries() const
{
    return m_Countries;
}

Operators &DatabaseManager::operators() const
{
    return m_Operators;
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager singleton;
    return singleton;
}
