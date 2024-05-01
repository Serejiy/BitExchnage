#ifndef LOGIN_H
#define LOGIN_H

#include "welcomepage.h"
#include <QWidget>
#include <QDebug>
#include <QtSql>
#include <QFileInfo>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class login;
}
QT_END_NAMESPACE

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_2_clicked();

    void on_loginBtn_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::login *ui;
    welcomepage *welcomePage;

    QSqlDatabase myDB;
};

#endif
