#include "hint.h"
#include "ui_hint.h"

Hint::Hint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Hint)
{
    ui->setupUi(this);
}

Hint::~Hint()
{
    delete ui;
}

void Hint::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "Информация", "Прочитайте вкладку F.A.Q в главном окне.");
}

void Hint::on_pushButton_clicked()
{
    close();
}
