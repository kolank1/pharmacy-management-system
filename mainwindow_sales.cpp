#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSpinBox>
#include <QPushButton>
#include <QHeaderView>
#include "stylemanager.h"

void MainWindow::selasPage(){
    // для розтягування
    ui->SelasMedicinesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SelasMedicinesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    // ховання номерів
    ui->SelasMedicinesTable->verticalHeader()->setVisible(false);
    //  ui->SelasMedicinesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //шоб не редагувати таблицю.
    ui->SelasMedicinesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->CardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->CardTable->verticalHeader()->setVisible(false);
    //ДЛЯ кольору
    ui->SelasMedicinesTable->setAlternatingRowColors(true);

    //шоб не редагувати таблицю.
    ui->CardTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->CardTable->setAlternatingRowColors(true);
    //щоб не можна було сортувати
    ui->CardTable->setSortingEnabled(false);
    ui->CardTable->horizontalHeader()->setSectionsClickable(false);
    //кнопка оформити продаж
    connect(ui->mekeSaleButton,&QPushButton::clicked,this,[this](){
        mekeSale();


    });
}

void MainWindow::loadSelasMedicines(){
    QSqlQuery query;
    query.exec("SELECT* FROM medicines WHERE quantity > 0");
    ui->SelasMedicinesTable->setRowCount(0);
    int row = 0;
    while(query.next())
    {
        ui->SelasMedicinesTable->insertRow(row);

        ui->SelasMedicinesTable->setItem(row, 0,
                                         new QTableWidgetItem(QString::number(row + 1)));
        \
            QTableWidgetItem *nameItem =
            new QTableWidgetItem(query.value("name").toString());


        nameItem->setData(Qt::UserRole, query.value("id"));


        ui->SelasMedicinesTable->setItem(row,1,nameItem);

        ui->SelasMedicinesTable->setItem(row, 2,
                                         new QTableWidgetItem(query.value("manufacturer").toString()));

        ui->SelasMedicinesTable->setItem(row, 3,
                                         new QTableWidgetItem(query.value("price").toString()));

        ui->SelasMedicinesTable->setItem(row, 4,
                                         new QTableWidgetItem(query.value("quantity").toString()));

        QPushButton *addButton = new QPushButton("Додати");
        StyleManager::setBlueButton(addButton);

        QWidget *cellWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(cellWidget);

        layout->addStretch();
        layout->addWidget(addButton);
        layout->addStretch();
        layout->setContentsMargins(0, 0, 0, 0);
        ui->SelasMedicinesTable->setCellWidget(row, 5, cellWidget);
        StyleManager::setBlueButton(addButton);

        connect(addButton,&QPushButton::clicked,this , [this,row](){
            addToCheck(row);
        });


        row++;
    }
}

