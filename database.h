#pragma once
#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include <QString>
#include <memory>
#include <utility>
#include <QDebug>
#include <optional>
#include <QDate>

enum class Items {
    BALOONS,
    TAXI,
    HELIUM,
    OTHER,
    PROCEEDS,
    TOTALORDERS,
    BALOONSTOTALAMOUNT
};

enum class Status {
    YES,
    NO,
    PROC
};

enum class BaloonChar{
    COUNT,
    COST_PRICE,
    SELLING_PRICE
};

class DataBase;

using My_map =  std::map<QString, std::vector<double>>; // Baloons properties
//                   map<name, vector<count, cost price, selling price>>
//                                     [0]       [1]           [2]

using My_vector = std::vector<std::vector<QString>>; // I can use variant, but vector is rather faster
// name -> count -> real revenue -> with helium or not("0", "1")
//  [0]     [1]          [2]                [3]
//                                       <<<< OR >>>>
// all count -> all cost price -> all selling price -> telephone number -> comment -> social networks -> taxi -> other -> status
//    [0]             [1]                [2]                  [3]              [4]         [5]            [6]      [7]      [8]

// status == YES || NO  || PROC

using SuperMap = std::multimap<QDate, My_vector>; // Orders
//                              Data     Order

using My_set = std::multiset<QDate>; // For orders

using MapCountOrders = std::map<int, std::map<int, int>>;
//                    map<year, map<month, Orders's count>>

using Data = std::shared_ptr<DataBase>;

class DataBase
{
public:
    DataBase();

    void SaveDataBase();
    bool ReadDataBase();

    void SetPATH(const QString& path);
    const QString& GetPATH() const;

    void CreateEmptyPurchaseStorage();
    int GetCost(Items items) const;
    void SetCost(int price, Items items);
    void NormalSetCost(int price, Items items);
    void ChangeCost(int price, Items items);

    const My_map& GetMap() const;
    void ChangeOldBaloons(const QString& name, int count);
    void AddNewBaloons(My_map m);
    void AddNewBaloon2(const QString& name, std::vector<double>);
    void DeleteBaloon(const QString& name);
    bool CheckElementInMap(const QString& name) const;
    void AddSomethingToMap();
    std::optional<QString> CheckBaloonsCount(const QString& name, int count) const;
    double GetCount(const QString& name) const;
    void DellCount(const QString& name, int count);
    void ReturnCountInMap(const QString& name, int count);
    void MinusBaloonsCharacteristics(const QString& name, double count, BaloonChar chr);
    void SetBaloonsCharacteristics(const QString& name, double count, BaloonChar chr);

    void AddToOrder(const QDate& date, My_vector order);
    const SuperMap& GetOrder() const;
    void ChangeOrderStatus(SuperMap::const_iterator it, const QString& status);
    void EraseOrder(SuperMap::const_iterator it);

    void AddToSet(const QDate& date);
    const My_set& GetSet() const;
    void EraseOrderInSet(My_set::const_iterator it);

    const std::map<QString, int> GetSocialNetworksMap() const;
    void PlusToSocialNetworksMap(const QString& name);
    void MinusToSocialNetworksMap(const QString& name);
    void CreateEmptySocialMap();

    const std::vector<int> GetStatusOfOrders() const;
    void PlusStatusOfOrders(Status status);
    void MinusStatusOfOrders(Status status);
    int  GetCountOfOrderStatus(Status status) const;

    const MapCountOrders& GetNumbersOfOrders() const;
    void PlusNumbesrOfOrders(const QDate& date);
    void MinusNumbesrOfOrders(const QDate& date);
    int  GetNumberOdOrdersCurrentMonth(int year, int month);

    std::optional<QString> GetMostPopularBaloon();

    ~DataBase();

private:
    QString PATH;
    std::vector<int> db_storage_of_purchase;
    // Baloons -> Taxi -> Helium -> Other -> Proceed -> Total Amount of Orders -> Baloons Total Amount
    //  [0]        [1]      [2]      [3]       [4]              [5]                        [6]
    My_map db_baloons;
    std::map<QString, int> db_social_networks;
    SuperMap db_orders;
    My_set db_set;
    MapCountOrders db_orders_number;
    std::map<QString, int> db_most_populat_baloon;
    std::vector<int> db_status;
    //  YES     NO      PROC
    //  [0]     [1]     [2]
};
