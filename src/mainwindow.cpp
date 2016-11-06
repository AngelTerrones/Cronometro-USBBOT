#include <QHeaderView>
#include <QKeySequence>
#include <QSerialPortInfo>
#include <QString>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    ConnectSS();

    lcdTime->display("00.000");

    twTiempo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    twTiempo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    twTiempo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    GetSerialPortList();

    enabled = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(enabled){
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::slotHabilitar()
{
    if(!enabled){
        m_serialPort.setPortName(cbSerialPort->currentText());
        m_serialPort.setBaudRate(QSerialPort::Baud9600);
        if(!m_serialPort.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, "Error", "No se puede abrir el puerto serial seleccionado");
            return;
        }
        pbHabilitar->setText("D&eshabilitar");
        pbHabilitar->setShortcut(QKeySequence("Ctrl+H"));
        pbReiniciar->setEnabled(true);
        pbDetener->setEnabled(true);
        enabled = true;
        slotReiniciar();
    }else{
        slotDetener();
        pbHabilitar->setText("&Habilitar");
        pbHabilitar->setShortcut(QKeySequence("Ctrl+H"));
        pbReiniciar->setEnabled(false);
        pbDetener->setEnabled(false);
        m_serialPort.close();
        enabled = false;
    }
}

void MainWindow::slotReiniciar()
{
    if(!enabled)
        return;
    qDebug() << "reiniciar";
    m_serialPort.readAll();
    m_lap = 0;
    lcdTime->display("00.000");
    m_timer.stop();
    twTiempo->clearContents();
    ignoreSerial = false;
}

void MainWindow::slotDetener()
{
    if(!enabled)
        return;
    qDebug() << "Stop";
    ignoreSerial = true;
    m_timer.stop();
}

void MainWindow::slotShowTime()
{
    QString tmp = QTime(0, 0, 0, 0).addMSecs(m_timePartial.elapsed()).toString("ss.zzz");
    lcdTime->display(tmp);
}

void MainWindow::slotRecibirSerial()
{
    QByteArray token = m_serialPort.readAll();
    if(ignoreSerial)
        return;

    if(token == "\xFF"){
        if(m_lap == 0){
            qDebug() << "Start timer";
            m_timePartial.restart();
            m_timeGlobal.restart();
            m_timer.start(10);
            m_lap++;
        }else{
            qDebug() << "Lap";
            UpdateList();
            m_timePartial.restart();
            m_lap++;
        }
    }
}

void MainWindow::ConnectSS()
{
    connect(pbHabilitar, SIGNAL(clicked()), this, SLOT(slotHabilitar()));
    connect(pbReiniciar, SIGNAL(clicked()), this, SLOT(slotReiniciar()));
    connect(pbDetener, SIGNAL(clicked()), this, SLOT(slotDetener()));
    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(slotRecibirSerial()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotShowTime()));
}

void MainWindow::GetSerialPortList()
{
    const auto spInfos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &spInfo : spInfos){
        QString port = spInfo.portName();
        cbSerialPort->addItem(port);
    }
}

void MainWindow::UpdateList()
{
    QString lap = QTime(0, 0, 0, 0).addMSecs(m_timePartial.elapsed()).toString("ss.zzz");
    QString total = QTime(0, 0, 0, 0).addMSecs(m_timeGlobal.elapsed()).toString("ss.zzz");

    qDebug() << m_lap << lap << total;

    QTableWidgetItem *n_lap = new QTableWidgetItem(QString("%1").arg(m_lap));
    QTableWidgetItem *t_lap = new QTableWidgetItem(lap);
    QTableWidgetItem *t_glob = new QTableWidgetItem(total);
    twTiempo->setItem(m_lap - 1, 0, n_lap);
    twTiempo->setItem(m_lap - 1, 1, t_lap);
    twTiempo->setItem(m_lap - 1, 2, t_glob);
}