// додавання до чеку
void MainWindow::addToCheck(int row){

    QString name = ui->SelasMedicinesTable->item(row,1)->text();
    double price = ui->SelasMedicinesTable->item(row,3)->text().toDouble();
    int quantity = 1;

    double sum = price * quantity;
    int medicineId = ui->SelasMedicinesTable->item(row,1)->data(Qt::UserRole).toInt();
    // для додавання в один рядок
    for(int i = 0; i < ui->CardTable->rowCount(); i++)
    {
        int idInCard = ui->CardTable->item(i, 1)->data(Qt::UserRole).toInt();

        if(idInCard == medicineId)
        {
            QSpinBox *spin =
                qobject_cast<QSpinBox*>(ui->CardTable->cellWidget(i, 2));

            if(spin->value() < spin->maximum())
            {
                spin->setValue(spin->value() + 1);
            }
            else
            {
                QMessageBox::warning(this, "помилка", "недостатньо товару");
            }

            return;
        }
    }
    int checkRow = ui->CardTable->rowCount();
    ui->CardTable->insertRow(checkRow);


    QTableWidgetItem *nameItem = new QTableWidgetItem(name);

    nameItem->setData( Qt::UserRole, medicineId);

    ui->CardTable->setItem(checkRow,1, nameItem);


    ui->CardTable->setItem(checkRow,0,new QTableWidgetItem(QString::number(checkRow +1 )));



    int availableQuantity = ui->SelasMedicinesTable->item(row, 4)->text().toInt();

    QSpinBox *spin = new QSpinBox();
    spin->setMinimum(1);
    spin->setMaximum(availableQuantity);
    spin->setValue(1);

    ui->CardTable->setCellWidget(checkRow,2,spin);

    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged),this,[this, spin, checkRow](int value)
            {

                double price =
                    ui->CardTable->item(checkRow,3)->text().toDouble();

                double sum = value * price;

                ui->CardTable->setItem(
                    checkRow,
                    4,
                    new QTableWidgetItem(QString::number(sum))
                    );

                CardTotalSum();

            });

    ui->CardTable->setItem(checkRow,3,new QTableWidgetItem(QString::number(price)));

    ui->CardTable->setItem(checkRow,4,new QTableWidgetItem(QString::number(sum)));

    QPushButton * deleteButton = new QPushButton();
    ui->CardTable->setCellWidget(checkRow ,5,deleteButton);
    deleteButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));


    connect(deleteButton,&QPushButton::clicked,this , [this,deleteButton](){
        int row = ui->CardTable->indexAt(deleteButton->pos()).row();
        ui->CardTable->removeRow(row);
        CardTotalSum();
    });
    CardTotalSum();
}
// загальна сума
void MainWindow::CardTotalSum(){
    double total = 0;

    for (int i = 0; i < ui->CardTable->rowCount(); i++)
    {
        if(ui->CardTable->item(i,4)){
            total += ui->CardTable->item(i, 4)->text().toDouble();
        }
    }

    ui->TotalSalesLabel->setText(QString::number(total) + " UAH");

}
//кнопка очистити чек
void MainWindow::cleanCard(){
    connect(ui->clearCardButton,&QPushButton::clicked,this,[this](){
        QMessageBox reply;


        reply.setWindowTitle("Видалення");
        reply.setText("Очистити чек?");

        QPushButton *yesButton = reply.addButton("Так", QMessageBox::YesRole);
        QPushButton *noButton = reply.addButton("Ні", QMessageBox::NoRole);
        reply.setIcon(QMessageBox::Warning);
        reply.exec();

        if(reply.clickedButton() == yesButton ){
            ui->CardTable->setRowCount(0);
            CardTotalSum();
        }
    });
}
//кнопка пошуку в продажі
void MainWindow::searchSelas(){
    connect(ui->selasSearchEdit, &QLineEdit::textChanged,this,[this](const QString &text){
        for(int row = 0;row< ui->SelasMedicinesTable->rowCount();row++){
            bool match = false;
            for(int col = 1;col < ui->SelasMedicinesTable->columnCount();col++){
                QTableWidgetItem*item = ui->SelasMedicinesTable->item(row,col);
                if(item && item->text().contains(text,Qt::CaseInsensitive)){
                    match = true;
                    break;
                }
            }
            ui->SelasMedicinesTable->setRowHidden(row,!match);
        }
    });
}
void MainWindow::mekeSale(){
    if(ui->CardTable->rowCount()==0){
        QMessageBox::warning(this,"полмика","чек порожній");
        return;
    }
    QSqlQuery query;
    double total = 0;

    for(int i = 0;i<ui->CardTable->rowCount();i++){
        total+=ui->CardTable->item(i,4)->text().toDouble();
    }
    query.prepare(
        "INSERT INTO sales "
        "(sale_date, total_price, seller) "
        "VALUES(datetime('now'), :total, :sellar)"
        );
    query.addBindValue(total);
    query.addBindValue(currentUserLogin);

    if(!query.exec())
    {
        QMessageBox::warning(
            this,
            "SQL Error",
            query.lastError().text()
            );
        return;
    }
    int saleId = query.lastInsertId().toInt();
    for(int row=0;row<ui->CardTable->rowCount();row++)
    {
        int medicineId =ui->CardTable->item(row,1)->data(Qt::UserRole).toInt();

        QString medicineName =ui->CardTable->item(row,1)->text();
        QSpinBox *spin = qobject_cast<QSpinBox*>(ui->CardTable->cellWidget(row,2));

        int quantity =spin->value();
        double price = ui->CardTable->item(row,3)->text().toDouble();
        double sum =ui->CardTable->item(row,4)->text().toDouble();

        QSqlQuery itemQuery;
        itemQuery.prepare(
            "INSERT INTO sale_items "
            "(sale_id, medicine_id, medicine_name, quantity, price, sum) "
            "VALUES(:sale_id, :medicine_id, :medicine_name, :quantity, :price, :sum)"
            );

        itemQuery.bindValue(":sale_id", saleId);
        itemQuery.bindValue(":medicine_id", medicineId);
        itemQuery.bindValue(":medicine_name", medicineName);
        itemQuery.bindValue(":quantity", quantity);
        itemQuery.bindValue(":price", price);
        itemQuery.bindValue(":sum", sum);

        if(!itemQuery.exec())
        {
            QMessageBox::warning(this, "Момилка", "Не вдалось оновити чек!");
            qDebug() << itemQuery.lastError().text();
            return;
        }

        QSqlQuery updateQuery;
        updateQuery.prepare(
            "UPDATE medicines "
            "SET quantity = quantity - :quantity "
            "WHERE id = :id"
            );

        updateQuery.bindValue(":quantity", quantity);
        updateQuery.bindValue(":id", medicineId);

        if(!updateQuery.exec())
        {
            QMessageBox::warning(this, "Помилка", "Не вдалось оновити кількічість товару!");
            qDebug() << updateQuery.lastError().text();
            return;
        }
    }

    ui->CardTable->setRowCount(0);
    CardTotalSum();
    loadMedicines();
    loadSelasMedicines();
    loadReporsMouth();

    QMessageBox::information(this, "Успіх", "Продаж успішно оформлено!");
}