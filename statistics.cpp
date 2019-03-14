#include "statistics.h"
#include "ui_statistics.h"

#define NAME ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text()

Statistics::Statistics(QWidget *parent, Data db) :
    QDialog(parent),
    ui(new Ui::Statistics),
    data(db), maximum_count(0), user_date(QDate::currentDate()),
    max_count_of_orders(0)
{
    ui->setupUi(this);
    ui->dateEdit_2->setDate(QDate::currentDate());              // default
    GetMaximumCountOfSocialNetworks();
    CreateSocialBatChart();
    CreateOrderGraph();
    DisplayALL();

    // For second Window:
    DisplayTable();
    CreateAllTrueVector();
    DisplayFrame3();
    ui->pushButton_2->setToolTip("Они просто удалятся из базы данных.");
}

int Statistics::GetMaximumCountOfSocialNetworks(){
    for (const auto& x : data->GetSocialNetworksMap()){
        if (maximum_count < x.second){
            maximum_count = x.second;
        }
    }
    return maximum_count;
}

void Statistics::CreateSocialBatChart(){
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->customPlot->setBackground(QBrush(gradient));

    // create empty bar chart objects:
    QCPBars *net = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    net->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    // set colors:
    net->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    net->setBrush(QColor(0, 168, 140));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6;
    labels << "Instagram"  << "VK" << "WhatsApp" << "Другое" << "Знакомые" << "Сайт";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTicks(false);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->setRange(0, 7);
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, (6 > maximum_count ? 7 : maximum_count + 1));
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("Количество заказов");
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> NetData;
    for (const auto& x : data->GetSocialNetworksMap()){
        NetData << x.second;
    }
    net->setData(ticks, NetData);
}

void Statistics::CreateOrderGraph(){
    // prepare data:
    QVector<double> x1(13), y1(1000);
    QVector<double> x2(13), y2(1100);
    for (int i = 1; i < x1.size(); ++i)
    {
      x1[i] = i;
      y1[i] = data->GetNumberOdOrdersCurrentMonth(user_date.year(), i);;
    }
    for (int i = 1; i < x2.size(); ++i)
    {
      x2[i] = i;
      y2[i] = (data->GetNumberOdOrdersCurrentMonth(user_date.year(), i) == 0) ?  0 :
                                     data->GetNumberOdOrdersCurrentMonth(user_date.year(), i) + 2;
      if (y2[i] > max_count_of_orders){
          max_count_of_orders = static_cast<int>(y2[i]);
      }
    }

    // create and configure plottables:
    QCPGraph *graph1 = ui->customPlot_2->addGraph();
    graph1->setData(x1, y1);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));

    QCPGraph *graph2 = ui->customPlot_2->addGraph();
    graph2->setData(x2, y2);
    graph2->setPen(Qt::NoPen);
    graph2->setBrush(QColor(200, 200, 200, 20));
    graph2->setChannelFillGraph(graph1);

    ui->customPlot_2->replot();         // To update

    // move bars above graphs and grid below bars:
    ui->customPlot_2->addLayer("abovemain", ui->customPlot_2->layer("main"), QCustomPlot::limAbove);
    ui->customPlot_2->addLayer("belowmain", ui->customPlot_2->layer("main"), QCustomPlot::limBelow);
    graph1->setLayer("abovemain");
    ui->customPlot_2->xAxis->grid()->setLayer("belowmain");
    ui->customPlot_2->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    ui->customPlot_2->yAxis->setLabel("Количество заказов");
    ui->customPlot_2->xAxis->setLabel("Месяц");
    ui->customPlot_2->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot_2->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot_2->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot_2->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot_2->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot_2->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot_2->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot_2->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot_2->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot_2->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot_2->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot_2->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot_2->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot_2->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot_2->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot_2->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot_2->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot_2->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->customPlot_2->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->customPlot_2->axisRect()->setBackground(axisRectGradient);
    ui->customPlot_2->xAxis->setLabelColor(Qt::white);
    ui->customPlot_2->yAxis->setLabelColor(Qt::white);

    ui->customPlot_2->rescaleAxes();
    ui->customPlot_2->yAxis->setRange(0, max_count_of_orders + 1);
    ui->customPlot_2->xAxis->setRange(1, 12);
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked()){
        ui->dateEdit_2->setEnabled(true);
    } else {
        ui->dateEdit_2->setEnabled(false);
    }
}

