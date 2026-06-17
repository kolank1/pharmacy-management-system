#include "authmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthManager::AuthManager(){}

    bool AuthManager::authenticate(const QString &login, const QString &password)
{
        QSqlQuery query;

    query.prepare(
        "SELECT id FROM users "
        "WHERE login = :login AND password = :password"
        );

        query.bindValue(":login",login);
        query.bindValue(":password",password);
        if(!query.exec()){
            qDebug()<<query.lastError().text();
            return false;
        }
        return query.next();


}
QString AuthManager::getUserRole(const QString &login)
{
    QSqlQuery query;

    query.prepare(
        "SELECT role FROM users "
        "WHERE login = :login"
        );

    query.bindValue(":login", login);

    if(query.exec() && query.next())
    {
        return query.value("role").toString();
    }

    return "";
}