#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QFile>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息
#include "commhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void DataReceived();
    void onRX_1byte(unsigned char c);

private:
    Ui::MainWindow *ui;
    QSerialPort* com;
    QFile* logfile;

    unsigned char buffer[MAX_COMMAND_SIZE];
    unsigned char dataByteCounter;//数据字节计数器

    long long timestamp;
    int lastHeight;
    unsigned char lastC;

    void log();
private slots:
    void comConnect();
    void comRefresh();
    void Zhidong();
};

#endif // MAINWINDOW_H
