#include "newspend.h"
#include "ui_newspend.h"
#define PRICE ui->lineEdit->text().toInt()

NewSpend::NewSpend(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::NewSpend),
    data(db)
{
    ui->setupUi(this);
    ui->label->hide();
    ui->label_2->hide();
    ui->lineEdit->hide();
}

NewSpend::~NewSpend()
{
    delete ui;
}

void NewSpend::on_comboBox_activated(const QString &arg1)
{
    if (arg1 != "Шары"){
        ui->label->show();
        ui->label_2->show();
        ui->lineEdit->show();
        ui->radioButton->hide();
        ui->radioButton_2->hide();
        ui->pushButton->setText("Принять");
        ui->pushButton->setIcon(QPixmap(":/new/prefix1/icons/Allowed.svg_.png"));
    } else {
        ui->label->hide();
        ui->label_2->hide();
        ui->lineEdit->hide();
        ui->radioButton->show();
        ui->radioButton_2->show();
        ui->pushButton->setText("Далее");
        ui->pushButton->setIcon(QPixmap(":/new/prefix1/icons/Right-07-512.png"));
    }
}

void NewSpend::on_pushButton_3_clicked()
{

}

void NewSpend::on_pushButton_2_clicked()
{
    close();
}

void NewSpend::on_pushButton_clicked()
{
    if(ui->comboBox->currentText() == "Гелий"){
        if (ui->lineEdit->text().isEmpty()){
            QMessageBox::warning(this,"Ошибка", "Для продолжения нужно ввести цену.");
        }
        else if (PRICE == 0){
            QMessageBox::warning(this,"Ошибка", "Опечатка при вводе цены.");
        } else if (PRICE < 1000)
            QMessageBox::warning(this,"Ошибка", "Цена за гелий слишком маленькая, возможно опечатка.");
        else {
            data->ChangeCost(PRICE, Items::HELIUM);
            close();
        }
    } else if (ui->comboBox->currentText() == "Другое"){
        if (ui->lineEdit->text().isEmpty()){
            QMessageBox::warning(this,"Ошибка", "Для продолжения нужно ввести цену.");
        }
        else if (PRICE == 0){
            QMessageBox::warning(this,"Ошибка", "Опечатка при вводе цены.");
        } else if (PRICE > 30000)
            QMessageBox::warning(this,"Ошибка", "Возможно у Вас опечатка.");
        else {
            data->ChangeCost(PRICE, Items::OTHER);
            close();
        }
    } else {
        if (ui->radioButton_2->isChecked()){
            AddOldBaloons baloons(this, data);
            baloons.setModal(true);
            baloons.exec();
        } else {
            AddNewBaloons baloons(this, data);
            baloons.setModal(true);
            baloons.exec();
        }
        close();
    }
}
