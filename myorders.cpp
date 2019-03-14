#include "myorders.h"
#include "ui_myorders.h"
#include <QDebug>

#define CHECK data->GetSet().lower_bound(QDate::currentDate()) != data->GetSet().end()

MyOrders::MyOrders(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::MyOrders),
    data(db), current_position(0), row(1), wid(nullptr)
{
    ui->setupUi(this);

    keyLeft = new QShortcut(this);
    keyLeft->setKey(Qt::Key_Left);
    connect(keyLeft, SIGNAL(activated()), this, SLOT(on_left_clicked()));

    keyRight = new QShortcut(this);
    keyRight->setKey(Qt::Key_Right);
    connect(keyRight, SIGNAL(activated()), this, SLOT(on_right_clicked()));

    SetIterators(); // And current position
    DisplayOrder();

    ui->pushButton->setToolTip("При удалении заказа, все записи о нём стираются,\n"
                               "будто его и не было. Все шары, которые были в заказе, \n"
                               "возвращаются обратно. Поэтому будте внимательнее, при удалении\n"
                               "уже завершённых заказов. Это может привести к неправильному отображению\n"
                               "в базе данных!");

}

void MyOrders::SetIterators(){
    current_position = 0;
    if (data->GetSet().size() != 0){
        if (CHECK){
            qDebug() << "It is a SetIterators and the 1 condition";
            set_iterator = data->GetSet().lower_bound(QDate::currentDate());
        } else {
            qDebug() << "It is a SetIterators and the 2 condition";
            set_iterator = prev(data->GetSet().end());
        }
        for (auto it = data->GetOrder().begin();it != data->GetOrder().end(); ++it){
            ++current_position;
            if (it->first == *set_iterator){
                map_iterator = it;
                break;
            }
        }
    }
}

void MyOrders::ClearTable(){
    ui->table->clear();

    wid = new QTableWidgetItem("Название");
    ui->table->setItem(0, 0, wid);
    ui->table->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    ui->table->item(0, 0)->setIcon(QPixmap(":/new/prefix1/icons/title.png"));

    wid = new QTableWidgetItem("Гелий");
    ui->table->setItem(0, 1, wid);
    ui->table->item(0, 1)->setTextAlignment(Qt::AlignCenter);
    ui->table->item(0, 1)->setIcon(QPixmap(":/new/prefix1/icons/8.png"));

    wid = new QTableWidgetItem("Количество");
    ui->table->setItem(0, 2, wid);
    ui->table->item(0, 2)->setTextAlignment(Qt::AlignCenter);
    ui->table->item(0, 2)->setIcon(QPixmap(":/new/prefix1/icons/30162724-ed8b-4a46-93ca-37c4d4ab8d45.png"));
}

void MyOrders::DisplayCurentDate(){
    if (data->GetSet().size() != 0){
        qDebug() << "This is first condition," << "count of orders is" << data->GetOrder().size();
        qDebug() << (map_iterator->first).toString("dd/MM/yy");

        ui->dateEdit->setDate(map_iterator->first);
    } else {
        qDebug() << "This is second condition";
        ui->dateEdit->setDate(QDate::currentDate());
    }
}

void MyOrders::SetCountOfActiveOrders(){
    ui->active->setText(QString::number(data->GetCountOfOrderStatus(Status::PROC)));
}

void MyOrders::DisplayCurentPosition(){
    ui->active_3->setText(QString::number(current_position) +
                          "/" + QString::number(data->GetSet().size()));
}

