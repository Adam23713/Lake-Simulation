#include "databaseinterface.h"

#ifdef WITH_SQL

DatabaseInterface* DatabaseInterface::_instance = nullptr;

DatabaseInterface::DatabaseInterface()
{

}

bool DatabaseInterface::IsOpen() const
{
    return _dataBase.isOpen();
}

bool DatabaseInterface::ConnectToTheDataBase(const QString &path)
{
    _dataBase = QSqlDatabase::addDatabase("QSQLITE");
    _dataBase.setDatabaseName(path);

    if (!_dataBase.open())
    {
        return false;
    }
    else
    {
        return true;
    }
}

DatabaseInterface* DatabaseInterface::Instance()
{
    if( _instance == nullptr )
        return new DatabaseInterface();
    return _instance;
}

bool DatabaseInterface::AddNewData(const int &time, const int &id, QString &event)
{
    bool success = false;

    if (!event.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO History (time, id, event) "
                          "VALUES (:time, :id, :event)");


        queryAdd.bindValue(":time", time);
        queryAdd.bindValue(":id", id);
        queryAdd.bindValue(":event", event);

        if(queryAdd.exec())
            success = true;
    }
    return success;


}

bool DatabaseInterface::CreateHistoryTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE History(time INTEGER, id INTEGER, event text);");

    if (!query.exec())
    {
        success = false;
    }
    return success;
}

#endif
