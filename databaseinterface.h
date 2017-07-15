#undef WITH_SQL
#ifdef WITH_SQL

#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QtSql>
#include <QSqlDatabase>

//This is a Sigleton class

class DatabaseInterface
{

protected:
    DatabaseInterface();

private:
    QSqlDatabase _dataBase;
    static DatabaseInterface* _instance;

public:
    static DatabaseInterface* Instance();

    bool AddNewData(const int& time, const int& id, QString& event);
    bool CreateHistoryTable();
    bool IsOpen() const;
    bool ConnectToTheDataBase(const QString& path);

};

#endif // DATABASEINTERFACE_H

#endif //WITH_SQL