void Statistics::ClearGraph(){
    for (int i = 0; i < ui->customPlot_2->graphCount(); ++i){   // For clear graph
        ui->customPlot_2->graph(i)->data().data()->clear();
    }
}

void Statistics::on_dateEdit_2_userDateChanged(const QDate &date)
{
        user_date = ui->dateEdit_2->date();
        ClearGraph();
        CreateOrderGraph();

}


void Statistics::DisplayALL(){
    ui->lineEdit_33->setText(QString::number(data->GetOrder().size()));                     // All Orders
    ui->lineEdit_40->setText(QString::number(data->GetStatusOfOrders()[2]));                // Active
    ui->lineEdit_31->setText(QString::number(data->GetStatusOfOrders()[0]));                // YES
    ui->lineEdit_32->setText(QString::number(data->GetStatusOfOrders()[1]));                // NO

    ui->lineEdit_38->setText(QString::number(data->GetCost(Items::BALOONSTOTALAMOUNT)));    // Total Baloon's Amount

    ui->lineEdit_39->setText(QString::number(data->GetCost(Items::OTHER) +                  // All cost
                                             data->GetCost(Items::HELIUM) +
                                             data->GetCost(Items::BALOONS)));

     ui->lineEdit_34->setText(QString::number(data->GetCost(Items::BALOONS)));              // Baloons
     ui->lineEdit_35->setText(QString::number(data->GetCost(Items::HELIUM)));               // Helium
     ui->lineEdit_36->setText(QString::number(data->GetCost(Items::OTHER)));                // Other
     ui->lineEdit_37->setText(QString::number(data->GetCost(Items::PROCEEDS)));             // Proceeds

     std::optional<QString> most_popular_baloon = data->GetMostPopularBaloon();             // Most popular baloon
     if (most_popular_baloon){
         ui->lineEdit_43->setText(*most_popular_baloon);
     } else {
         ui->lineEdit_43->setText("Пока нет такого шара");
     }

     if (data->GetStatusOfOrders()[0] != 0){                                        // Midle proceeds
         ui->lineEdit_41->setText(QString::number(data->GetCost(Items::PROCEEDS) * 1.0 /
                                                   data->GetStatusOfOrders()[0]));
     } else {
          ui->lineEdit_41->setText("0");
     }

         ui->lineEdit_42->setText(QString::number(data->GetCost(Items::TOTALORDERS)));
}

//**************************************************************************************************//
//****************************** It's for second window  *******************************************//
//*****************************************|  |*****************************************************//
//*****************************************|  |*****************************************************//
//****************************************\    /****************************************************//
//*****************************************\  /*****************************************************//
//******************************************\/******************************************************//
//**************************************************************************************************//

void Statistics::DisplayTable(){

    ui->tableWidget->setRowCount(static_cast<int>(data->GetMap().size()) + 1);
    int row = 1;
    for (const auto& x : data->GetMap()){

        wid = new QTableWidgetItem(x.first); // name
        ui->tableWidget->setItem(row, 0, wid);

        wid = new QTableWidgetItem(QString::number(x.second[0])); // count
        ui->tableWidget->setItem(row, 1, wid);
        ui->tableWidget->item(row, 1)->setTextAlignment(Qt::AlignCenter);

        wid = new QTableWidgetItem(QString::number(x.second[1])); // cost price
        ui->tableWidget->setItem(row, 2, wid);
        ui->tableWidget->item(row, 2)->setTextAlignment(Qt::AlignCenter);

        wid = new QTableWidgetItem(QString::number(x.second[2])); // selling price
        ui->tableWidget->setItem(row, 3, wid);
        ui->tableWidget->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        ++row;
    }


}

void Statistics::DisplayFrame3(){
    ui->lineEdit_9->setText(QString::number(data->GetCost(Items::BALOONS)));            // Baloons
    ui->lineEdit_10->setText(QString::number(data->GetCost(Items::HELIUM)));            // Helium
    ui->lineEdit_11->setText(QString::number(data->GetCost(Items::OTHER)));             // Other
    ui->lineEdit_8->setText(QString::number(data->GetCost(Items::PROCEEDS)));           // Total Proceed
}

