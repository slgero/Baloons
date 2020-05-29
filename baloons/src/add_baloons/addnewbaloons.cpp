#include "addnewbaloons.h"
#include "ui_addnewbaloons.h"
#include <QList>
#include <QTimer>

#define NameInRow ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text()
#define FIRST ui->helium_price->text().isEmpty()
#define SECOND ui->helium_price->text().toInt() > 300
#define THIRD ui->helium_price->text().toInt() == 0
#define CHECKBOX_SHOW ui->helium_price->show(); \
                      ui->label_12->show();     \
                      ui->label_8->show();

AddNewBaloons::AddNewBaloons(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::AddNewBaloons),
    data(db), save_flag(true), row(1), wig(nullptr)
{
    ui->setupUi(this);
    ui->status_bar->hide();


    ui->warning->setToolTip("Данное название будет использоваться в дальнейшем,\n"
                                            "поэтому будьте внимательнее");

    QTimer *MyTimer = new QTimer(this);
    QObject::connect(MyTimer, SIGNAL(timeout()), this, SLOT(ChangeCostPrice()));
    MyTimer->start(25);
}

AddNewBaloons::~AddNewBaloons()
{
    delete ui;
}

void AddNewBaloons::ChangeCostPrice(){
    if (!ui->price1->text().isEmpty() && !ui->count->text().isEmpty()){
        double CostPrice =  ui->price1->text().toDouble()/ui->count->text().toDouble();
        if (!ui->checkBox->isChecked() && !ui->helium_price->text().isEmpty()){
            CostPrice += ui->helium_price->text().toInt();
        }
        ui->lineEdit_2->setText(QString::number(CostPrice));
        if (!ui->price2->text().isEmpty()){
            ui->lineEdit_3->setText(QString::number(ui->price2->text().toDouble() - CostPrice));
        }
    } else {
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
    }
}

void AddNewBaloons::on_close_clicked()
{
    if (!save_flag){
        QMessageBox::StandardButton quest =
                QMessageBox::question(this, "Вопрос", "Сохранить изменения?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);
        if (quest == QMessageBox::Yes){
            this->on_save_clicked();
            close();
        } else if (quest == QMessageBox::Close){
            // Do nothing
        } else {
            close();
        }
    } else{
        close();
    }
}

void AddNewBaloons::on_add_clicked()
{
    if(ui->name->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Отсутсвует название шара.");
    } else if(!data->CheckElementInMap(ui->name->text()) ||
                                        to_data.count(ui->name->text()) == 1){
        QMessageBox::warning(this, "Ошибка", "Шар с таким названием уже есть в базе, пожалуйста, выберите другое");
    } else if (ui->count->text().isEmpty()){
         QMessageBox::warning(this, "Ошибка", "Не введено количество шаров.");
    } else if (ui->price1->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Закупочная стоимость не введена.");
    } else if (ui->price2->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Розничная стоимость не введена.");
    } else if (!ui->checkBox->isChecked() && (FIRST || SECOND || THIRD)) {
        if (FIRST || THIRD) {
            QMessageBox::warning(this, "Ошибка", "Цена за гелий неккоректно заполнена.");
        } else if (SECOND){
            QMessageBox::warning(this, "Ошибка", "Слишком большая цена за гелий, возможно Вы ошиблись.");
        }
    } else {
        QString name = ui->name->text();
        int count = ui->count->text().toInt();
        int price1 = ui->price1->text().toInt();
        int price2 = ui->price2->text().toInt();
        if (count == 0 || price1 == 0 || price2 == 0){
            QMessageBox::warning(this, "Ошибка", "Неправильно заполнены поля");
        } else {
            wig = new QTableWidgetItem(name);
            ui->tableWidget->setItem(row, 0, wig); // Add to table name of baloon

            // Thanks to RAII it nessesary do delete QTableWidgetItem
            wig = new QTableWidgetItem(QString::number(ui->count->text().toInt() * ui->spinBox->value()));
            ui->tableWidget->setItem(row, 1, wig); // Add to table count

            wig = new QTableWidgetItem(ui->lineEdit_2->text());
            ui->tableWidget->setItem(row, 2, wig); // Add to table price1

            wig = new QTableWidgetItem(ui->price2->text());
            ui->tableWidget->setItem(row, 3, wig); // Add to table price2

            // Add to map new items
            double CostPrice =  ui->price1->text().toDouble()/ui->count->text().toDouble();
            if (!ui->checkBox->isChecked() && !ui->helium_price->text().isEmpty()){
                CostPrice += ui->helium_price->text().toInt();
            }
            to_data.insert(std::make_pair(name, std::vector<double>{
                                              ui->count->text().toDouble() * ui->spinBox->value(),
                                                                 CostPrice,
                                                                 ui->price2->text().toDouble()}));
            for_baloons.insert(std::make_pair(ui->name->text(), price1  * ui->spinBox->value()));

            // To clear fields after adding
            ui->price2->clear();
            ui->price1->clear();
            ui->name->clear();
            ui->count->clear();
            ui->helium_price->clear();
            ui->spinBox->setValue(1);

            ui->checkBox->setChecked(false);
            CHECKBOX_SHOW
            ++row;
            save_flag = false;
        }
    };
}

void AddNewBaloons::on_delete_2_clicked()
{
    if (row == 1){
        QMessageBox::warning(this, "Ошибка", "Отсутсвуют элементы для удаления.");
    }
    else if (!ui->tableWidget->isItemSelected(ui->tableWidget->currentItem())){
        QMessageBox::warning(this, "Ошибка", "Невозможно удалить - нет выбранных элементов.");
    } else if (ui->tableWidget->currentRow() == 0){
        return;
    } else {
        if (to_data.count(NameInRow) == 1)
        {
            to_data.erase(NameInRow);
        }
        to_delete.push_back(NameInRow);
        to_delete_price.push_back(for_baloons.at(NameInRow));

        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
        --row;
        save_flag = false;
    }
}

void AddNewBaloons::on_save_clicked()
{
    for (const auto& x : for_baloons){
        data->ChangeCost(x.second, Items::BALOONS);
        qDebug() << "I add to baloons price =" << x;
    }
    data->AddNewBaloons(std::move(to_data));
    save_flag = true;
    to_data.clear(); // just in case
    for_baloons.clear();

    for (size_t i = 0; i < to_delete.size(); ++i){
        data->SetCost(to_delete_price[i], Items::BALOONS);
        qDebug() << "I del to baloons price =" << to_delete_price[i];
        data->DeleteBaloon(to_delete[i]);
    }
    to_delete.clear();
    to_delete_price.clear();
    ui->status_bar->show();
    QTimer::singleShot(2 * 1000, ui->status_bar, SLOT(hide()));

    /* Here I have two ways:
     *  1) I can clear table after Saving, and the user can't interact with it.
     *      But this logic is rather strange, although it saves from unnecessary problems.
     *  2) I don't clear table. However if the user wants to delete thi item after Saving,
     *      I wil have to delete it also in database.
     * I will try to implement the second way.
     */

    qDebug() << "Hey, I am from AddNewBaloons and I've just save changes!";
}

void AddNewBaloons::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked()){
        ui->helium_price->hide();
        ui->label_12->hide();
        ui->label_8->hide();
    } else {
        CHECKBOX_SHOW
    }
}
