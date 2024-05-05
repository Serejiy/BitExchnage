#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QSettings>

#define Path_to_DB "C:/Users/nomi4/Desktop/Bit_Exchange/DB/db.db"

//double USDT_value, BTC_value, ETH_value, BNB_value, PEPE_value, DOGE_value;

MainWindow::MainWindow(const QString& username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_username(username),
    _currentPrice(0.0),
    _bReady(false),
    _serverTime(0),
    _priceOrder(0.0),
    _ccurrency("BTC")
{
    ui->setupUi(this);
    QIcon icon(":/images/logo.png");
    setWindowIcon(icon);

    ui->UserLabel->setText(m_username);
    qDebug()<<"test";
    labels_update(); // Вызываем метод для обновления меток

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Path_to_DB);
    if(db.open()) {
        labels_update();
        qDebug() << "Database connected!";
    } else {
        qDebug() << "Failed to connect to the database!";
    }

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

void MainWindow::labels_update()
{
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        qDebug() << "No connection to db :(";
        return;
    }

    QSqlDatabase myDB = QSqlDatabase::database("qt_sql_default_connection");
    if (!myDB.isOpen()) {
        qDebug() << "Failed to open the database!";
        return;
    }

    QSqlQuery qry(myDB);
    if(qry.exec("SELECT Username,USDT, BTC, ETH, BNB, PEPE, DOGE FROM users WHERE Username =" + m_username))
    {
        qDebug()<< "exec";
        if(qry.next())
        {
            double USDT_value = qry.value(1).toDouble();
            double BTC_value = qry.value(2).toDouble();
            double ETH_value = qry.value(3).toDouble();
            double BNB_value = qry.value(4).toDouble();
            double PEPE_value = qry.value(5).toDouble();
            double DOGE_value = qry.value(6).toDouble();

            QString USDT_string, BTC_string, ETH_string, BNB_string, PEPE_string, DOGE_string;

            USDT_string = QString::number(USDT_value);
            BTC_string = QString::number(BTC_value);
            ETH_string = QString::number(ETH_value);
            BNB_string = QString::number(BNB_value);
            PEPE_string = QString::number(PEPE_value);
            DOGE_string = QString::number(DOGE_value);

            ui->USDTOutput->setText(USDT_string);
            ui->BTCValue->setText(BTC_string);
            ui->ETHValue->setText(ETH_string);
            ui->BNBValue->setText(BNB_string);
            ui->PEPEValue->setText(PEPE_string);
            ui->DOGEValue->setText(DOGE_string);

        } else {
            QMessageBox::information(this, "Error", "Failed to fetch data from database!");
        }
    } else {
        QMessageBox::information(this, "Error", "Query execution failed!");
    }
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
}


void MainWindow::on_radioButton_clicked()
{
    _ccurrency = "BTC";
}


void MainWindow::on_radioButton_3_clicked()
{
    _ccurrency = "BNB";
}


void MainWindow::on_radioButton_4_clicked()
{
    _ccurrency = "PEPE";

}


void MainWindow::on_radioButton_5_clicked()
{
    _ccurrency = "DOGE";

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