void Statistics::CreateAllTrueVector(){
    flags.reserve(7);
    for (size_t i = 0; i < 7; ++i){
        flags[i] = true;
    }
}

void Statistics::on_pushButton_clicked()
{
    Hint hint(this);
    hint.setModal(true);
    hint.exec();
}

void Statistics::SetEnableForRadoiButton(bool flag){
    ui->lineEdit_2->setEnabled(flag);
    ui->lineEdit_3->setEnabled(flag);
    ui->lineEdit_4->setEnabled(flag);
}

void Statistics::on_checkBox_2_clicked()
{
    if (ui->checkBox_2->isChecked()){
        ui->lineEdit_8->setEnabled(true);
    } else{
        ui->lineEdit_8->setEnabled(false);
    }
}

void Statistics::on_checkBox_3_clicked()
{
    if (ui->checkBox_3->isChecked()){
        ui->lineEdit_9->setEnabled(true);
    } else{
        ui->lineEdit_9->setEnabled(false);
    }
}

void Statistics::on_checkBox_4_clicked()
{
    if (ui->checkBox_4->isChecked()){
        ui->lineEdit_10->setEnabled(true);
    } else{
        ui->lineEdit_10->setEnabled(false);
    }
}

void Statistics::on_checkBox_5_clicked()
{
    if (ui->checkBox_5->isChecked()){
        ui->lineEdit_11->setEnabled(true);
    } else{
        ui->lineEdit_11->setEnabled(false);
    }
}

void Statistics::on_pushButton_3_clicked()
{
    ui->frame->setEnabled(true);
    ui->lineEdit->setEnabled(false);
    ui->radioButton_2->setChecked(true);
    SetEnableForRadoiButton(true);
}

void Statistics::on_pushButton_4_clicked()
{
    ui->frame_2->setEnabled(true);
}

void Statistics::on_radioButton_toggled(bool checked)
{
    if (checked) {} // unused parametr
    if(ui->radioButton->isChecked()){
        ui->lineEdit->setEnabled(true);
    } else {
        ui->lineEdit->setEnabled(false);
    }
    if (ui->radioButton_2->isChecked()){
        SetEnableForRadoiButton(true);
    } else {
        SetEnableForRadoiButton(false);
    }
}


void Statistics::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_2->isEnabled() && flags[0] == true){
        ui->lineEdit_2->clear();
        flags[0] = false;
    }
}

void Statistics::on_lineEdit_4_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_4->isEnabled() && flags[1] == true){
        ui->lineEdit_4->clear();
        flags[1] = false;
    }
}

void Statistics::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_3->isEnabled() && flags[2] == true){
        ui->lineEdit_3->clear();
        flags[2] = false;
    }
}

void Statistics::on_lineEdit_8_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_8->isEnabled() && flags[3] == true){
        ui->lineEdit_8->clear();
        flags[3] = false;
    }
}

void Statistics::on_lineEdit_9_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_9->isEnabled() && flags[4] == true){
        ui->lineEdit_9->clear();
        flags[4] = false;
    }
}

void Statistics::on_lineEdit_10_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_10->isEnabled() && flags[5] == true){
        ui->lineEdit_10->clear();
        flags[5] = false;
    }
}

void Statistics::on_lineEdit_11_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()){} // unused parametr
    if (ui->lineEdit_11->isEnabled() && flags[6] == true){
        ui->lineEdit_11->clear();
        flags[6] = false;
    }
}

