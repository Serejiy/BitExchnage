#include "registration.h"
#include "ui_login.h"
#include "login.h"
#include "ui_registration.h"
#include "welcomepage.h"
#include "ui_welcomepage.h"

#define Path_to_DB "C:/Users/nomi4/Desktop/Bit_Exchange/DB/db.db"

registration::registration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::registration)
{
    ui->setupUi(this);

    myDB = QSqlDatabase::addDatabase("QSQLITE");
    myDB.setDatabaseName(Path_to_DB);
    QFileInfo checkFile(Path_to_DB);

    if (checkFile.isFile()){
        if (myDB.open()){
            ui->label_3->setText("[+] Connected to Database File :)");
        }
    } else {
        ui->label_3->setText("[+] Database File does not exist :(");
    }

}

registration::~registration()
{

    delete ui;
}

void registration::on_regButton_clicked()
{
    myDB.setHostName("localhost");
    myDB.setUserName("root");
    myDB.setPassword("");

    if (myDB.open()) {
        QString username = ui->txtUser->text();
        QString password = ui->txtPass->text();

        QSqlQuery qry;

        qry.prepare("INSERT INTO users(Username,Password)"
                    "VALUES(:Username,:Password)");

        qry.bindValue(":Username",username);
        qry.bindValue(":Password",password);

        if (qry.exec()){

            QMessageBox::information(this,"Inserted","Data Inserted Successfuly");

            login *loginPage = new login();
            loginPage->show();
            close();

        }else{
            QMessageBox::information(this,"Not inserted","Data Inserted Unsuccessfuly");
        }

    }

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
    close();
}

