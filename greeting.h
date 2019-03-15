#ifndef GREETING_H
#define GREETING_H

#include <QDialog>

namespace Ui {
class Greeting;
}

class Greeting : public QDialog
{
    Q_OBJECT

public:
    explicit Greeting(QWidget *parent = nullptr);
    ~Greeting();

private slots:
    void on_pushButton_clicked();

signals:
    void GetOrgName(QString);

private:
    Ui::Greeting *ui;
};

#endif // GREETING_H
