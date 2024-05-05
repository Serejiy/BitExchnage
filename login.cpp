#include "login.h"
#include "ui_login.h"
#include "welcomepage.h"
#include "mainwindow.h"

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
            qDebug()<< "Connected to the databasedile m :)";
        }
    } else {
        qDebug()<< "Database file not found :(";
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
    if(qry.exec("SELECT Username,Password, Role, USDT, BTC, ETH, BNB, PEPE, DOGE FROM Users WHERE Username=\'" + Username + "\' AND Password=\'" + Password + "\'"))
    {
        if(qry.next())
        {
            //user_value;
            QString user_value = qry.value(0).toString();
            /*double USDT_value = qry.value(3).toDouble();
            double BTC_value = qry.value(4).toDouble();
            double ETH_value = qry.value(5).toDouble();
            double BNB_value = qry.value(6).toDouble();
            double PEPE_value = qry.value(7).toDouble();
            double DOGE_value = qry.value(8).toDouble();

            // Выводим значения в консоль для проверки
            qDebug()<< "Username" << user_value;

            qDebug() << "USDT" << USDT_value << " ";
            qDebug() << "BTC:" << BTC_value<< " ";
            qDebug() << "ETH:" << ETH_value<< " ";
            qDebug() << "BNB:" << BNB_value<< " ";
            qDebug() << "PEPE:" << PEPE_value<< " ";
            qDebug() << "DOGE:" << DOGE_value<< " ";*/

            MainWindow *mainwindowPage = new MainWindow(user_value);
            mainwindowPage->show();
            close();

        }else{
            QMessageBox::information(this,"Inserted","Wrong username or password.");
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



