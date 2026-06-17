#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    DatabaseManager();
    bool connectDatabase();
    bool createTables();
    bool addMedicine(const QString &name,const QString &category,const QString &manufacturer,
                     const QString &description,
                     double price,int quantity );
    bool deleteMedicine(int id);
    bool updateMedicine(int id,
    const QString &name,const QString &category,
                        const QString &manufacturer,
                        const QString &description,
    double price,int quantity);

private:
    QSqlDatabase db;

};

#endif // DATABASEMANAGER_H
