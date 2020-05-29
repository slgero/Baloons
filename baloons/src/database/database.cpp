#include "database.h"
#include <QMessageBox>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>
#include <QFileDialog>
#include <QDialog>
#include <QStringList>

#define UTF8 QObject::trUtf8

DataBase::DataBase() : db_status(3)
{
    CreateEmptyPurchaseStorage();
    CreateEmptySocialMap();
//    AddSomethingToMap();
}

void DataBase::SaveDataBase(){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile file(PATH);

    if(file.open(QIODevice::ReadWrite)){
        qDebug() << "SaveDATA I open file to save in" << PATH;
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        stream << "PATH" << "\r\n";
        stream <<  PATH << "\r\n";                                                        //PATH

        stream << "db_storage_of_purchase" << "\r\n";
        stream << db_storage_of_purchase.size() << "\r\n";                                // Baloon's Storage
        for (size_t i = 0; i < db_storage_of_purchase.size(); ++i){
            stream << db_storage_of_purchase[i] << "\r\n";
        }

        stream << "db_baloons" << "\r\n";
        stream << db_baloons.size() << "\r\n";                                            // Baloons properties
        for (const auto& x : db_baloons){
            stream << x.first << "\r\n";
            stream << x.second.size() << "\r\n";
            for (const auto& y : x.second){
                stream << y << "\r\n";
            }
        }

        // std::map<QString, int>
        stream << "db_social_networks" << "\r\n";                                          // Socian Networks
        stream << db_social_networks.size() << "\r\n";
        for (const auto& x : db_social_networks){
            stream << x.first << "\r\n";
            stream << x.second << "\r\n";
        }

        // multimap<QDate, vector<vector<QString>>>
        stream << "db_orders" << "\r\n";
        stream << db_orders.size() << "\r\n";
        for (const auto& x : db_orders){
            qDebug() << x.first.toString("dd.MM.yyyy");
            stream << x.first.toString("dd.MM.yyyy") << "\r\n";        // Date
            stream << x.second.size() << "\r\n";        // Date size
            for (const auto& q : x.second){
                stream << q.size() << "\r\n";           // Order size
                for (const auto& y : q){
                    stream << y << "\r\n";
                }
            }
        }

        // std::multiset<QDate>;
        stream << "db_set" << "\r\n";
        stream << db_set.size() << "\r\n";
        for (const auto& x : db_set){
            stream << x.toString("dd.MM.yyyy") << "\r\n";
        }

        // std::map<int, std::map<int, int>>;
        stream << "db_orders_number" << "\r\n";
        stream << db_orders_number.size() << "\r\n";
        for (const auto& x : db_orders_number){
            stream << x.first << "\r\n";
            stream << x.second.size() << "\r\n";
            for (const auto& y : x.second){
                stream << y.first << "\r\n";
                stream << y.second << "\r\n";
            }
        }

        // std::map<QString, int>
        stream << "db_most_populat_baloon" << "\r\n";
        stream << db_most_populat_baloon.size() << "\r\n";
        for (const auto& x : db_most_populat_baloon){
            stream << x.first << "\r\n";
            stream << x.second << "\r\n";
        }

        // std::vector<int>
        stream << "db_status" << "\r\n";
        stream << db_status.size() << "\r\n";
        for (const auto& x : db_status){
            stream << x << "\r\n";
        }

        file.close();
    } else {
        qDebug() << "I can't write to file!!!!";
    }
}