void MyOrders::DisplayOrder(){
    DisplayCurentDate();
    ui->active_2->setText(QString::number(data->GetOrder().size()));        // Set count of all orders
    SetCountOfActiveOrders();
    DisplayCurentPosition();

    if (data->GetSet().size() != 0) { // DisplayOrder
        qDebug() << current_position;
        My_vector tmp = map_iterator->second;

        // For table Widget:
        ClearTable();
        ui->table->setRowCount((9 >= tmp.size()) ?                          // It's for current row's count
                                            9 : static_cast<int>(tmp.size()));

        for (size_t i = 0; i + 1 < tmp.size(); ++i){
            wid = new QTableWidgetItem(tmp[i][0]); // name
            ui->table->setItem(row, 0, wid);

            wid = new QTableWidgetItem(tmp[i][1]); // count
            ui->table->setItem(row, 2, wid);
            ui->table->item(row, 2)->setTextAlignment(Qt::AlignCenter);

            wid = new QTableWidgetItem((tmp[i][3] == "yes") ? "С гелием" : "Без гелия"); // helium
            ui->table->setItem(row, 1, wid);
            ui->table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            ui->table->item(row, 1)->setIcon((tmp[i][3] == "yes") ?
                                                QPixmap(":/new/prefix1/icons/Allowed.svg_.png") :
                                                QPixmap(":/new/prefix1/icons/not.png"));
            ++row;
        }
        row = 1;

        // For status:
        if (tmp[tmp.size() - 1][8] == "YES"){
            qDebug() << "YES";
            ui->ComboBoxStatus->setCurrentIndex(0);
            ui->PBcomplete->setEnabled(false);
            ui->PBcancel->setEnabled(false);
        } else if (tmp[tmp.size() - 1][8] == "PROC"){
            qDebug() << "PROC";
            ui->ComboBoxStatus->setCurrentIndex(1);
            ui->PBcomplete->setEnabled(true);
            ui->PBcancel->setEnabled(true);
        } else {
            qDebug() << "NO";
            ui->ComboBoxStatus->setCurrentIndex(2);
            ui->PBcomplete->setEnabled(false);
            ui->PBcancel->setEnabled(false);
        }

        ui->total_price->setText(tmp[tmp.size() - 1][1]);                   // For Total price
        ui->count->setText(tmp[tmp.size() - 1][0]);                         // For Total count
        ui->proceed->setText(tmp[tmp.size() - 1][2]);                       // For Total proceed
        ui->number->setText(tmp[tmp.size() - 1][3]);                        // For telephone number
        ui->comments->setText(tmp[tmp.size() - 1][4]);                      // For comments
        AddSocial(tmp);                                                     // For social networks
        ui->active_4->setText(tmp[tmp.size() - 1][6]);                      // For Taxi
        ui->active_5->setText(tmp[tmp.size() - 1][7]);
    } else {
        for (int i = 1; i <= ui->table->rowCount(); ++i){                   // To clear table
            ui->table->removeRow(i);
        }
    }
}

void MyOrders::AddSocial(const My_vector& tmp){
    if(tmp[tmp.size() - 1][5] == "Instagram"){
        ui->ComboBoxSocial->setCurrentIndex(0);
    } else if (tmp[tmp.size() - 1][5] == "WhatsApp"){
        ui->ComboBoxSocial->setCurrentIndex(1);
    } else if (tmp[tmp.size() - 1][5] == "Сайт"){
        ui->ComboBoxSocial->setCurrentIndex(2);
    } else if (tmp[tmp.size() - 1][5] == "Знакомые"){
        ui->ComboBoxSocial->setCurrentIndex(3);
    } else if (tmp[tmp.size() - 1][5] == "VK"){
        ui->ComboBoxSocial->setCurrentIndex(4);
    } else {
        ui->ComboBoxSocial->setCurrentIndex(5);
    }
}

MyOrders::~MyOrders()
{
    delete ui;
}

void MyOrders::on_right_clicked()
{
    if (data->GetSet().size() != 0){
        if (map_iterator != prev(data->GetOrder().end())){
            ++current_position;
            ++set_iterator;
            ++map_iterator;
            DisplayOrder();
        }
    }
}

void MyOrders::on_left_clicked()
{
    if (data->GetSet().size() != 0){
        if (map_iterator != data->GetOrder().begin()){
            --current_position;
            --set_iterator;
            --map_iterator;
            DisplayOrder();
        }
    }
}

void MyOrders::on_PBgo_away_clicked()
{
    close();
}

void MyOrders::ReturnBaloonsInDataBase(){
    for (size_t i = 0; i < map_iterator->second.size() - 1; ++i){
        qDebug() << "I return" << i << "baloon";
        data->ReturnCountInMap(ui->table->item(i + 1, 0)->text(), ui->table->item(i + 1, 2)->text().toInt());
    }
}

void MyOrders::on_PBcancel_clicked() // to NO
{
    if (data->GetOrder().size() == 0){
        QMessageBox::warning(this, "Ошибка", "У Вас пока нет заказов.");
    } else {
        if (map_iterator->second[map_iterator->second.size() - 1][8] == "PROC"){
            data->ChangeOrderStatus(map_iterator, "NO");
            data->MinusStatusOfOrders(Status::PROC);
            data->PlusStatusOfOrders(Status::NO);
            ReturnBaloonsInDataBase();
            DisplayOrder();
        } else {
            QMessageBox::warning(this, "Ошибка", "Отменить можно только активный заказ.");
        }
    }
}

