#include "greeting.h"
#include "ui_greeting.h"

Greeting::Greeting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Greeting)
{
    ui->setupUi(this);
    connect(this, SIGNAL(GetOrgName(QString)), parent, SLOT(SetOrganisationName(QString)));
}

Greeting::~Greeting()
{
    delete ui;
}

void Greeting::on_pushButton_clicked()
{
    emit GetOrgName(ui->lineEdit->text());
    close();
}
