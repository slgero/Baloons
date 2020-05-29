#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>
#include <database.h>
#include <hint.h>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QValidator>

namespace Ui {
class Statistics;
}

class Statistics : public QDialog
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr, Data db = nullptr);

    // For first Window:
    void CreateSocialBatChart();
    void CreateOrderGraph();
    int GetMaximumCountOfSocialNetworks();
    void DisplayALL();
    void ClearGraph();

    // For second Window:
    void SetEnableForRadoiButton(bool flag);
    void DisplayTable();
    void DisplayFrame3();
    void CreateAllTrueVector();
    bool CheckIntEnter();
    void ChangeFrame3();

    ~Statistics();

private slots:
    void on_checkBox_clicked();

    void on_dateEdit_2_userDateChanged(const QDate &date);

    void on_pushButton_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_toggled(bool checked);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_8_textChanged(const QString &arg1);

    void on_lineEdit_9_textChanged(const QString &arg1);

    void on_lineEdit_10_textChanged(const QString &arg1);

    void on_lineEdit_11_textChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Statistics *ui;
    Data data;
    int maximum_count;
    QDate user_date;
    int max_count_of_orders;
    QTableWidgetItem *wid;
    std::vector<bool> flags;
};

#endif // STATISTICS_H