bool DataBase::ReadDataBase() {

    // Read PATH
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile file(PATH);

    if(file.open(QIODevice::ReadOnly)){
        if (!file.exists()){
            qDebug() << "No file to load DataBase";
            return false;
        }

        QTextStream stream(&file);
//        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        // Read PATH
        if (stream.readLine() != "PATH"){
            qDebug() << "Error with PATH";
            return false;
        }
        PATH = stream.readLine();

        // Read db_storage_of_purchase
        if (stream.readLine() != "db_storage_of_purchase"){
            qDebug() << "Error with db_storage_of_purchase";
            return false;
        }
        // std::vector<int>
        size_t x1 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x1; ++i){
            db_storage_of_purchase[i] = (stream.readLine().toInt());
        }

        // Read db_baloons
        if (stream.readLine() != "db_baloons"){
            qDebug() << "Error with db_baloons";
            return false;
        }
        // map<QString, std::vector<double>>
        size_t x2 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x2; ++i){
            QString tmp = stream.readLine();
            db_baloons[tmp];  // Name
            size_t x21 = stream.readLine().toULongLong();
            for (size_t j = 0; j < x21; ++j){
                db_baloons[tmp].push_back(stream.readLine().toDouble());
            }
        }

        // Read db_social_networks
        if (stream.readLine() != "db_social_networks"){
            qDebug() << "Error with db_social_networks";
            return false;
        }

        qDebug() << "READ SOCIAL BEFORE:";
        for (const auto& t : db_social_networks){
            qDebug() << t.first << t.second;
        }

        //map<QString, int>
        size_t x3 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x3; ++i){
            QString tmp = stream.readLine();
            int pop = stream.readLine().toInt();
            qDebug() << "TMP =" << tmp << "INT =" << pop;

            db_social_networks[tmp] = pop;
        }

        qDebug() << "READ SOCIAL:";
        for (const auto& t : db_social_networks){
            qDebug() << t.first << t.second;
        }
        // Read db_orders
        if (stream.readLine() != "db_orders"){
            qDebug() << "Error with db_orders";
            return false;
        }
        // multimap<QDate, vector<vector<QString>>>
        size_t x4 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x4; ++i){
            QStringList list = stream.readLine().split(".");
            QDate date;
            date.setDate((list.begin() + 2)->toInt(), (list.begin() + 1)->toInt() ,list.begin()->toInt());
            qDebug() << "DATE =" << date.toString("dd.MM.yyyy");
            size_t x41 = stream.readLine().toULongLong();
            My_vector tmp;
            for (size_t j = 0; j < x41; ++j){
                size_t x42 = stream.readLine().toULongLong();
                std::vector<QString> tmp2;
                for (size_t p = 0; p < x42; ++p){
                    tmp2.push_back(stream.readLine());
                }
                tmp.push_back(std::move(tmp2));
            }
            db_orders.insert(std::make_pair(date, std::move(tmp)));
        }

        // Read db_set
        if (stream.readLine() != "db_set"){
            qDebug() << "Error with db_set";
            return false;
        }
        // std::multiset<QDate>
        size_t x5 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x5; ++i){
            QStringList list = stream.readLine().split(".");
            QDate date;
            date.setDate((list.begin() + 2)->toInt(), (list.begin() + 1)->toInt() ,list.begin()->toInt());
            db_set.insert(date);
        }

        // Read db_orders_number
        if (stream.readLine() != "db_orders_number"){
            qDebug() << "Error with db_orders_number";
            return false;
        }
//        using MapCountOrders = std::map<int, std::map<int, int>>;
        //                      map<year, map<month, Orders's count>>
        size_t x6 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x6; ++i){
             int year = stream.readLine().toInt();
             size_t x61 = stream.readLine().toULongLong();
             std::map<int, int> map_tmp;
             for (size_t j = 0; j < x61; ++j){
                 int month = stream.readLine().toInt();
                 int count = stream.readLine().toInt();
                 map_tmp.insert(std::make_pair(month, count));
             }
             db_orders_number.insert(std::make_pair(year, std::move(map_tmp)));
        }

        qDebug() << "++++++++++++++++++++++++++++++++++++++++++=";
        qDebug() << db_orders_number.size();
        for (const auto& x : db_orders_number){
            qDebug() << x.first;
            for (const auto& y : x.second){
                qDebug() << y.first;
                qDebug() << y.second;
            }
        }

        // Read db_most_populat_baloon
        if (stream.readLine() != "db_most_populat_baloon"){
            qDebug() << "Error with db_most_populat_baloon";
            return false;
        }
        //std::map<QString, int>
        size_t x7 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x7; ++i){
            QString tmp = stream.readLine();
            db_most_populat_baloon.insert(std::make_pair(std::move(tmp), stream.readLine().toInt()));
        }

        // Read db_status
        if (stream.readLine() != "db_status"){
            qDebug() << "Error with db_status";
            return false;
        }
        //std::vector<int> db_status;
        size_t x8 = stream.readLine().toULongLong();
        for (size_t i = 0; i < x8; ++i){
            db_status[i] = (stream.readLine().toInt());
        }
        return true;
    } else {
        qDebug() << "I can't read the file !!!!";
    }
    return false;
}

DataBase::~DataBase(){
    qDebug() << "Buy from DataBase!";
}

void DataBase::SetPATH(const QString &path){
    PATH = path;
}

const QString& DataBase::GetPATH() const{
    return PATH;
}

QString DataBase::GetDataBaseName() const{
    if (PATH.isEmpty()){
        return "Новый проект";
    }
    return PATH.split("/").back();
}

void DataBase::CreateEmptyPurchaseStorage(){
    for (size_t i = 0; i < 7; ++i){
        db_storage_of_purchase.push_back(0);
    }
}

