#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include <string>
#include <QListWidget>
#include <QMenuBar>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), settings(ORGANIZATION_NAME, APPLICATION_NAME)
{
    ui->setupUi(this);

    db = std::make_shared<DataBase>();
    ReadSettings();
    UpdateWindowName();
    qDebug() << "Main W";
}

void MainWindow::UpdateWindowName(){
    setWindowTitle(GetOrgName() + " @ " + db->GetDataBaseName());
}

MainWindow::~MainWindow()
{
    if (db->GetPATH().isEmpty()){
        QMessageBox::StandardButton quest =
                QMessageBox::question(this, "Вопрос", "Вы не указали путь для сохранения Базы Данных.\n"
                                                      "Хотите это сделать сейчас?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);
        if (quest == QMessageBox::Yes){
            QString fileName = QFileDialog::getSaveFileName(this, "Сохранить базу данных как...",
                                                             ".", tr("Файл базы данны (*.txt)"));
            if (fileName.isEmpty()){
                QMessageBox::warning(this, "Ошибка", "Вы не указали путь до сохранения!\n"
                                                     "Сохранение проводиться не будет!");
            } else {
                db->SetPATH(fileName);
                qDebug() << "I save DataBase, it is a 1 condition in ~";
                db->SaveDataBase();
                WriteSettings();
            }
        }
    }
    else {
        qDebug() << "I save DataBase, it is a 2 condition in ~";
        db->SaveDataBase();
        WriteSettings();
    }
    delete ui;
}

void MainWindow::SetOrganisationName(const QString& name){
    settings.beginGroup("/Settings");
    qDebug() << "I add org name =" << name;
    settings.setValue("/organisation_name2", name);
    settings.endGroup();
    SetOrgName(name);
//    UpdateWindowName();
}

QString MainWindow::GetOrganisationName(){
    settings.beginGroup("/Settings");
    QString tmp = settings.value("/organisation_name2", "").toString();
    qDebug() << "I get org name =" << tmp;
    settings.endGroup();
    return tmp;
}

void MainWindow::ReadSettings(){
    settings.beginGroup("/Settings");
    db->SetPATH(settings.value("/path2", "").toString());
    QString tmp = GetOrganisationName();
    if(tmp.isEmpty()){
        Greeting greet(this);
        greet.setModal(true);
        greet.exec();
    } else {
        SetOrgName(tmp);
    }
    if (!db->GetPATH().isEmpty()){
        if (db->ReadDataBase()){
            ui->statusBar->showMessage("База данных успешно загружена.", 2000);
        } else {
             QMessageBox::critical(this, "Жопа...", "Невозможно прочитать файл базы данных.\n"
                                                    "Попробуйте заново открыть нужный Вам файл. Если подобная ошибка\n"
                                                    "повторится и в следующий раз, то скорее всего\n"
                                                    "Вы потеряли Ваши данные. Надеюсь, Вы воспользовались\n"
                                                    "моим советом и создали резервную копию.");
        }
    } else {
        ui->statusBar->showMessage("База данных не загружена.", 2000);
    }
    qDebug() << "PATH =" << db->GetPATH();

    settings.endGroup();
}

void MainWindow::WriteSettings(){
     settings.beginGroup("/Settings");
     settings.setValue("/path2", db->GetPATH());
     settings.endGroup();
}

void MainWindow::on_spend_money_clicked()
{
    NewSpend spend(this,  db);
    spend.setModal(true);
    spend.exec();
}

void MainWindow::on_earned_money_clicked()
{
    Order order(this, db);
    order.setModal(true);
    order.exec();
}

void MainWindow::on_my_orders_clicked()
{
    MyOrders orders(this, db);
    orders.setModal(true);
    orders.exec();
}

void MainWindow::on_view_statistics_clicked()
{
    Statistics stat(this, db);
    stat.setModal(true);
    stat.exec();
}

void MainWindow::on_openAction_triggered()
{
    /*
    Создание диалогового окна getOpenFileName в которое мы передаем 4 различных параметра.
        1 параметр задает родителя данного диалогового окна
        2 параметр задает заголовок данного окна
        3 параметр задает корневую папку с которой необходимо начать поиск файла, в нашем случае мы передали "." - это означает что поиск файла необходимо начать с корневой папки нашего приложения
        4 параметр задает расширение файла который мы можем открыть в нашей таблицы
    */
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть базу данных"),
                                                    ".", tr("Файл базы данных (*.txt)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы не указали путь до базы данных!\n"
                                             "Отркрытие невозможно!");
    } else {
        QMessageBox::StandardButton quest =
                QMessageBox::question(this, "Вопрос", "Сохранить эту базу данных?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (quest == QMessageBox::Yes){
            if (db->GetPATH().isEmpty()){
                on_save_as_triggered();
            }else {
                db->SaveDataBase();                     // Save Old DataBase
            }
        }
        db = std::make_shared<DataBase>();          // Create New DataBase
        db->SetPATH(fileName);                      // Set new PATH
        WriteSettings();                            // Update Path to open current document later
        db->ReadDataBase(); // Make it bool !!!
        UpdateWindowName();
        ui->statusBar->showMessage("База данных успешно загружена.", 2000);
    }
}

void MainWindow::on_save_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить базу данных как...",
                                                    ".", tr("Файл базы данны (*.txt)"));
   if (fileName.isEmpty()){
       QMessageBox::warning(this, "Ошибка", "Вы не указали путь до сохранения!\n"
                                            "Сохранение проводиться не будет!");
   } else {
        db->SetPATH(fileName);
        db->SaveDataBase();
        WriteSettings();
        UpdateWindowName();
        ui->statusBar->showMessage("База данных успешно сохранена.", 2000);
   }
}

void MainWindow::on_action_triggered()
{
    FAQ faq(this);
    faq.setModal(true);
    faq.exec();
}

void MainWindow::on_new_2_triggered()
{
    QMessageBox::StandardButton quest =
            QMessageBox::question(this, "Вопрос", "Сохранить эту базу данных?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (quest == QMessageBox::Yes){
        if (db->GetPATH().isEmpty()){
            on_save_as_triggered();
        }else {
            db->SaveDataBase();                     // Save Old DataBase
        }
    }
    db = std::make_shared<DataBase>();          // Create New DataBase
    db->SetPATH("");                            // Set EmptyPath
    WriteSettings();                            // Change PATH
    UpdateWindowName();
}

void MainWindow::SetOrgName(const QString& name){
    organisation_name = name;
}

QString MainWindow::GetOrgName(){
    return organisation_name;
}

void MainWindow::on_save_triggered()
{
    if (db->GetPATH().isEmpty()){
        on_save_as_triggered();
    }else {
        db->SaveDataBase();                     // Save Old DataBase
    }
}
