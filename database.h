#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    Database();
    ~Database();

    bool isOpen() const;

    QString getTestTilePath();

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