int DataBase::GetCost(Items items) const {
    switch (items) {
    case Items::BALOONS:
        return db_storage_of_purchase[0];
    case Items::TAXI:
        return db_storage_of_purchase[1];
    case Items::HELIUM:
        return db_storage_of_purchase[2];
    case Items::OTHER:
        return db_storage_of_purchase[3];
    case Items::PROCEEDS:
        return db_storage_of_purchase[4];
    case Items::TOTALORDERS:
        return db_storage_of_purchase[5];
    case Items::BALOONSTOTALAMOUNT:
        return db_storage_of_purchase[6];
    }
    return 1000000;
}

void DataBase::ChangeCost(int price, Items items){
    switch (items) {
    case Items::BALOONS:
        db_storage_of_purchase[0] += price;
        break;
    case Items::TAXI:
        db_storage_of_purchase[1] += price;
        break;
    case Items::HELIUM:
        db_storage_of_purchase[2] += price;
        break;
    case Items::OTHER:
        db_storage_of_purchase[3] += price;
        break;
    case Items::PROCEEDS:
        qDebug() << "now = " << db_storage_of_purchase[4] << "I Add" << price <<"to proceed";
        db_storage_of_purchase[4] += price;
        qDebug() << "PROSEED AFTER" << db_storage_of_purchase[4];
        break;
    case Items::TOTALORDERS:
        db_storage_of_purchase[5] += price;
        break;
    case Items::BALOONSTOTALAMOUNT:
        db_storage_of_purchase[6] += price; // It's not a price, it's a count
        break;
    }
}

void DataBase::SetCost(int price, Items items){
    switch (items) {
    case Items::BALOONS:
        db_storage_of_purchase[0] -= price;
        break;
    case Items::TAXI:
        db_storage_of_purchase[1] -= price;
        break;
    case Items::HELIUM:
        db_storage_of_purchase[2] -= price;
        break;
    case Items::OTHER:
        db_storage_of_purchase[3] -= price;
        break;
    case Items::PROCEEDS:
        db_storage_of_purchase[4] -= price;
        break;
    case Items::TOTALORDERS:
        db_storage_of_purchase[5] -= price;
        break;
    case Items::BALOONSTOTALAMOUNT:
        db_storage_of_purchase[6] -= price; // It's not a price, it's a count
        break;
    }
}

void DataBase::NormalSetCost(int price, Items items){
    switch (items) {
    case Items::BALOONS:
        db_storage_of_purchase[0] = price;
        break;
    case Items::TAXI:
        db_storage_of_purchase[1] = price;
        break;
    case Items::HELIUM:
        db_storage_of_purchase[2] = price;
        break;
    case Items::OTHER:
        db_storage_of_purchase[3] = price;
        break;
    case Items::PROCEEDS:
        db_storage_of_purchase[4] = price;
        break;
    case Items::TOTALORDERS:
        db_storage_of_purchase[5] = price;
        break;
    case Items::BALOONSTOTALAMOUNT:
        db_storage_of_purchase[6] = price; // It's not a price, it's a count
        break;
    }
}

const My_map& DataBase::GetMap() const {
    return db_baloons;
}

void DataBase::ChangeOldBaloons(const QString &name, int count) {
    db_baloons.at(name)[0] += count;
}

void DataBase::AddNewBaloons(My_map m){
    for (auto &x : m){
        db_baloons.insert(std::make_pair(std::move(x.first), std::move(x.second)));
    }
}

void DataBase::AddNewBaloon2(const QString &name, std::vector<double> v){
    if (db_baloons.count(name) == 0){
        db_baloons.insert(std::make_pair(name, std::move(v)));
    }
}

void DataBase::DeleteBaloon(const QString &name){
    if (db_baloons.count(name) == 1){
        db_baloons.erase(name);
    }
}

bool DataBase::CheckElementInMap(const QString &name) const {
    if (db_baloons.count(name) == 1) {
        return false;
    }
    return true;
}

void DataBase::AddSomethingToMap(){
    db_baloons["first"] = {1, 2, 3};
     db_baloons["second"] = {1, 2, 3};
      db_baloons["third"] = {1, 2, 3};
      db_baloons["fourth"] = {1, 2, 3};
      db_baloons["5"] = {1, 2, 3};
      db_baloons["6"] = {1, 2, 3};
      db_baloons["7"] = {1, 2, 3};
      db_baloons["8"] = {1, 2, 3};
      db_baloons["9"] = {1, 2, 3};
      db_baloons["10"] = {1, 2, 3};
}

std::optional<QString> DataBase::CheckBaloonsCount(const QString &name, int count) const {
    if (db_baloons.at(name)[0] >= count)
        return name;
    return std::nullopt;
}

double DataBase::GetCount(const QString &name) const {
    if (db_baloons.count(name) != 1)
    {
        qDebug() << "Shit is happens...GET_COUNT";
    }
    return db_baloons.at(name)[0];
}

