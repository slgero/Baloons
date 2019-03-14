#ifndef ADDNEWBALOONS_H
#define ADDNEWBALOONS_H

#include <database.h>
#include <QDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class AddNewBaloons;
}

class AddNewBaloons : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewBaloons(QWidget *parent = nullptr, Data db = nullptr);
    ~AddNewBaloons();

private slots:
    void on_close_clicked();

    void on_add_clicked();

    void on_delete_2_clicked();

    void on_save_clicked();

    void ChangeCostPrice();

    void on_checkBox_clicked();

private:
    Ui::AddNewBaloons *ui;
    Data data;
    bool save_flag;
    int row;
    QTableWidgetItem* wig;
    My_map to_data;
    std::vector<QString> to_delete;
    std::vector<int> to_delete_price;
    std::vector<int> for_baloons;
};

#endif // ADDNEWBALOONS_H
