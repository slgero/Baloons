#ifndef ADDOLDBALOONS_H
#define ADDOLDBALOONS_H

#include <database.h>
#include <QDialog>

namespace Ui {
class AddOldBaloons;
}

class AddOldBaloons : public QDialog
{
    Q_OBJECT

public:
    explicit AddOldBaloons(QWidget *parent = nullptr, Data db = nullptr);
    void UpdateList();
    ~AddOldBaloons();

private slots:
    void on_cancel_clicked();

    void on_add_clicked();

    void ChangePrice();

    void on_line_price_textEdited(const QString &arg1);

private:
    Ui::AddOldBaloons *ui;
    Data data;
    bool TextEditingFlag;
};

#endif // ADDOLDBALOONS_H
