#ifndef NEWSPEND_H
#define NEWSPEND_H

#include <QDialog>
#include <database.h>
#include <addnewbaloons.h>
#include <addoldbaloons.h>
#include <QMessageBox>

namespace Ui {
class NewSpend;
}

class NewSpend : public QDialog
{
    Q_OBJECT

public:
    explicit NewSpend(QWidget *parent = nullptr, Data db = nullptr);
    ~NewSpend();

private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::NewSpend *ui;
    Data data;
};

#endif // NEWSPEND_H
