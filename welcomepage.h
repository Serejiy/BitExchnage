#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class welcomepage;
}
QT_END_NAMESPACE

class welcomepage : public QWidget
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
<<<<<<< HEAD
#endif
=======
#endif // WELCOMEPAGE_H
>>>>>>> 56574a742599998160a589b0df374320011c6ce8
