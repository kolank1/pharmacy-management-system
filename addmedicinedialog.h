#ifndef ADDMEDICINEDIALOG_H
#define ADDMEDICINEDIALOG_H

#include <QDialog>

namespace Ui {
class AddMedicineDialog;
}

class AddMedicineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMedicineDialog(QWidget *parent = nullptr);
    ~AddMedicineDialog();

    QString getName() const;
    QString getCategory() const;
    double getPrice() const;
    int getQuantity() const;
    void setMedicineData(const QString &name,
                         const QString &category,
                         const QString &manufacturer,
                         const QString &description,
                         double price,
                         int quantity);
    QString getManufacturer() const;
    QString getDescription() const;

private:
    Ui::AddMedicineDialog *ui;
};

#endif // ADDMEDICINEDIALOG_H
