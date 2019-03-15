#include "order.h"
#include "ui_order.h"
#include <QMessageBox>
#include <QTimer>
#include <algorithm>

#define P_B tmp.push_back
#define FLAGS TextEditingFlag = true;     \
              ui->flag->show();


#define PRICE ui->table->item(i, 2)->text().toInt()
#define PRICE1 ui->table->item(i, 3)->text().toDouble()
#define COUNT ui->table->item(i, 1)->text().toInt()
#define NAME ui->list->item(ui->list->currentRow(), 0)->text()
#define PROCEEDS1 data->GetMap().at(NAME)[1] * ui->count->text().toInt()
#define TAXII ui->TotalCost_2->text().toInt()
#define ADDITIONAL_EXPENSES ui->AdditionalExpenses->text().toInt()

Order::Order(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::Order),
    data(db), row(1), list_row(1), wid(nullptr),
    TextEditingFlag(true), TextEditingFlag2(true),
    AdditionalExpenses(true), discount(1)
{
    ui->setupUi(this);

    keyEsc = new QShortcut(this);
    keyEsc->setKey(Qt::Key_Escape);
    connect(keyEsc, SIGNAL(activated()), this, SLOT(on_exit_without_saving_clicked())); // Esc

    keySpace = new QShortcut(this);
    keySpace->setKey(Qt::Key_Space);
    connect(keySpace, SIGNAL(activated()), this, SLOT(on_save_and_exit_clicked()));     // Enter


    // For first table:
    ui->list->setRowCount(static_cast<int>(data->GetMap().size()) + 1);     // Set actuall size
    for (const auto& x : data->GetMap()){                                   // Filling items
        wid = new QTableWidgetItem(x.first); // name
        ui->list->setItem(list_row, 0, wid);

        wid = new QTableWidgetItem(QString::number(x.second[0])); // count
        ui->list->setItem(list_row, 1, wid);
        ui->list->item(list_row, 1)->setTextAlignment(Qt::AlignCenter);
        ++list_row;
    }


    ui->dateEdit->setDate(QDate::currentDate());                            // To set currentDate
    ui->dateEdit->hide();                                                   // For a DataCheckBox

    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(ChangePrice()));
    timer->start(25);                                                       // Every 25 milliseconds

    // Create a form of hints:
    ui->hint->setToolTip("В любой момент Вы можете подкорректировать цену.");
    ui->checkBox->setToolTip("В данной версии программы\n"
                             "это никак не будет влиять на стоимость товара.\n"
                             "Это сделано для дополнительной информации заказа.");
    ui->comment->setToolTip("Здесь можно указать адресс доставки.");
    ui->hint_2->setToolTip("Данная цена добавляется в общую стоимость\n"
                           "заказа. И никак не влияет на выручку.");
    ui->hint_3->setToolTip("Данная цена добавляется в общую стоимость\n"
                           "заказа и в выручку.");
    ui->spinBox->setToolTip("Скидка не распростроняется на доставку. Только на шары.");
}

Order::~Order()
{
    delete ui;
}

void Order::ChangePrice(){  // Yeah, it is really cool, I can change the field in a real-time mode!
    // For price
    if (!ui->count->text().isEmpty() && !ui->flag->isHidden() &&
                                         ui->list->currentItem()->isSelected() &&
                                         ui->list->currentRow() != 0){
         ui->price->setText(QString::number(
                                            data->GetMap().at(NAME)[2] *
                                            ui->count->text().toInt()
                                                                       ));
    }

    // For TotalCost
    double total_cost = 0, not_selling_price = 0;
    for (int i = 1; i < row; ++i){
        total_cost += PRICE;
        not_selling_price += PRICE1;
    }
    total_cost *= discount;     // discount doesn't apply to shipping
    ui->TotalCost->setText(QString::number(total_cost + TAXII + ADDITIONAL_EXPENSES));
    ui->lineEdit->setText(QString::number(total_cost * 1.0 - not_selling_price + ADDITIONAL_EXPENSES));
}

