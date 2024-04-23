#include "login.h"
#include "ui_login.h"
#include "welcomepage.h"
#include "charts.h"

#define Path_to_DB "C:/Users/nomi4/Desktop/Bit_Exchange/DB/db.db"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
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

login::~login()
{
    myDB.close();
    qDebug()<< "Closing the connection to Database file on exit";
    delete ui;
}

void login::on_loginBtn_clicked()
{
    QString Username, Password;
    Username = ui->txtUser->text();
    Password = ui->txtPass->text();

    if (!myDB.isOpen()){
        qDebug() << "No connection to db :(";
        return;
    }

    QSqlQuery qry;
    if(qry.exec("SELECT Username,Password, Role FROM Users WHERE Username=\'" + Username + "\' AND Password=\'" + Password + "\'"))
    {
        if(qry.next())
        {
            ui->label_3->setText("[+]Valid Username and Password");
            QString msg = "Username = " + qry.value(0).toString() + "\n";

            QMessageBox::warning(this, "Login was successful", msg);

            charts *chartsPage = new charts();
            chartsPage->show();
            close();

        }else{
            ui->label_3->setText("[-]Wrong username or password. :(");
        }
    }
}


void login::on_pushButton_2_clicked()
{
    welcomepage *welcomePage = new welcomepage();
    welcomePage->show();
    close();

}

void login::on_pushButton_3_clicked()
{
    ui->txtPass->setText("");
    ui->txtUser->setText("");
}


