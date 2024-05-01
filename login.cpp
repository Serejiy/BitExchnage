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
            qDebug()<< "Connected to the databasedile :)";
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
    if(qry.exec("SELECT Username,Password, Role FROM Users WHERE Username=\'" + Username + "\' AND Password=\'" + Password + "\'"))
    {
        if(qry.next())
        {
            MainWindow *mainwindowPage = new MainWindow();
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


