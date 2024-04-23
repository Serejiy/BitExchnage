#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QDebug>
#include <QtSql>
#include <QFileInfo>
#include <QMessageBox>

namespace Ui {
class registration;
}

class registration : public QWidget
{
    Q_OBJECT
public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();

private slots:
    void on_clearButton_clicked();
    void on_regButton_clicked();
    void on_backButton_clicked();

private:
    Ui::registration *ui;

};

#endif // REGISTRATION_H
