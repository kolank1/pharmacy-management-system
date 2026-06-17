// #include "login.h"

// #include "authmanager.h"
// #include "register.h"
// #include "ui_login.h"

// #include <QMessageBox>

// login::login(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::login)
// {
//     ui->setupUi(this);

//     connect(ui->loginEdit,&QLineEdit::textChanged, this, [this](){
//         ui->errorLabel->clear();
//     });

//     connect(ui->passwordEdit,&QLineEdit::textChanged, this, [this](){
//         ui->errorLabel->clear();
//     });

//     connect(ui->loginButton, &QPushButton::clicked, this, [this]() {


//         const QString loginText = ui->loginEdit->text();
//         const QString pass = ui->passwordEdit->text();

//         if(loginText.isEmpty() || pass.isEmpty()){
//             ui->errorLabel->setText("заповніть пусті поля");
//             return;
//         }

//         if (AuthManager::authenticate(loginText, pass)) {
//          accept();
//         }

//         else {
//             ui->errorLabel->setText("неправельний пароль");
//         }
//     });

//     connect(ui->registerButton, &QPushButton::clicked, this, &login::openRegisterDialog);
// }

// login::~login()
// {
//     delete ui;
// }

// void login::openRegisterDialog()
// {
//     Register registerDialog(nullptr);
//     registerDialog.setWindowState(Qt::WindowMaximized);
//     if (registerDialog.exec() == QDialog::Accepted) {
//         ui->loginEdit->setText(registerDialog.registeredLogin());
//         ui->passwordEdit->clear();
//         QMessageBox::information(this, "Реєстрація", "Користувача створено. Тепер увійдіть під новими даними.");
//     }
// }