void Order::on_add_clicked()
{
    if (!ui->list->currentItem()->isSelected() || ui->list->currentRow() == 0){
        QMessageBox::warning(this, "Ошибка", "Вы забыли выбрать позицию.");
    } else if (ui->count->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы забыли ввести количество.");
    } else if (data->GetMap().at(NAME)[0] < ui->count->text().toInt()){
        QMessageBox::warning(this, "Ошибка", "В наличие нет такого большого количества шаров.");
    } else if (ui->price->text().toInt() <= 0){
        QMessageBox::warning(this, "Ошибка", "Введите корректную цену.");
    } else {
        std::vector<QString> tmp;
        P_B(NAME);                                          // name     [0]
        P_B(ui->count->text());                             // count    [1]
        P_B(QString::number(PROCEEDS1));                    // revenue  [2]
        P_B(ui->checkBox->isChecked() ? "yes" : "no");      // helium   [3]
        to_data.push_back(std::move(tmp));

        wid = new QTableWidgetItem(NAME); // name
        ui->table->setItem(row, 0, wid);

        wid = new QTableWidgetItem(ui->count->text()); // count
        ui->table->setItem(row, 1, wid);
        ui->table->item(row, 1)->setTextAlignment(Qt::AlignCenter);

        wid = new QTableWidgetItem(ui->price->text()); // selling price
        ui->table->setItem(row, 2, wid);
        ui->table->item(row, 2)->setTextAlignment(Qt::AlignCenter);

        wid = new QTableWidgetItem(QString::number(PROCEEDS1)); // cost price
        ui->table->setItem(row, 3, wid);
        ui->table->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        wid = new QTableWidgetItem(ui->checkBox->isChecked() ? "С гелием" : "Без гелия"); // helium
        ui->table->setItem(row, 4, wid);
        ui->table->item(row, 4)->setTextAlignment(Qt::AlignCenter);
        ui->table->item(row, 4)->setIcon(ui->checkBox->isChecked() ?
                                             QPixmap(":/new/prefix1/icons/Allowed.svg_.png") :
                                             QPixmap(":/new/prefix1/icons/not.png"));

        ui->count->clear();
        ui->price->clear();
        ui->checkBox->setChecked(true);

        FLAGS
        ++row;
    }
}

void Order::on_exit_without_saving_clicked()
{
    close();
}

void Order::on_del_clicked()
{
    if (row == 1){
        QMessageBox::warning(this, "Ошибка", "Таблица уже пустая.");
    }
    else if (!ui->table->isItemSelected(ui->table->currentItem())){
        QMessageBox::warning(this, "Ошибка", "Отсутсвуют позиции для удаления.");
    } else if (ui->table->currentRow() == 0){
        return;
    } else {
        to_data.erase(to_data.begin() + ui->table->currentRow() - 1);
        ui->table->removeRow(ui->table->currentRow());

        FLAGS
        --row;
    }
}

void Order::on_number_textEdited(const QString &arg1) // For telephone number
{
    if (arg1.isEmpty()){} // I don't need this parametr
    if (TextEditingFlag){
        ui->number->clear();
        TextEditingFlag = false;
    }
}

void Order::on_TotalCost_2_textChanged(const QString &arg1) // For Taxi
{
    if (arg1.isEmpty()){} // I don't need this parametr
    if (TextEditingFlag2){
        ui->TotalCost_2->clear();
        TextEditingFlag2 = false;
    }
}

void Order::on_AdditionalExpenses_textChanged(const QString &arg1) //For AdditionalExpenses
{
    if (arg1.isEmpty()){} // I don't need this parametr
    if (AdditionalExpenses){
        ui->AdditionalExpenses->clear();
        AdditionalExpenses = false;
    }
}

void Order::on_price_textEdited(const QString &arg1) // For TotalCost
{
    if (arg1.isEmpty()){} // I don't need this parametr
    ui->flag->hide();
}

bool Order::CheckCountInTable(){
    std::map<QString, int> tmp;
    for (int i = 1; i < row; ++i){
        tmp[ui->table->item(i, 0)->text()] += COUNT;
    }
    for (const auto& x : tmp){
        if(!data->CheckBaloonsCount(x.first, x.second)){
            QMessageBox::critical(this, "Ошибка", "Количество шаров \"" + x.first + "\" в Вашей базе всего " +
                                  QString::number(static_cast<int>(data->GetCount(x.first))) +
                                  "шт, а Вы пытаетесь продать " + QString::number(x.second) + "шт.");
            return false;
        }
    }
    return true;
}

