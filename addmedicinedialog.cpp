#include "addmedicinedialog.h"
#include "ui_addmedicinedialog.h"
#include <QPushButton>

AddMedicineDialog::AddMedicineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMedicineDialog)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

}

QString AddMedicineDialog::getName()const{
    return ui->nameEdit->text();
}
QString AddMedicineDialog::getCategory()const{
    return ui->categoryEdit->text();
}
double AddMedicineDialog::getPrice()const{
    return ui->priceSpinBox->value();
}
int AddMedicineDialog::getQuantity()const{
    return ui->quantityspinBox->value();
}
void AddMedicineDialog::setMedicineData(const QString &name,
                                        const QString &category,
                                        const QString &manufacturer,
                                        const QString &description,
                                        double price,
                                        int quantity)
{
    ui->nameEdit->setText(name);
    ui->categoryEdit->setText(category);
    ui->manufacturerEdit->setText(manufacturer);
    ui->descriptionEdit->setPlainText(description);
    ui->priceSpinBox->setValue(price);
    ui->quantityspinBox->setValue(quantity);

}
QString AddMedicineDialog::getManufacturer() const
{
    return ui->manufacturerEdit->text();
}
QString AddMedicineDialog::getDescription() const
{
    return ui->descriptionEdit->toPlainText();
}

AddMedicineDialog::~AddMedicineDialog()
{
    delete ui;
}