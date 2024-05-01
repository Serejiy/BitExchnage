#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _currentPrice(0.0),
    _bReady(false),
    _serverTime(0),
    _priceOrder(0.0),
    _ccurrency("BTC")
{
    ui->setupUi(this);
    QIcon icon(":/images/logo.png");
    setWindowIcon(icon);

    QString path(QDir::currentPath() + QDir::separator() + "key.ini");
    QSettings settings(path, QSettings::IniFormat);
    QString api_key=settings.value("api_key").toString();
    QString secret_key=settings.value("secret_key").toString();

    _client = new binanceClient(api_key.toLocal8Bit()
                                , secret_key.toLocal8Bit());

    connect(_client, SIGNAL(priceSignal(double)), this, SLOT(onPriceReply(double)));
    connect(_client, SIGNAL(serverTimeSignal(qulonglong)), this, SLOT(onRefreshSTimeReply(qulonglong)));
    connect(_client, SIGNAL(candleSticksSignal(QJsonArray)), this, SLOT(onCandleReply(QJsonArray)));
    _thread = new priceThread(this);
    connect(_thread, SIGNAL(emitPrice()), this, SLOT(onPrice()));
    connect(_thread, SIGNAL(refreshCandles()), this, SLOT(onRefreshCandles()));
    connect(_thread, SIGNAL(refreshSTime()), this, SLOT(onRefreshSTime()));
    _thread->start();

    _wtcethCandles = new QCandlestickSeries();
    _wtcethCandles->setIncreasingColor(QColor(Qt::green));
    _wtcethCandles->setDecreasingColor(QColor(Qt::red));

    _chart = new QChart();
    _chart->setAnimationOptions(QChart::NoAnimation);
    _chart->setTheme(QChart::ChartThemeDark);

    _chart->legend()->setVisible(false);

    _chartView = new candleSticksView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayout_2->addWidget(_chartView);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPrice()
{
    _client->getSymbolPrice(_ccurrency + QString("USDT"));
}

void MainWindow::onPriceReply(double price)
{
    _currentPrice = price;
    ui->lcdNumberWTCETH->display(price);


}

void MainWindow::onRefreshSTimeReply(qulonglong stime)
{
    _serverTime = stime;
}

void MainWindow::onCandleReply(QJsonArray jcandleArray)
{

    QStringList categories;

    _chart->removeSeries(_wtcethCandles);
    _wtcethCandles->clear();

    int count = 0;
    foreach(QJsonValue element, jcandleArray)
    {
        QJsonArray array = element.toArray();
        QCandlestickSet *candlestickSet = new QCandlestickSet(array[0].toDouble());
        candlestickSet->setOpen(array[1].toString().toDouble());
        candlestickSet->setHigh(array[2].toString().toDouble());
        candlestickSet->setLow(array[3].toString().toDouble());
        candlestickSet->setClose(array[4].toString().toDouble());
        _wtcethCandles->append(candlestickSet);
        categories << QDateTime::fromMSecsSinceEpoch(candlestickSet->timestamp()).toString("hh:mm");
        count++;
    }

    _chart->addSeries(_wtcethCandles);

    _chart->createDefaultAxes();

    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(_chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);

    if(!_chartView->ready()) _chartView->setReady(true);

}

qulonglong candleInterval = 3600000; // По умолчанию интервал равен 1 часу


void MainWindow::onRefreshCandles()
{
    if(_serverTime > 0)
        _client->candleSticks(_ccurrency + QString("USDT"), _serverTime - candleInterval);
}



void MainWindow::on_Hour_clicked()
{
    candleInterval = 3600000; // Интервал в 1 час
    onRefreshCandles(); // Обновляем данные о свечах
}

void MainWindow::on_TwoHours_clicked()
{
    candleInterval = 7200000; // Интервал в 2 часа
    onRefreshCandles(); // Обновляем данные о свечах
}

void MainWindow::on_FourHours_clicked()
{
    candleInterval = 14400000; // Интервал в 4 часа
    onRefreshCandles(); // Обновляем данные о свечах
}



void MainWindow::onRefreshSTime()
{
    _client->getServerTime();
}

priceThread::priceThread(QObject *parent) : _parent(parent)
{

}

void priceThread::run()
{
    int refreshAccCntr = 0;
    int refreshCandleCntr = 0;
    int refreshSTimeCntr = 0;

    while(1)
    {
        if(refreshSTimeCntr % 120 == 0)
        {
            refreshSTimeCntr = 0;
            emit refreshSTime();
            //QThread::msleep(500);
        }

        if(refreshAccCntr % 50 == 0)
        {
            refreshAccCntr = 0;
            emit refreshAccount();
            //QThread::msleep(500);
        }

        if(refreshCandleCntr % 15 == 0)
        {
            refreshCandleCntr = 0;
            emit refreshCandles();
        }


        emit emitPrice();

        refreshAccCntr++;
        refreshCandleCntr++;
        refreshSTimeCntr++;

        QThread::msleep(500);
    }
}

void MainWindow::on_radioButton_2_clicked()
{
    _ccurrency = "ETH";
    ui->Token->setText("ETH");
}


void MainWindow::on_radioButton_clicked()
{
    _ccurrency = "BTC";
    ui->Token->setText("BTC");
}


void MainWindow::on_radioButton_3_clicked()
{
    _ccurrency = "BNB";
    ui->Token->setText("BNB");
}


void MainWindow::on_radioButton_4_clicked()
{
    _ccurrency = "PEPE";
    ui->Token->setText("PEPE");
}


void MainWindow::on_radioButton_5_clicked()
{
    _ccurrency = "DOGE";
    ui->Token->setText("DOGE");
}


void MainWindow::on_PlaceOrder_clicked()
{

}

void MainWindow::on_EightHours_clicked()
{
    candleInterval = 28800000;
    onRefreshCandles();
}


void MainWindow::on_TwentyFourHours_clicked()
{
    candleInterval = 86400000;
    onRefreshCandles();
}


void MainWindow::on_Buybtn_clicked()
{
    ui->Execbtn->setText("Buy");
}


void MainWindow::on_Selbtn_clicked()
{
    ui->Execbtn->setText("Sell");
}


void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    //connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::sliderValueChanged);

}

