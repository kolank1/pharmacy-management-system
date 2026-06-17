#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "authmanager.h"
#include <QTableWidgetItem>
#include "databasemanager.h"
#include <QInputDialog>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QSpinBox>

#include "stylemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //підключаю базу даних
    DatabaseManager db;
    if(db.connectDatabase()){
        db.createTables();
    }
    // запускаєм логін
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    showMaximized();
    //для сховування паролю
    ui->passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    CreateBatton();
    setupLogin();
    mainPage();
    loadMedicines();
    styleTable();
    deleteMed();
    updateMedicine();
    searchMedicines();
    // продажі
    selasPage();
    loadSelasMedicines();
    CardTotalSum();
    cleanCard();
    searchSelas();

    //звіти
    reportsPage();
    loadReporsMouth();

    //користувачі
    userPage();
    loadUsers();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // для того щоб вікно було по середині
    QMainWindow::resizeEvent(event);
    ui->meinFrame->move(
        (width() - ui->meinFrame->width()) / 2,
        (height() - ui->meinFrame->height()) / 2
        );

}

void MainWindow::setupLogin(){
    connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
        const QString loginText = ui->loginEdit->text();
        const QString pass = ui->passwordEdit->text();

        if(loginText.isEmpty() || pass.isEmpty()){
            ui->errorLabel->setText("заповніть пусті поля");
            return;
        }

        if (AuthManager::authenticate(loginText, pass)) {
            currentUserLogin = loginText;
            currentRole = AuthManager::getUserRole(loginText);


            if(currentRole == "admin")
            {
                ui->usersButton->show();
            }
            else
            {
                ui->usersButton->hide();
                ui->deleteMedicineButton->setEnabled(false);
            }


            ui->stackedWidget->setCurrentWidget(ui->mainPage);
        }
        else
        {
            ui->errorLabel->setText("неправельний логін або пароль");
        }

    });


}
void MainWindow::mainPage(){
QList<QPushButton*> menuButtons;
    menuButtons =
        {
            ui->medicinesButton,
            ui->salesButton,
            ui->usersButton,
            ui->reportsButton
        };

// перехід по сторінкам
    connect(ui->medicinesButton,&QPushButton::clicked,this , [=](){
        ui->stackedWidget_2->setCurrentWidget(ui->medicinesPage);

        StyleManager::setActiveButton(
            ui->medicinesButton,
            menuButtons
            );
    });

    connect(ui->salesButton,&QPushButton::clicked,this , [=](){
        ui->stackedWidget_2->setCurrentWidget(ui->salesPage);

        StyleManager::setActiveButton(
            ui->salesButton,
            menuButtons
            );
    });
    connect(ui->reportsButton,&QPushButton::clicked,this , [=](){
        ui->stackedWidget_2->setCurrentWidget(ui->reportsPage);

        StyleManager::setActiveButton(
            ui->reportsButton,
            menuButtons
            );
    });

    connect(ui->usersButton, &QPushButton::clicked, this, [=]() {

        if(currentRole != "admin")
        {
            QMessageBox::warning(this,
                                 "Доступ заборонено",
                                 "Ця сторінка доступна тільки адміністратору.");
            return;
        }

        ui->stackedWidget_2->setCurrentWidget(ui->usersPage);

        StyleManager::setActiveButton(
            ui->usersButton,
            menuButtons
            );
    });

    connect(ui->logoutButton, &QPushButton::clicked, this, [this]() {

        currentUserLogin.clear();
        currentRole.clear();

        ui->loginEdit->clear();
        ui->passwordEdit->clear();
        ui->errorLabel->clear();

        ui->usersButton->hide();

        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    });
    //для кнопок в меню

}


MainWindow::~MainWindow()
{
    delete ui;

}