void MyOrders::on_PBcomplete_clicked() // to YES
{
    if (data->GetOrder().size() == 0){
        QMessageBox::warning(this, "Ошибка", "У Вас пока нет заказов.");
    } else {
        if (map_iterator->second[map_iterator->second.size() - 1][8] == "PROC"){
            data->ChangeOrderStatus(map_iterator, "YES");
            qDebug() << "(ui->proceed->text() = " << ui->proceed->text() <<
                     "ui->proceed->text().toInt()" <<static_cast<int>(ui->proceed->text().toDouble());
            data->ChangeCost(static_cast<int>(ui->proceed->text().toDouble()), Items::PROCEEDS);                 // Add proceeds
            data->ChangeCost(ui->active_5->text().toInt(), Items::TAXI);                    // Add Taxi
            data->ChangeCost(ui->active_5->text().toInt(), Items::OTHER);                   // Add Other
            data->ChangeCost(ui->total_price->text().toInt(), Items::TOTALORDERS);          // Add Total Price
            data->ChangeCost(ui->count->text().toInt(), Items::BALOONSTOTALAMOUNT);         // Add Baloons Total count
            data->MinusStatusOfOrders(Status::PROC);
            data->PlusStatusOfOrders(Status::YES);
            data->PlusToSocialNetworksMap(ui->ComboBoxSocial->currentText());
            data->PlusNumbesrOfOrders(ui->dateEdit->date());
            DisplayOrder();
        } else {
            QMessageBox::warning(this, "Ошибка", "Завершить можно только активный заказ.");
        }
    }
}

void MyOrders::on_pushButton_clicked()  // Delete
{
    if (data->GetOrder().size() == 0){
        QMessageBox::warning(this, "Ошибка", "У Вас пока нет заказов.");
    } else {
        QMessageBox::StandardButton quest =
                QMessageBox::question(this, "Вопрос", "Вы уверены, что хотите безвозвратно удалить данный заказ?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (quest == QMessageBox::Yes){
            if (map_iterator->second[map_iterator->second.size() - 1][8] == "PROC"){
                data->MinusStatusOfOrders(Status::PROC);
                ReturnBaloonsInDataBase();
                data->SetCost(static_cast<int>(ui->proceed->text().toDouble()), Items::PROCEEDS);
            } else if(map_iterator->second[map_iterator->second.size() - 1][8] == "NO"){
                data->MinusStatusOfOrders(Status::NO);
            } else if(map_iterator->second[map_iterator->second.size() - 1][8] == "YES"){
                data->MinusStatusOfOrders(Status::YES);
                data->MinusNumbesrOfOrders(ui->dateEdit->date());
                data->SetCost(ui->total_price->text().toInt(), Items::TOTALORDERS);
                data->SetCost(ui->count->text().toInt(), Items::BALOONSTOTALAMOUNT);
            }
            qDebug() << "Current set to del is" << (*set_iterator);
            data->EraseOrderInSet(set_iterator);
            data->EraseOrder(map_iterator);
            data->MinusToSocialNetworksMap(map_iterator->second[map_iterator->second.size() - 1][5]);
            SetIterators();
            DisplayOrder();
        }
    }
}

void MyOrders::on_checkBoxDate_clicked()
{
    if (!ui->checkBoxDate->isChecked()){
        ui->dateEdit->setEnabled(false);
    } else {
        ui->dateEdit->setEnabled(true);
    }
}

void MyOrders::on_dateEdit_userDateChanged(const QDate &date)
{
    if (ui->dateEdit->isEnabled()){
        if(data->GetOrder().count(date) == 0){
            ui->active_3->clear();
            ui->total_price->clear();
            ui->proceed->clear();
            ui->number->clear();
            ui->comments->clear();
            ui->active_4->clear();
            ui->active_5->clear();
            ClearTable();
        } else {
            size_t count = 1;
            SuperMap::const_iterator tmp = data->GetOrder().begin();
            for(auto it = data->GetSet().begin(); it != data->GetSet().end(); ++it){
                if (*it == ui->dateEdit->date()){
                    set_iterator = it;
                    map_iterator = tmp;
                    qDebug() << "count of current_position = " << count;
                    current_position = count;
                    qDebug() << "Only user change date";
                    DisplayOrder();
                    break;
                }
                qDebug() << "count = " << count;
                ++tmp;
                ++count;
            }
        }
    }
}