void Statistics::on_pushButton_6_clicked()  // Frame 2
{
    if (!(ui->tableWidget->currentRow() > 0 && ui->tableWidget->currentRow() < ui->tableWidget->rowCount())){
        QMessageBox::warning(this, "Ошибка", "Вы забыли выбрать позицию.");
    } else if (ui->lineEdit_5->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы забыли ввести количество.");
    } else if (ui->lineEdit_6->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы забыли ввести себестоимость.");
    } else if (ui->lineEdit_5->text().toInt() <= 0){
        QMessageBox::warning(this, "Ошибка", "Количество не может быть отрицательным или равным нулю.");
    } else if (data->GetMap().at(NAME)[0] < ui->lineEdit_5->text().toInt()){
        QMessageBox::warning(this, "Ошибка", "В наличие нет такого количества шаров.");
    } else if (ui->lineEdit_6->text().toInt() <= 0){
        QMessageBox::warning(this, "Ошибка", "Введите корректную цену.");
    } else if (ui->lineEdit_6->text().toInt() >= 500){
        QMessageBox::warning(this, "Ошибка", "Введите корректную цену.");
    } else {
        data->SetCost(ui->lineEdit_6->text().toInt(), Items::PROCEEDS);
        data->MinusBaloonsCharacteristics(NAME, ui->lineEdit_5->text().toDouble(), BaloonChar::COUNT);
        ui->lineEdit_5->clear();
        ui->lineEdit_6->clear();
        DisplayTable();
    }
}

bool Statistics::CheckIntEnter(){
    qDebug() << ui->lineEdit_2->text();
    if (ui->lineEdit_2->text().toInt() == 0 && ui->lineEdit_2->text() != "Себестоимость" &&
                                                 ui->lineEdit_2->text() != "0"){
         QMessageBox::warning(this, "Ошибка", "Неккоректна заполнена себестоимость.");
         return false;
     } else if (ui->lineEdit_4->text().toInt() == 0 && ui->lineEdit_4->text() != "Розничная цена" &&
                                                  ui->lineEdit_4->text() != "0"){
         QMessageBox::warning(this, "Ошибка", "Неккоректна заполнена розничная цена.");
         return false;
     } else if (ui->lineEdit_3->text().toInt() == 0 && ui->lineEdit_3->text() != "Количество" &&
                                                  ui->lineEdit_3->text() != "0"){
         QMessageBox::warning(this, "Ошибка", "Неккоректна заполнена количество.");
         return false;
     }
    return true;
}

void Statistics::on_pushButton_5_clicked()  // Frame 1
{
    if (!(ui->tableWidget->currentRow() > 0 && ui->tableWidget->currentRow() < ui->tableWidget->rowCount())){
        QMessageBox::warning(this, "Ошибка", "Вы забыли выбрать позицию.");
        return;
    }
    if (ui->radioButton->isChecked() && ui->lineEdit->isEnabled()){                 // Set new name
        if (ui->lineEdit->text().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Название не может быть пустым.");
            return;
        } else if (data->GetMap().count(ui->lineEdit->text())){
            QMessageBox::warning(this, "Ошибка", "Шар с таким именем уже есть.");
            return;
        }
        std::vector<double> tmp = data->GetMap().at(NAME);
        data->DeleteBaloon(NAME);
        data->AddNewBaloon2(ui->lineEdit->text(), std::move(tmp));
        ui->lineEdit->clear();
    }
    if (ui->radioButton_2->isChecked() && ui->lineEdit_2->isEnabled()){             // Set new parametres

        flags[0] = flags[1] = flags[2] = false;     // it's really matter
        if (ui->lineEdit_2->text().isEmpty() || ui->lineEdit_2->text() == "Себестоимость"){
            ui->lineEdit_2->setText(ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->text());
        }
        if (ui->lineEdit_4->text().isEmpty() || ui->lineEdit_4->text() == "Розничная цена"){
            ui->lineEdit_4->setText(ui->tableWidget->item(ui->tableWidget->currentRow(), 3)->text());
        }
        if (ui->lineEdit_3->text().isEmpty() || ui->lineEdit_3->text() == "Количество"){
            ui->lineEdit_3->setText(ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text());
        }

        if (!CheckIntEnter()){
            flags[0] = flags[1] = flags[2] = true;
            return;
        }

        // the count or the price couldn't be negative:
        if (ui->lineEdit_3->text().toDouble() < 0 ||
                ui->lineEdit_2->text().toDouble() < 0 ||
                    ui->lineEdit_4->text().toDouble() < 0){
            flags[0] = flags[1] = flags[2] = true;
            QMessageBox::warning(this, "Ошибка", "Введите корректные данные.");
            return;
        }

        // Change characteristics
        data->SetBaloonsCharacteristics(NAME, ui->lineEdit_2->text().toDouble(), BaloonChar::COST_PRICE);
        data->SetBaloonsCharacteristics(NAME, ui->lineEdit_4->text().toDouble(), BaloonChar::SELLING_PRICE);
        data->SetBaloonsCharacteristics(NAME, ui->lineEdit_3->text().toDouble(), BaloonChar::COUNT);

        // clear
        ui->lineEdit_2->setText("Себестоимость");
        ui->lineEdit_4->setText("Розничная цена");
        ui->lineEdit_3->setText("Количество");
        flags[0] = flags[1] = flags[2] = true;
    }
    DisplayTable();
}

