#pragma once

#include <QString>
#include <memory>

#include "countries.h"
#include "operators.h"

class QSqlDatabase;

const QString DATABASE_FILENAME = "system.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

    Countries& countries() const;
    Operators& operators() const;

protected:
    DatabaseManager(const QString &path = DATABASE_FILENAME);

private:
    std::unique_ptr<QSqlDatabase> m_Database;

    mutable Countries m_Countries;
    mutable Operators m_Operators;
};