void DataBase::DellCount(const QString &name, int count){
    if (db_baloons.count(name) == 1)
    {
        db_baloons.at(name)[0] -= count;
        db_most_populat_baloon[name] += count;
    } else {
        qDebug() << "Shit is happens...DEL";
    }
}

void DataBase::ReturnCountInMap(const QString &name, int count){
    if (db_baloons.count(name) == 1)
    {
        db_baloons.at(name)[0] += count;
        db_most_populat_baloon[name] -= count;
    } else {
        qDebug() << "Shit is happens...RETURN";
    }
}

void DataBase::MinusBaloonsCharacteristics(const QString& name, double count, BaloonChar chr){
    switch (chr) {
    case BaloonChar::COUNT:
        db_baloons.at(name)[0] -= count;
        break;
    case BaloonChar::COST_PRICE:
        db_baloons.at(name)[1] -= count;
        break;
    case BaloonChar::SELLING_PRICE:
        db_baloons.at(name)[2] -= count;
        break;
    }
}

void DataBase::SetBaloonsCharacteristics(const QString &name, double count, BaloonChar chr){
    switch (chr) {
    case BaloonChar::COUNT:
        db_baloons.at(name)[0] = count;
        break;
    case BaloonChar::COST_PRICE:
        db_baloons.at(name)[1] = count;
        break;
    case BaloonChar::SELLING_PRICE:
        db_baloons.at(name)[2] = count;
        break;
    }
}

void DataBase::AddToOrder(const QDate& date, My_vector order){
    db_orders.insert(std::make_pair(date, order));
}

const SuperMap& DataBase::GetOrder() const {
    return db_orders;
}

void DataBase::ChangeOrderStatus(SuperMap::const_iterator it, const QString& status){
    for(auto iter = db_orders.begin(); iter != db_orders.end(); ++iter){
        if (iter == it){
            iter->second[iter->second.size() - 1][8] = status;
            break;
        }
    }
}

void DataBase::EraseOrder(SuperMap::const_iterator it){
    db_orders.erase(it);
}

void DataBase::AddToSet(const QDate &date){
    db_set.insert(date);
}

const My_set& DataBase::GetSet() const {
    return db_set;
}

void DataBase::EraseOrderInSet(My_set::const_iterator it){
    db_set.erase(it);
}

const std::map<QString, int> DataBase::GetSocialNetworksMap() const{
    return db_social_networks;
}

void DataBase::PlusToSocialNetworksMap(const QString &name){
    ++db_social_networks[name];
}

void DataBase::MinusToSocialNetworksMap(const QString &name){
    --db_social_networks[name];
}

void DataBase::CreateEmptySocialMap(){
    db_social_networks["Instagram"];
    db_social_networks["VK"];
    db_social_networks["WhatsApp"];
    db_social_networks["Сайт"];
    db_social_networks["Другое"];
    db_social_networks["Знакомые"];
}

const std::vector<int>  DataBase::GetStatusOfOrders() const{
    return db_status;
}

void  DataBase::PlusStatusOfOrders(Status status){
    switch (status) {
    case Status::YES:
        ++db_status[0];
        break;
    case Status::NO:
        ++db_status[1];
        break;
    case Status::PROC:
        ++db_status[2];
        break;
    }
}

void  DataBase::MinusStatusOfOrders(Status status){
    switch (status) {
    case Status::YES:
        --db_status[0];
        break;
    case Status::NO:
        --db_status[1];
        break;
    case Status::PROC:
        --db_status[2];
        break;
    }
}

int  DataBase::GetCountOfOrderStatus(Status status) const {
    switch (status) {
    case Status::YES:
        return db_status[0];
    case Status::NO:
        return db_status[1];
    case Status::PROC:
        return db_status[2];
    }
    qDebug() << "I have a problem in DataBase in GetCountOfOrderStatus";
    return 0;
}

const MapCountOrders& DataBase::GetNumbersOfOrders() const {
    return db_orders_number;
}

void DataBase::PlusNumbesrOfOrders(const QDate& date){
    ++db_orders_number[date.year()][date.month()];
}

void DataBase::MinusNumbesrOfOrders(const QDate& date){
    --db_orders_number[date.year()][date.month()];
}

int DataBase::GetNumberOdOrdersCurrentMonth(int year, int month){
    return db_orders_number[year][month];
}

std::optional<QString> DataBase::GetMostPopularBaloon(){
    QString tmp;
    int num = 0;
    for (const auto& x : db_most_populat_baloon){
        if (num < x.second){
            num = x.second;
            tmp = x.first;
        }
    }
    if (num != 0){
        return std::move(tmp);
    }
    return std::nullopt;
}