void Statistics::ChangeFrame3(){
    if (ui->lineEdit_8->isEnabled()){                                           // Proceeds
        if (ui->lineEdit_8->text().toInt() == 0){
            QMessageBox::StandardButton quest =
                   QMessageBox::question(this, "Вопрос", "Вы точно хотите обнулить выручку?",
                                         QMessageBox::Yes | QMessageBox::No);
            if (quest == QMessageBox::Yes){
                data->NormalSetCost(ui->lineEdit_8->text().toInt(), Items::PROCEEDS);
            }
        } else {
            data->NormalSetCost(ui->lineEdit_8->text().toInt(), Items::PROCEEDS);
        }
    }
    if (ui->lineEdit_9->isEnabled()){                                           // Baloons
        if (ui->lineEdit_9->text().toInt() == 0){
            QMessageBox::StandardButton quest =
                   QMessageBox::question(this, "Вопрос", "Вы точно хотите обнулить позицию \"шары\"?",
                                         QMessageBox::Yes | QMessageBox::No);
            if (quest == QMessageBox::Yes){
                data->NormalSetCost(ui->lineEdit_9->text().toInt(), Items::BALOONS);
            }
        } else {
             data->NormalSetCost(ui->lineEdit_9->text().toInt(), Items::BALOONS);
        }
    }
    if (ui->lineEdit_10->isEnabled()){                                           // Helium
        if (ui->lineEdit_10->text().toInt() == 0){
            QMessageBox::StandardButton quest =
                   QMessageBox::question(this, "Вопрос", "Вы точно хотите обнулить позицию \"Гелий\"?",
                                         QMessageBox::Yes | QMessageBox::No);
            if (quest == QMessageBox::Yes){
                data->NormalSetCost(ui->lineEdit_10->text().toInt(), Items::HELIUM);
            }
        } else {
            data->NormalSetCost(ui->lineEdit_10->text().toInt(), Items::HELIUM);
        }
    }
    if (ui->lineEdit_11->isEnabled()){                                           // Other
        if (ui->lineEdit_11->text().toInt() == 0){
            QMessageBox::StandardButton quest =
                   QMessageBox::question(this, "Вопрос", "Вы точно хотите обнулить позицию \"Другое\"?",
                                         QMessageBox::Yes | QMessageBox::No);
            if (quest == QMessageBox::Yes){
                data->NormalSetCost(ui->lineEdit_11->text().toInt(), Items::OTHER);
            }
        } else {
            data->NormalSetCost(ui->lineEdit_11->text().toInt(), Items::OTHER);
        }
    }
}

void Statistics::on_pushButton_7_clicked() {  // Frame 3
    if (ui->lineEdit_8->text().toInt() < 0 || ui->lineEdit_9->text().toInt() < 0 ||
                ui->lineEdit_10->text().toInt() < 0 || ui->lineEdit_11->text().toInt() < 0){
         QMessageBox::warning(this, "Ошибка", "Введите положительные данные.");
    } else {
        ChangeFrame3();
        DisplayFrame3();
        DisplayALL();
    }
}

void Statistics::on_pushButton_2_clicked()  // Delete
{
    if (!(ui->tableWidget->currentRow() > 0 && ui->tableWidget->currentRow() < ui->tableWidget->rowCount())){
        QMessageBox::warning(this, "Ошибка", "Вы забыли выбрать позицию.");
    } else {
        data->DeleteBaloon(NAME);
        DisplayTable();
    }
}
