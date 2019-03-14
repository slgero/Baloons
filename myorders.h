#ifndef MYORDERS_H
#define MYORDERS_H

#include <QDialog>
#include <database.h>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QShortcut>

namespace Ui {
class MyOrders;
}

class MyOrders : public QDialog
{
    Q_OBJECT

public:
    explicit MyOrders(QWidget *parent = nullptr, Data db = nullptr);
    void SetIterators();
    void DisplayCurentDate();
    void SetCountOfActiveOrders();
    void DisplayCurentPosition();
    void DisplayOrder();
    void AddSocial(const My_vector& tmp);
    void ReturnBaloonsInDataBase();
    void ClearTable();
    ~MyOrders();

private slots:
    void on_right_clicked();

    void on_left_clicked();

    void on_PBgo_away_clicked();

    void on_PBcancel_clicked();

    void on_PBcomplete_clicked();

    void on_pushButton_clicked();

    void on_checkBoxDate_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MyOrders *ui;
    Data data;
    My_set::const_iterator set_iterator;
    SuperMap::const_iterator map_iterator;
    size_t current_position;
    int row;
    QTableWidgetItem *wid;
    QShortcut *keyLeft;
    QShortcut *keyRight;
};

#endif // MYORDERS_H
