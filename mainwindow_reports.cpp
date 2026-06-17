#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>


void MainWindow::reportsPage(){
    ui->DaysTable->horizontalHeader()
    ->setSectionResizeMode(QHeaderView::Stretch);

    ui->CheksTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    ui->ItemTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    ui->DaysTable->verticalHeader()->setVisible(false);

    ui->CheksTable->verticalHeader()->setVisible(false);

    ui->ItemTable->verticalHeader()->setVisible(false);


    connect(ui->MonthcomboBox,&QComboBox::currentTextChanged,this,[this](const QString &month)
            {
                loadReporsDays(month);
            }
            );
    connect(ui->DaysTable, &QTableWidget::cellClicked,
            this, [this](int row, int column){
                Q_UNUSED(column);
                QString date = ui->DaysTable->item(row, 0)->text();
                loadReportsChecks(date);
            });
    ui->DaysTable->setSortingEnabled(true);

    ui->CheksTable->setSortingEnabled(true);

    ui->ItemTable->setSortingEnabled(false);
    //заборона редагування
    ui->DaysTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->CheksTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->ItemTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);
    //щоб виділялась вся лінія
    ui->DaysTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->CheksTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->ItemTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    //
    ui->DaysTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->CheksTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->ItemTable->setSelectionMode(
        QAbstractItemView::SingleSelection);
    ui->DaysTable->setCurrentCell(-1,-1);

    ui->CheksTable->setCurrentCell(-1,-1);

    ui->ItemTable->setCurrentCell(-1,-1);
}

void MainWindow:: loadReporsMouth()
{
    ui->MonthcomboBox->clear();

    QSqlQuery query;

    query.exec(
        "SELECT DISTINCT "
        "strftime('%Y-%m',sale_date) AS month "
        "FROM sales "
        "ORDER BY month DESC"

        );

    while(query.next())
    {
        ui->MonthcomboBox->addItem(
            query.value(0).toString()
            );
    }
    if(ui->MonthcomboBox->count()>0)
    {
        loadReporsDays(ui->MonthcomboBox->currentText());
    }
    connect(ui->CheksTable, &QTableWidget::cellClicked,this, [this](int row, int column){
        Q_UNUSED(column);
        int saleId = ui->CheksTable->item(row, 0)->data(Qt::UserRole).toInt();
        loadReportItem(saleId);
    });
}

void MainWindow::loadReporsDays(const QString &mouth)
{
    ui->DaysTable->setRowCount(0);

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "DATE(s.sale_date) AS day, "
        "COUNT(DISTINCT s.id) AS checks_count, "
        "IFNULL(SUM(si.quantity), 0) AS items_count, "
        "(SELECT SUM(total_price) "
        "FROM sales "
        " WHERE DATE(sale_date) = DATE(s.sale_date)) AS total_sum "
        "FROM sales s   "
        "LEFT JOIN sale_items si ON s.id = si.sale_id "
        "WHERE strftime('%Y-%m', s.sale_date) = :month "
        "GROUP BY DATE(s.sale_date) "
        "ORDER BY day DESC"
        );

    query.bindValue(":month", mouth);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return;
    }

    int row = 0;

    while(query.next())
    {
        ui->DaysTable->insertRow(row);

        ui->DaysTable->setItem(row, 0,
                               new QTableWidgetItem(query.value("day").toString()));

        ui->DaysTable->setItem(row, 1,
                               new QTableWidgetItem(query.value("checks_count").toString()));

        ui->DaysTable->setItem(row, 2,
                               new QTableWidgetItem(query.value("items_count").toString()));

        ui->DaysTable->setItem(row, 3,
                               new QTableWidgetItem(query.value("total_sum").toString() + " UAH"));

        row++;
    }

    ui->CheksTable->setRowCount(0);
    ui->ItemTable->setRowCount(0);
}

void MainWindow::loadReportsChecks(const QString &date)
{
    ui->CheksTable->setRowCount(0);
    ui->ItemTable->setRowCount(0);

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "s.id, "
        "time(s.sale_date) AS sale_time, "
        "s.seller, "
        "SUM(si.quantity) AS items_count, "
        "s.total_price "
        "FROM sales s "
        "LEFT JOIN sale_items si ON s.id = si.sale_id "
        "WHERE DATE(s.sale_date) = :date "
        "GROUP BY s.id "
        "ORDER BY s.sale_date DESC"
        );

    query.bindValue(":date", date);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return;
    }

    int row = 0;

    while(query.next())
    {
        ui->CheksTable->insertRow(row);

        QTableWidgetItem *checkItem =
            new QTableWidgetItem(QString::number(row + 1));

        checkItem->setData(Qt::UserRole, query.value("id").toInt());

        ui->CheksTable->setItem(row, 0, checkItem);
        ui->CheksTable->setItem(row, 1,
                                new QTableWidgetItem(query.value("sale_time").toString()));

        ui->CheksTable->setItem(row, 2,
                                new QTableWidgetItem(query.value("seller").toString()));

        ui->CheksTable->setItem(row, 3,
                                new QTableWidgetItem(query.value("items_count").toString()));

        ui->CheksTable->setItem(row, 4,
                                new QTableWidgetItem(query.value("total_price").toString() + " UAH"));

        row++;
    }
}
void MainWindow::loadReportItem(int seleId)
{
    ui->ItemTable->setRowCount(0);

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "medicine_name, "
        "price, "
        "quantity, "
        "sum "
        "FROM sale_items "
        "WHERE sale_id = :sale_id"
        );

    query.bindValue(":sale_id", seleId);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return;
    }

    int row = 0;

    while(query.next())
    {
        ui->ItemTable->insertRow(row);

        ui->ItemTable->setItem(row, 0,
                               new QTableWidgetItem(query.value("medicine_name").toString()));

        ui->ItemTable->setItem(row, 1,
                               new QTableWidgetItem(query.value("price").toString() + " UAH"));

        ui->ItemTable->setItem(row, 2,
                               new QTableWidgetItem(query.value("quantity").toString()));

        ui->ItemTable->setItem(row, 3,
                               new QTableWidgetItem(query.value("sum").toString() + " UAH"));

        row++;
    }
}