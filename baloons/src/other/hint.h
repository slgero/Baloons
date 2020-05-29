#ifndef HINT_H
#define HINT_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Hint;
}

class Hint : public QDialog
{
    Q_OBJECT

public:
    explicit Hint(QWidget *parent = nullptr);
    ~Hint();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Hint *ui;
};

#endif // HINT_H
