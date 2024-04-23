#include "registration.h"
#include "ui_registration.h"
#include "welcomepage.h"
#include "ui_welcomepage.h"

registration::registration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::registration)
{
    ui->setupUi(this);

}

registration::~registration()
{

    delete ui;
}

void registration::on_regButton_clicked()
{

}


void registration::on_clearButton_clicked()
{
    ui->txtPass->setText("");
    ui->txtUser->setText("");
}


void registration::on_backButton_clicked()
{
    welcomepage *welcomePage = new welcomepage();
    welcomePage->show();
    hide();
}