QDate Order::GetCurrenDate(){
    QDate CurrentDate;
    if (ui->dateEdit->isHidden()){
        CurrentDate = QDate::currentDate();
    } else {
        CurrentDate = ui->dateEdit->date();
    }
    return CurrentDate;
}

QString Order::GetStatus(){
    if (GetCurrenDate() < QDate::currentDate()){
        return "YES";
    } else {
        return "PROC";

    }
}

int Order::GetBaloonsTotalAmount(){
    int num = 0;
    for (const auto& x : to_data){
         num += x[1].toInt();
    }
    qDebug() << "NUM = " << num;
    return num;
}

void Order::on_save_and_exit_clicked()
{
   if (row == 1){
        QMessageBox::StandardButton quest =
                QMessageBox::question(this, "Вопрос", "Заказ пустой, выйти?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (quest == QMessageBox::Yes){
            close();
            return;
        } else {
            return;
        }
   }
   if (ui->lineEdit->text().toInt() < 0){
       QMessageBox::StandardButton quest =
               QMessageBox::question(this, "Вопрос", "Ваша выручка отрицательная. Продолжить?",
                                     QMessageBox::Yes | QMessageBox::No);
       if (quest == QMessageBox::No){
           return;
       }
   }
   if (!CheckCountInTable())
       return;
   std::vector<QString> tmp;
   int b_count = 0;
   if (GetStatus() == "YES"){
       data->ChangeCost(static_cast<int>(ui->lineEdit->text().toDouble()), Items::PROCEEDS);         // Add proceeds
       data->ChangeCost(TAXII, Items::TAXI);                                    // Add Taxi
       data->ChangeCost(ADDITIONAL_EXPENSES, Items::OTHER);                     // Add Other
       data->ChangeCost(ui->TotalCost->text().toInt(), Items::TOTALORDERS);     // Add Total Cost
       data->ChangeCost(GetBaloonsTotalAmount(), Items::BALOONSTOTALAMOUNT);    // Add Baloons Total Count
       data->PlusToSocialNetworksMap(ui->comboBox->currentText());              // Social Networks
       data->PlusStatusOfOrders(Status::YES);                                   // ++Status YES
       data->PlusNumbesrOfOrders(ui->dateEdit->date());                         // ++ Order count
       qDebug() << "Твой заказ уже завершён, я доабвлю его в" << ui->dateEdit->date();
   } else {
       data->PlusStatusOfOrders(Status::PROC);
   }
   for (const auto& x : to_data){
        data->DellCount(x[0], x[1].toInt());                // Update baloon's count
        b_count += x[1].toInt();
   }

   // Adding the whole information about the order:
   P_B(QString::number(b_count));               // Total amount         [0]
   P_B(ui->TotalCost->text());                  // Total cost           [1]
   P_B(ui->lineEdit->text());                   // Total proceeds       [2]
   P_B(ui->number->text());                     // Telephone number     [3]
   P_B(ui->comment->document()->toPlainText()); // Comment              [4]
   P_B(ui->comboBox->currentText());            // Social Networks      [5]
   P_B(ui->TotalCost_2->text());                // Taxi                 [6]
   P_B(ui->AdditionalExpenses->text());         // Other                [7]
   P_B(GetStatus());                            // Status               [8]

   to_data.push_back(std::move(tmp));
   data->AddToSet(GetCurrenDate());
   data->AddToOrder(GetCurrenDate(), std::move(to_data));
   close();
}



void Order::on_checkBox_2_clicked()
{
    if (ui->checkBox_2->isChecked()){
        ui->dateEdit->hide();
    } else {
        ui->dateEdit->show();
    }
}


void Order::on_spinBox_valueChanged(int arg1)
{
    qDebug() << "Before = " << discount;
    qDebug() << "After disc =" << (static_cast<double>(arg1) / 100);
    discount = 1 - (static_cast<double>(arg1) / 100);
     qDebug() << discount;
}
