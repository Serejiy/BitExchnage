#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "binanceclient.h"

#include <QMainWindow>
#include <QThread>
#include <QMutex>

#include <QtSql>
#include <QSqlDatabase>
#include <QFileInfo>

#include <QtCharts>

#include "candlesticksview.h"

namespace Ui {
class MainWindow;
}

class priceThread: public QThread
{
    Q_OBJECT
public:
    priceThread(QObject* parent);

    void run();
private:

    QObject* _parent;

signals:

    void emitPrice();
    void refreshAccount();
    void refreshCandles();
    void refreshSTime();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& username, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    binanceClient* _client;
    priceThread* _thread;
    QCandlestickSeries* _wtcethCandles;
    QChart* _chart = new QChart();
    candleSticksView* _chartView;

    qulonglong _serverTime;

    QString _ccurrency;

    bool _bReady;
    double _currentPrice;
    double _priceOrder;

    QSqlDatabase myDB;
    QString m_username;


public slots:
    void onPrice();
    void onPriceReply(double price);
    void onRefreshSTimeReply(qulonglong stime);
    void onCandleReply(QJsonArray jcandleArray);
    void onRefreshCandles();
    void onRefreshSTime();

private slots:
    void on_Hour_clicked();
    void on_TwoHours_clicked();
    void on_FourHours_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_PlaceOrder_clicked();
    void on_EightHours_clicked();
    void on_TwentyFourHours_clicked();
    void on_Buybtn_clicked();
    void on_Selbtn_clicked();
    void labels_update();
};

#endif // MAINWINDOW_H

