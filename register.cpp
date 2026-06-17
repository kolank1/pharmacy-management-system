// #include "register.h"

// #include "authmanager.h"
// #include "ui_register.h"

// #include <QMessageBox>

// Register::Register(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::Register)
// {
//     ui->setupUi(this);

//     connect(ui->registerButton, &QPushButton::clicked, this, &Register::handleRegistration);
// }

// Register::~Register()
// {
//     delete ui;
// }

// QString Register::registeredLogin() const
// {
//     return m_registeredLogin;
// }

// void Register::handleRegistration()
// {
//     connect(ui->loginEdit,&QLineEdit::textChanged, this, [this](){
//         ui->errorLabelRegister->clear();
//     });

//     connect(ui->passwordEdit,&QLineEdit::textChanged, this, [this](){
//         ui->errorLabelRegister->clear();
//     });

//     const QString login = ui->loginEdit->text();
//     const QString password = ui->passwordEdit->text();
//     const QString confirmPassword = ui->confirmPasswordEdit->text();

//     if (password != confirmPassword) {
//         ui->errorLabelRegister->setText("паролі не співпадають");
//         return;
//     }

//     QString errorMessage;
//     if (!AuthManager::registerUser(login, password, &errorMessage)) {
//         ui->errorLabelRegister->setText(errorMessage);
//         return;
//     }

//     m_registeredLogin = login.trimmed();
//     accept();
// }