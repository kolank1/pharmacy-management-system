#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    void setupLogin();
    void mainPage();
    void loadMedicines();
    void CreateBatton();
    void styleTable();
    void deleteMed();
    void updateMedicine();
    void searchMedicines();
    void showMedicinesDetail(int row);
    // продажі
    void selasPage();
    void loadSelasMedicines();
    void addToCheck(int row);
    void CardTotalSum();
    void cleanCard();
    void searchSelas();
    void mekeSale();

    //звіти
    void reportsPage();
    void loadReporsMouth();
    void loadReporsDays(const QString &mouth);
    void loadReportsChecks(const QString &date);
    void loadReportItem(int seleId);
    QString currentUserLogin;

    //користувачі
    QString currentRole;
    void userPage();
    void loadUsers();
    void addUser();
    void deleteUser();
    void editUser();





};
#endif // MAINWINDOW_H
