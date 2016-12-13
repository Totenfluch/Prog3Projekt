/**
 * @file mainwindow.cpp
 *
 */

/**
 * @file mainwindow.cpp
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QTimer>
#include <srf02.h>
#include <QList>


static int datenCounter = 0; /** X Achsen Wert des Graphen */
static int fd; /** I2C Initialisierung */
static QList<double> ourValues; /** Alle Werte die Wir bisher gelesen haben */
/**
 * @brief
 * Inizialisiert das Fenster, den Graphen und den I2C Bus
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    x = new QWidget();

    // Erstellung des Graphen
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

    // Achsen verbinden
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));

    // Initialisieren I2C
    fd = initi2c();

    // Erstelle Timer der unseren Sensor abfragt und setzte Ihn auf einen slot
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

/**
 * @brief
 * Dekonstruktor -- aufräumen
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief
 * Speichert alle Daten die in der Liste sind in einer Text datei deren Pfad
 * durch 2 LineEdits angegeben werden kann
 */
void MainWindow::on_saveData_clicked()
{
    // Initialisiere Pfad von LineEdit
    QDir myDir(ui->dataPathText->text());
    // Überprüfe ob er exisitiert
    if(!myDir.exists())
        // Wenn nicht -> Erstelle kompletten Pfad
        myDir.mkpath(ui->dataPathText->text());

    // File initialisieren
    QFile myFile(ui->fileNameText->text());
    // Versuchen file zu öffnen im WriteOnly Modous als Text Device
    if(myFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // OutPutStream initialisieren
        QTextStream out(&myFile);
        for(int i = 0; i < ourValues.size(); i++){
            // Wert aus Liste
            int value = ourValues.at(i);
            out << "Value at " << dataCounter << ": " << value;
        }
        // Aufräumen
        out.reset();
    }
    // Aufräumen
    myFile.close();
}

/**
 * @brief
 * Ließt die Daten über I2C ein, packt sie in den Graphen, skaliert und zeichnet diesen neu
 * und speichert den Wert in einer Liste
 */
void MainWindow::update(){
    if(!(ui->newDataCheckBox->isChecked()))
        return;
    // Messwert holen durch schreiben auf I2C
    writeByte(fd, 0x00, 0x51);
    // Berechnung nach datenblatt. 255*Erstes Register + Zweites Register
    int distance = 255*readByte(fd, 0x02)+readByte(fd, 0x03);
    // Hinzufügen der Daten zum Graph
    ui->widget->graph(0)->addData(datenCounter++, distance);
    // Achsen neu skalieren
    ui->widget->graph(0)->rescaleAxes();
    // Wert im LCD Display anzeigen
    ui->rangeDisplay->display(distance);
    // Graph neu zeichnen
    ui->widget->replot();
    // Wert in unserer Liste abspeichern
    ourValues.append(distance);
}

/**
 * @brief
 * Setzt alle Daten des Graphen zurück und macht ihn leer
 */
void MainWindow::on_pushButton_clicked()
{
    // leeren Vektor erstellen
    QVector<double> x(0), y(0);
    // Leeren Vektor als Daten für Graphen setzen -> Er wird leer
    ui->widget->graph(0)->setData(x, y);
    // X Achsen counter resetten
    datenCounter = 0;
    // Achsen neu skalieren
    ui->widget->graph(0)->rescaleAxes();
    // Graph neu zeichen
    ui->widget->replot();
    // Werteliste bereinigen
    ourValues.clear();
}
