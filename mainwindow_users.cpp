#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QHeaderView>

void MainWindow::userPage(){
    ui->usersTable->horizontalHeader()
    ->setSectionResizeMode(QHeaderView::Stretch);

    ui->usersTable->verticalHeader()
        ->setVisible(false);

    ui->usersTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->usersTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->usersTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    connect(ui->addUserButton,&QPushButton::clicked,this,
            &MainWindow::addUser);

    connect(ui->deleteUserButton,
            &QPushButton::clicked,
            this, &MainWindow::deleteUser);

    connect(ui->editUserButton,
            &QPushButton::clicked,
            this, &MainWindow::editUser);
}

void MainWindow::loadUsers(){
    ui->usersTable->setRowCount(0);
    QSqlQuery query;

    query.exec(
        "SELECT id, login, role "
        "FROM users "
        "ORDER BY id"
        );

    int row = 0;

    while(query.next())
    {
        ui->usersTable->insertRow(row);

        QTableWidgetItem *numberItem =
            new QTableWidgetItem(
                QString::number(row + 1));

        numberItem->setData(
            Qt::UserRole,
            query.value("id").toInt());

        ui->usersTable->setItem(row, 0,numberItem);

        ui->usersTable->setItem(
            row, 1,
            new QTableWidgetItem(
                query.value("login").toString()));

        ui->usersTable->setItem(
            row, 2,
            new QTableWidgetItem(
                query.value("role").toString()));
        row++;
    }
}

void MainWindow::addUser()
{
    QString login =
        QInputDialog::getText(this,
                              "Додати користувача",
                              "Логін:");

    if(login.isEmpty())
        return;

    QString password =
        QInputDialog::getText(this,
                              "Додати користувача",
                              "Пароль:");

    if(password.isEmpty())
        return;

    QStringList roles;
    roles << "admin" << "cashier";

    QString role =
        QInputDialog::getItem(this,
                              "Додати користувача",
                              "Роль:",
                              roles,
                              1,
                              false);

    if(role.isEmpty())
        return;

    QSqlQuery query;

    query.prepare(
        "INSERT INTO users(login, password, role) "
        "VALUES(:login, :password, :role)"
        );

    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if(!query.exec())
    {
        QMessageBox::warning(this,
                             "помилка",
                             "користувач з таким логіном вже існує!");
        return;
    }

    loadUsers();

    QMessageBox::information(this,
                             "Успіх",
                             "Користувача додано");
}
void MainWindow::editUser()
{
    int row = ui->usersTable->currentRow();

    if(row < 0)
    {
        QMessageBox::warning(
            this,
            "Помилка",
            "Виберіть користувача!"
            );

        return;
    }
    int id =ui->usersTable->item(row,0)->data(Qt::UserRole).toInt();
    QString currentLogin =ui->usersTable->item(row,1)->text();
    bool isMainAdmin = (currentLogin == "admin");
    QString password =
        QInputDialog::getText(
            this,
            "Змінити користувача",
            "Новий пароль для " + currentLogin + ":"
            );

    if(password.isEmpty())
        return;

    QStringList roles;
    roles << "admin" << "cashier";

    QString role;

    if(isMainAdmin)
    {
        role = "admin";
    }
    else
    {
        QStringList roles;
        roles << "admin" << "cashier";
        role = QInputDialog::getItem(this,"Change Role","Role:",roles,0,false);
        if(role.isEmpty())
            return;
    }

    if(role.isEmpty())
        return;

    QSqlQuery query;

    query.prepare(
        "UPDATE users "
        "SET password = :password, "
        "role = :role "
        "WHERE id = :id"
        );
    query.bindValue(":password",password);
    query.bindValue(":role",role);
    query.bindValue(":id",id);

    if(!query.exec())
    {
        QMessageBox::warning(this,"Помилка","Не вдалось оновити користувача");

        return;
    }
    loadUsers();

    QMessageBox::information(this,"Успіх","Користувача оновлено");
}

void MainWindow::deleteUser()
{
    int row = ui->usersTable->currentRow();

    if(row < 0)
    {
        QMessageBox::warning(this,"Помилка","Виберіть користувача!");
        return;
    }
    int id = ui->usersTable->item(row, 0)->data(Qt::UserRole).toInt();
    QString login = ui->usersTable->item(row, 1)->text();

    if(login == "admin")
    {
        QMessageBox::warning(this,
                             "Error",
                             "Cannot delete main administrator!");
        return;
    }
    QMessageBox msgBox;

    msgBox.setWindowTitle("Видалення");
    msgBox.setText("Видалити користувача " + login + "?");
    msgBox.setIcon(QMessageBox::Warning);

    QPushButton *yesButton =
        msgBox.addButton("так", QMessageBox::YesRole);

    msgBox.addButton("ні", QMessageBox::NoRole);
    msgBox.exec();

    if(msgBox.clickedButton() != yesButton)
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.exec())
    {
        QMessageBox::warning(this,
                             "Помилка",
                             "Не вдалось видалити користувача!");
        return;
    }

    loadUsers();
}