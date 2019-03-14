#include "addoldbaloons.h"
#include "ui_addoldbaloons.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

AddOldBaloons::AddOldBaloons(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::AddOldBaloons),
    data(db), TextEditingFlag(true)
{
    ui->setupUi(this);
    if(!db){
        qDebug() << "Oh! I am from AddOldBaloons and you FORGOT pass the parameter to Data!!!";
    } else {
       qDebug() << "Hi, I am from AddOldBaloons and size of map is" << data->GetMap().size();
       UpdateList();
       ui->status_bar->hide();
       ui->flag->show();
    }
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(ChangePrice()));
    timer->start(25);
}

void AddOldBaloons::UpdateList(){
    qDebug() << "Hi, I am from AddOldBaloons, this is UpdateList() and new size of map is" << data->GetMap().size();
    for (const auto& x : data->GetMap()){
        ui->listWidget->addItem(x.first);
    }
}

AddOldBaloons::~AddOldBaloons()
{
    delete ui;
}

void AddOldBaloons::ChangePrice(){
    if (!ui->flag->isHidden() && !ui->line_count->text().isEmpty() &&
                                                        data->GetMap().size() != 0){
        qDebug() << "I am in changePrice!";
        if (data->GetMap().count(ui->listWidget->currentItem()->text()) != 0 &&
                ui->listWidget->currentItem()->isSelected())
        {
            ui->line_price->setText(QString::number(ui->line_count->text().toInt() *
                                                    data->GetMap().at(ui->listWidget->currentItem()->text())[1]));
        }
    }
}

void AddOldBaloons::on_cancel_clicked()
{
    close();
}

void AddOldBaloons::on_add_clicked()
{
    // actually here I can use QIntValidator. I do it in the next update :)
    if (data->GetMap().size() == 0){
        QMessageBox::critical(this, "Ошибка", "У Вас отсутсвуют шары. Сначала Вам придётся их купить.");
    } else if(!ui->listWidget->currentItem()->isSelected())
       QMessageBox::critical(this, "Ошибка", "Не выбрана позиция шара.");
   else if(ui->line_count->text().isEmpty()){
       QMessageBox::critical(this, "Ошибка", "Неверно указано количество.");
   } else if (ui->line_price->text().isEmpty()){
       QMessageBox::critical(this, "Ошибка", "Неверно указана цена.");
   } else{
       int count = ui->line_count->text().toInt();
       int price = ui->line_price->text().toInt();
       if (count == 0 || count > 2000) {
           QMessageBox::critical(this, "Ошибка", "Не правильно заполнено \"количество\".");
       } else if(price == 0 || price > 10000){
           QMessageBox::critical(this, "Ошибка", "Неправильно заполнено \"цена\"");
       } else {
           data->ChangeCost(price, Items::BALOONS);
           qDebug() << "I add new price of BALOONS in DataBase. The price is:" << price;
           data->ChangeOldBaloons(ui->listWidget->currentItem()->text(), count);
           qDebug() << "Hey, all is OK, we will change value in map";

           ui->line_count->clear();
           ui->line_price->clear();

           ui->status_bar->show();
           ui->flag->hide();
           TextEditingFlag = true;
           QTimer::singleShot(1000 * 2, ui->status_bar, SLOT(hide()));
       }
   }
}


void AddOldBaloons::on_line_price_textEdited(const QString &arg1)
{
    if (arg1.isEmpty()){} // I don't need this parametr
    if (TextEditingFlag){
        ui->line_price->clear();
        ui->flag->hide();
        TextEditingFlag = false;
    }
}
