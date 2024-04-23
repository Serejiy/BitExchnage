#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class welcomepage;
}
QT_END_NAMESPACE

class welcomepage : public QMainWindow
{
    Q_OBJECT

public:
    welcomepage(QWidget *parent = nullptr);
    ~welcomepage();

private slots:
    void on_registration_clicked();

    void on_login_clicked();

private:
    Ui::welcomepage *ui;
};
#endif // WELCOMEPAGE_H
