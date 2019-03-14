#ifndef ORDER_H
#define ORDER_H

#include <QDialog>
#include <database.h>
#include <QTableWidgetItem>
#include <QShortcut>

namespace Ui {
class Order;
}

class Order : public QDialog
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = nullptr, Data db = nullptr);
    bool CheckCountInTable();
    QDate GetCurrenDate();
    int GetBaloonsTotalAmount();
    QString GetStatus();
    ~Order();


private slots:

    void ChangePrice();
    void on_add_clicked();
    void on_exit_without_saving_clicked();

    void on_del_clicked();

    void on_number_textEdited(const QString &arg1);

    void on_save_and_exit_clicked();

    void on_TotalCost_2_textChanged(const QString &arg1);

    void on_checkBox_2_clicked();

    void on_AdditionalExpenses_textChanged(const QString &arg1);

    void on_price_textEdited(const QString &arg1);

private:
    Ui::Order *ui;
    Data data;
    My_vector to_data;
    int row;
    int list_row;
    QTableWidgetItem* wid;
    bool TextEditingFlag;
    bool TextEditingFlag2;
    bool AdditionalExpenses;
    QShortcut *keyEsc;
    QShortcut *keySpace;
};

#endif // ORDER_H
