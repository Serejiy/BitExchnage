#include "welcomepage.h"
#include "ui_welcomepage.h"
#include "login.h"
#include "registration.h"

welcomepage::welcomepage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::welcomepage)
{
    ui->setupUi(this);
}

welcomepage::~welcomepage()
{
    delete ui;
}

void welcomepage::on_login_clicked()
{
    login *loginPage = new login();
    loginPage->show();
    close();
}



void welcomepage::on_registration_clicked()
{
    registration *registrationPage = new registration();
    registrationPage->show();
    close();
}

