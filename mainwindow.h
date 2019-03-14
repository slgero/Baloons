#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <database.h>
#include <QListWidget>
#include "newspend.h"
#include "myorders.h"
#include <order.h>
#include <faq.h>
#include <statistics.h>
#include <QSettings>

#define ORGANIZATION_NAME "Slgero"
#define ORGANIZATION_DOMAIN "sersavvov@yandex.ru"
#define APPLICATION_NAME "Baloons"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void ReadSettings();
    void WriteSettings();
    ~MainWindow();

private slots:

    void on_spend_money_clicked();

    void on_earned_money_clicked();

    void on_my_orders_clicked();

    void on_view_statistics_clicked();

    void on_openAction_triggered();

    void on_save_as_triggered();

    void on_action_triggered();

private:
    Ui::MainWindow *ui;
    Data db;
    QSettings settings;
};

#endif // MAINWINDOW_H
