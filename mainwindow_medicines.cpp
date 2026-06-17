#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "databasemanager.h"
#include "addmedicinedialog.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>


void MainWindow::loadMedicines()
{
    // підключення деталів
    connect(ui->medicinesTable, &QTableWidget::cellClicked,
            this, [this](int row, int column) {
                Q_UNUSED(column);
                showMedicinesDetail(row);
            });

    // для того щоб можна було після сортування додавати і вдаляти
    ui->medicinesTable->blockSignals(true);

    DatabaseManager db;
    if(!db.connectDatabase())
    {
        ui->medicinesTable->blockSignals(false);
        return;
    }
    QSqlQuery query;
    query.exec("SELECT * FROM medicines");

    ui->medicinesTable->setSortingEnabled(false);
    ui->medicinesTable->setRowCount(0);

    int row = 0;
    //заповнення таблиці з бази даних
    while(query.next())
    {
        ui->medicinesTable->insertRow(row);

        ui->medicinesTable->setItem(row, 0,
                                    new QTableWidgetItem(QString::number(row + 1)));
        // заборона редагування
        ui->medicinesTable->item(row, 0)->setFlags(ui->medicinesTable->item(row, 0)->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *nameItem =
            new QTableWidgetItem(query.value("name").toString());


        nameItem->setData(Qt::UserRole, query.value("id"));

        ui->medicinesTable->setItem(row,1,nameItem);
        ui->medicinesTable->setItem(row, 2,
                                    new QTableWidgetItem(query.value("category").toString()));

        ui->medicinesTable->setItem(row, 3,
                                    new QTableWidgetItem(query.value("price").toString()));

        ui->medicinesTable->setItem(row, 4,
                                    new QTableWidgetItem(query.value("quantity").toString()));

        row++;
    }

    ui->medicinesTable->setSortingEnabled(true);
    ui->medicinesTable->blockSignals(false);
}
void MainWindow::styleTable(){

    // тут відміняю дублюванняф айді
    ui->medicinesTable->verticalHeader()->setVisible(false);
    ui->medicinesTable->setAlternatingRowColors(true);

    ui->medicinesTable->horizontalHeader()->setStretchLastSection(true);

    //таблиця ліків розтягування колонків
    ui->medicinesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->medicinesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    ui->medicinesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->medicinesTable->setEditTriggers(QAbstractItemView::DoubleClicked);

    //сортування
    ui->medicinesTable->setSortingEnabled(true);
    ui->medicinesTable->horizontalHeader()->setSectionsClickable(true);
    connect(ui->medicinesTable->horizontalHeader(),&QHeaderView::sectionClicked,this,[this](int index)
            {
                // № не сортуємо
                if(index == 0)

                {
                    ui->medicinesTable->setSortingEnabled(false);
                }
                else

                {
                    ui->medicinesTable->setSortingEnabled(true);
                }
            });

}

void MainWindow::CreateBatton(){
    connect(ui->addMedicineButton, &QPushButton::clicked, this, [this]() {
        AddMedicineDialog dialog(this);

        if(dialog.exec() == QDialog::Accepted)
        {
            DatabaseManager db;
            if(db.connectDatabase() &&
                db.addMedicine(dialog.getName(),
                               dialog.getCategory(),
                               dialog.getManufacturer(),
                               dialog.getDescription(),
                               dialog.getPrice(),
                               dialog.getQuantity()))
            {

                loadMedicines();
                loadSelasMedicines();
            }
            else
            {
                QMessageBox::warning(this, "Error", "Failed to add medicine!");
            }
        }
    });

}

void MainWindow::deleteMed(){
    //видалення
    connect(ui->deleteMedicineButton, &QPushButton::clicked, this, [this]() {

        int currentRow = ui->medicinesTable->currentRow();

        if(currentRow < 0)
        {
            QMessageBox::warning(this, "помилка", "Виберіть запис!");
            return;
        }

        int id = ui->medicinesTable->item(currentRow, 1)->data(Qt::UserRole).toInt();

        QMessageBox msgBox;

        msgBox.setWindowTitle("Видалення");
        msgBox.setText("видалити цей рядок?");
        QPushButton *yesButton = msgBox.addButton("Так", QMessageBox::YesRole);
        QPushButton *noButton = msgBox.addButton("Ні", QMessageBox::NoRole);
        msgBox.setIcon(QMessageBox::Warning);

        msgBox.exec();

        if(msgBox.clickedButton() == yesButton)
        {
            DatabaseManager db;

            if(db.connectDatabase() && db.deleteMedicine(id))
            {
                loadMedicines();
                loadSelasMedicines();
            }
            else
            {
                QMessageBox::warning(this, "Error", "Cannot delete!");
            }
        }

    });

}

void MainWindow::updateMedicine(){
    //кнопка додавання
    connect(ui->editMedicineButton, &QPushButton::clicked, this, [this]() {

        int currentRow = ui->medicinesTable->currentRow();

        if(currentRow < 0)
        {
            QMessageBox::warning(this, "помилка", "Виберіть запис!");
            return;
        }

        int id = ui->medicinesTable->item(currentRow, 1)->data(Qt::UserRole).toInt();

        QString name = ui->medicinesTable->item(currentRow, 1)->text();
        QString category = ui->medicinesTable->item(currentRow, 2)->text();
        double price = ui->medicinesTable->item(currentRow, 3)->text().toDouble();
        int quantity = ui->medicinesTable->item(currentRow, 4)->text().toInt();

        AddMedicineDialog dialog(this);
        QString manufacturer ;
        QString description;
        QSqlQuery detailQuery;
        detailQuery.prepare("SELECT manufacturer, description FROM medicines WHERE id = :id");
        detailQuery.bindValue(":id", id);

        if(detailQuery.exec() && detailQuery.next())
        {
            manufacturer = detailQuery.value("manufacturer").toString();
            description = detailQuery.value("description").toString();
        }

        dialog.setMedicineData(name, category,manufacturer,description,price, quantity);

        if (dialog.exec() == QDialog::Accepted)
        {
            DatabaseManager db;

            if(db.connectDatabase() &&
                db.updateMedicine(id,
                                  dialog.getName(),
                                  dialog.getCategory(),
                                  dialog.getManufacturer(),
                                  dialog.getDescription(),
                                  dialog.getPrice(),
                                  dialog.getQuantity()))
            {
                loadMedicines();
                loadSelasMedicines();
            }
            else
            {
                QMessageBox::warning(this, "помилка", "Не вдалося оновити запис!");
            }
        }
    });
    //можливість редагування зразу в рядку
    connect(ui->medicinesTable, &QTableWidget::cellChanged, this, [this](int row, int column) {

        if(column == 0)
            return;
        //перевірка на звєві символи
        if(column == 3)
        {
            bool ok;
            ui->medicinesTable->item(row, column)->text().toDouble(&ok);

            if(!ok)
            {
                QMessageBox::warning(this, "Error", "Price must be a number!");
                loadMedicines();
                return;
            }
        }
        //перевірка на звєві символи
        if(column == 4)
        {
            bool ok;
            ui->medicinesTable->item(row, column)->text().toInt(&ok);

            if(!ok)
            {
                QMessageBox::warning(this, "Error", "Quantity must be an integer!");
                loadMedicines();
                return;
            }
        }

        int id = ui->medicinesTable->item(row, 1)->data(Qt::UserRole).toInt();

        QString name = ui->medicinesTable->item(row, 1)->text();
        QString category = ui->medicinesTable->item(row, 2)->text();
        double price = ui->medicinesTable->item(row, 3)->text().toDouble();
        int quantity = ui->medicinesTable->item(row, 4)->text().toInt();

        QString manufacturer;
        QString description;

        QSqlQuery detailQuery;
        detailQuery.prepare("SELECT manufacturer, description FROM medicines WHERE id = :id");
        detailQuery.bindValue(":id", id);

        if(detailQuery.exec() && detailQuery.next())
        {
            manufacturer = detailQuery.value("manufacturer").toString();
            description = detailQuery.value("description").toString();
        }

        DatabaseManager db;

        if(!db.connectDatabase() ||
            !db.updateMedicine(id, name, category, manufacturer, description, price, quantity))
        {
            QMessageBox::warning(this, "Error", "Failed to save changes!");
        }
        else
        {
            loadSelasMedicines();
            showMedicinesDetail(row);
        }
    });


}
void MainWindow::searchMedicines(){
    connect(ui->MedicinesLineSeach, &QLineEdit::textChanged,this,[this](const QString &text){
        for(int row = 0;row<ui->medicinesTable->rowCount();row++){
            bool match = false;
            for(int col = 1;col <ui->medicinesTable->columnCount();col++){
                QTableWidgetItem*item = ui->medicinesTable->item(row,col);
                if(item && item->text().contains(text,Qt::CaseInsensitive)){
                    match = true;
                    break;
                }
            }


        }


    });

}

void MainWindow::showMedicinesDetail(int row){
    if(row<0)
        return;
    int id = ui->medicinesTable->item(row, 1)->data(Qt::UserRole).toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM medicines WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.exec() || !query.next())
        return;

    QString name = query.value("name").toString();
    QString category = query.value("category").toString();
    QString manufacturer = query.value("manufacturer").toString();
    QString description = query.value("description").toString();
    double price = query.value("price").toDouble();
    int quantity = query.value("quantity").toInt();

    QString status;
    //статус
    if(quantity == 0){
        status = "Немає в наявності";
    ui->detailStatusLabel->setStyleSheet(
        "background-color:#ff8686;"
        );
    }
    else if(quantity <= 5){
        status = "Закінчується";
    ui->detailStatusLabel->setStyleSheet(
        "background-color:#fffdb7;"
        );
    }
    else{
        status = "В наявності";
        ui->detailStatusLabel->setStyleSheet(
            "background-color:#c1ffc0;"
            );
    }
    //для опису щоб текст переносився
    ui->detailNameLabel->setText(name);
    ui->detailCategoryLabel->setText(category);
    ui->detailManufacturerLabel->setText(manufacturer);
    ui->detailDescriptionLabel->setText(description);
    ui->detailPriceLabel->setText(QString::number(price) + " UAH ");
    ui->detailQuantityLabel->setText(QString::number(quantity));
    ui->detailStatusLabel->setText(status);
    //для опису щоб текст переносився

}