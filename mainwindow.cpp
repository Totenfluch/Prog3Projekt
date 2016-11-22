#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QTimer>
#include <srf02.h>
#include <QList>


static int datenCounter = 0;
static int fd;
static QList<double> ourValues;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    x = new QWidget();

    ui->setupUi(this);
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    ui->widget->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->widget->graph(0)->setAntialiasedFill(false);
    ui->widget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widget->xAxis->setDateTimeFormat("mm:ss");
    ui->widget->xAxis->setAutoTickStep(true);
    ui->widget->xAxis->setTickStep(5);
    ui->widget->axisRect()->setupFullAxesBox();
    ui->widget->yAxis->setRange(0,600);
    ui->widget->graph(0)->rescaleAxes();

    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));

    fd = initi2c();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveData_clicked()
{
    for(int i = 0; i < ourValues.size(); i++){
        int value = ourValues.at(i);
    }
}

void MainWindow::update(){
    if(!(ui->newDataCheckBox->isChecked()))
        return;
    writeByte(fd, 0x00, 0x51);
    int distance = readByte(fd, 0x02)+readByte(fd, 0x03);
    ui->widget->graph(0)->addData(datenCounter++, distance);
    ui->widget->graph(0)->rescaleAxes();
    ui->widget->replot();
    ourValues.append(distance);
}

void MainWindow::on_pushButton_clicked()
{
    QVector<double> x(1), y(1);
    ui->widget->graph(0)->setData(x, y);
    datenCounter = 0;
    ui->widget->graph(0)->rescaleAxes();
    ui->widget->replot();
    ourValues.clear();
}
