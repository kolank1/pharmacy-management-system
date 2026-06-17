#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>

class AuthManager
{
public:
    AuthManager();
    static bool authenticate(const QString &login, const QString &password);

    static QString getUserRole(const QString &login);
};
#endif // AUTHMANAGER_H
