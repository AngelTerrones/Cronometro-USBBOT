#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSerialPort>
#include <QTimer>
#include <QTime>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    protected:
        void closeEvent(QCloseEvent *event);
    private slots:
        void slotHabilitar();
        void slotReiniciar();
        void slotDetener();
        void slotShowTime();
        void slotRecibirSerial();
    private:
        void ConnectSS();
        void GetSerialPortList();
        void UpdateList();

        bool enabled;
        bool ignoreSerial;
        int m_lap;
        QSerialPort m_serialPort;
        QTimer m_timer;
        QTime m_timePartial;
        QTime m_timeGlobal;
};

#endif // MAINWINDOW_H
