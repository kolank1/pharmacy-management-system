#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
DatabaseManager::DatabaseManager() {}


bool DatabaseManager::connectDatabase(){
    db=QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("pharmacy.db");
    if(db.open()){
        qDebug()<<"база даних підключена";
        return true;

    }
    else{
        qDebug()<<"помилка підключення";
        qDebug()<<db.lastError().text();
        return false;
    }

}
//створення таблиці
bool DatabaseManager::createTables(){
    QSqlQuery query;

    QString createMedicinesTable =
        "CREATE TABLE IF NOT EXISTS medicines ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "category TEXT, "
        "manufacturer TEXT, "
        "description TEXT, "
        "price REAL NOT NULL, "
        "quantity INTEGER NOT NULL "
        ");";

    QString createSalesTable =
        "CREATE TABLE IF NOT EXISTS sales ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "sale_date TEXT NOT NULL, "
        "total_price REAL NOT NULL, "
        "seller TEXT NOT NULL "
        ");";

    QString createSaleItemsTable =
        "CREATE TABLE IF NOT EXISTS sale_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "sale_id INTEGER NOT NULL, "
        "medicine_id INTEGER NOT NULL, "
        "medicine_name TEXT NOT NULL, "
        "quantity INTEGER NOT NULL, "
        "price REAL NOT NULL, "
        "sum REAL NOT NULL "
        ");";

    QString createUsersTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "login TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL, "
        "role TEXT NOT NULL "
        ");";

    if(!query.exec(createMedicinesTable))
    {
        qDebug() << "error creating medicines";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec(createSalesTable))
    {
        qDebug() << "error creating sales";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec(createSaleItemsTable))
    {
        qDebug() << "error creating sale_items";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec(createUsersTable))
    {
        qDebug() << "error creating users";
        qDebug() << query.lastError().text();
        return false;
    }

    query.prepare(
        "INSERT OR IGNORE INTO users(login, password, role) "
        "VALUES('admin', '1234', 'admin')"
        );

    if(!query.exec())
    {
        qDebug() << "error creating default admin";
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}
bool DatabaseManager::addMedicine(const QString &name,
                                  const QString &category,
                                    const QString &manufacturer,
                                    const QString &description,
                                   double price,int quantity ){
    QSqlQuery query;

    query.prepare("INSERT INTO medicines(name, category, manufacturer, description, price, quantity)"
                  "VALUES(:name, :category, :manufacturer, :description, :price, :quantity)");
    query.bindValue(":name", name);
    query.bindValue(":manufacturer",manufacturer);
    query.bindValue(":category",category);
    query.bindValue(":description", description);
    query.bindValue(":price",price);
    query.bindValue(":quantity",quantity);

    if(query.exec()){
        qDebug()<<"Ліки додано";
        return true;
    }
    else{
        qDebug() << "error adding medication";
        qDebug() << query.lastError().text();


        return false;
    }
}

bool DatabaseManager::deleteMedicine(int id){
    QSqlQuery query;
    query.prepare("DELETE FROM medicines WHERE id = :id");
    query.bindValue(":id",id);
        if(query.exec()){
        qDebug()<<"ліки видалено";
        return true;
    }
    else
    {
        qDebug() << "Помилка видалення";
        qDebug() << query.lastError().text();
        return false;
    }
}
bool DatabaseManager::updateMedicine(int id,
                                     const QString &name,
                                     const QString &category,
                                     const QString &manufacturer,
                                     const QString &description,
                                     double price,
                                     int quantity)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE medicines SET "
        "name = :name, "
        "category = :category, "
        "manufacturer = :manufacturer, "
        "description = :description, "
        "price = :price, "
        "quantity = :quantity "
        "WHERE id = :id"
        );


    query.bindValue(":name", name);
    query.bindValue(":category", category);
    query.bindValue(":manufacturer", manufacturer);
    query.bindValue(":description", description);
    query.bindValue(":price", price);
    query.bindValue(":quantity", quantity);
    query.bindValue(":id", id);
    if(query.exec())
    {
        qDebug() << "Medication updated";
        return true;
    }
    else
    {
        qDebug() << query.lastError().text();
        return false;
    }
}