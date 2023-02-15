#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    ui->textBrowser->setSource(QUrl("file://"+QCoreApplication::applicationDirPath()+QString::fromUtf8("/currentlog")));
    com=new QSerialPort;
    comRefresh();
    com->setBaudRate(QSerialPort::Baud115200);
    com->setParity(QSerialPort::NoParity);
    com->setDataBits(QSerialPort::Data8);
    com->setStopBits(QSerialPort::OneStop);
    com->setFlowControl(QSerialPort::NoFlowControl);

    logfile=new QFile(QCoreApplication::applicationDirPath()+QString::fromUtf8("/currentlog"));

    connect(com,&QSerialPort::readyRead,this,&MainWindow::DataReceived);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::comConnect);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::comRefresh);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::Zhidong);
    dataByteCounter=0;
    lastHeight=0;
    lastC=0x05;
    timestamp=QDateTime::currentMSecsSinceEpoch();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete com;
}

void MainWindow::comConnect(){
    if(ui->ComBox->currentText()=="--请选择设备--"){
        QMessageBox::warning(this,tr("提示"),tr("请在下拉框中选择设备"),QMessageBox::Ok);
        return;
    }
    com->setPortName(ui->ComBox->currentText());
    if(!com->open(QIODevice::ReadWrite)){
        QMessageBox::warning(this,tr("提示"),tr("串口打开失败!"),QMessageBox::Ok);
        return;
    }
}

void MainWindow::comRefresh(){
    ui->ComBox->clear();
    ui->ComBox->addItem("--请选择设备--");
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        com->setPort(info);                      // 在对象中设置串口
        if(com->open(QIODevice::ReadWrite)){
            if(info.portName().contains("tty.usbserial"))
                ui->ComBox->addItem(info.portName());  // 添加计算机中的端口
            com->close();                        // 关闭
        }
    }
    if(ui->ComBox->count()==1){
        ui->ComBox->clear();
        ui->ComBox->addItem("未检测到设备");
    }
    ui->textBrowser->reload();
}
void MainWindow::DataReceived()
{
    QByteArray data = com->readAll();                      // 读取数据
    if(!data.isEmpty())                                 // 接收到数据
    {
        for(int i=0;i<data.size();i++){
            onRX_1byte((unsigned char)data[i]);
        }
    }
}
void MainWindow::Zhidong(){
    com->write("\x01",1);
}
void MainWindow::log(){
    QString tolog;
    logfile->open(QIODevice::Append);
    logfile->write((QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss]").toUtf8()));
    switch (lastC) {
    case 0x02:
        tolog="Openning the door<br>\n";
        break;
    case 0x03:
        tolog="Closing the door<br>\n";
        break;
    case 0x04:
        tolog="Alarm Triggerred<br>\n";
        break;
    case 0x05:
        tolog="Stopping on F"+(QString::number(ui->lcdNumber_3->intValue())+"<br>\n");
        break;
    case 0x06:
        tolog="Start Running<br>\n";
        break;
    case 0x07:
        tolog="Emergency program started<br>\n";
        break;
    }
    logfile->write(tolog.toUtf8());
    logfile->close();

    ui->textBrowser->reload();
}
void MainWindow::onRX_1byte(unsigned char c){
    if(c>0x80){//说明传输的是数据
        if(dataByteCounter>=MAX_DATA_LENGTH){
            //警告：请检查设备的连接状态
        }else{
            buffer[dataByteCounter]=c-0x80;
        }
        dataByteCounter++;
    }else{//说明传输的是指令
        int toDisplay=0;
        long long currentTS=0;
        switch (c) {
        case 0x01://位置报告指令
            for(int i=0;i<dataByteCounter;i++){
                toDisplay+=buffer[i]*pow(128,dataByteCounter-i-1);
            }
            ui->lcdNumber->display((double)toDisplay/1000);
            ui->lcdNumber_3->display(toDisplay/3600+1);
            currentTS=QDateTime::currentMSecsSinceEpoch();
            ui->lcdNumber_2->display(((double)(toDisplay-lastHeight)/(double)(currentTS-timestamp)));
            timestamp=currentTS;
            lastHeight=toDisplay;
            dataByteCounter=0;
            //显示
            break;
        case 0x02://开门报告指令
            ui->label_2->setText("开门中");
            ui->label_2->setStyleSheet("color:blue;");
            lastC=0x02;
            log();
            break;
        case 0x03://关门报告指令
            ui->label_2->setText("关门中");
            ui->label_2->setStyleSheet("color:blue;");
            lastC=0x03;
            log();
            break;
        case 0x04://报警指令
            QMessageBox::critical(this,tr("警报"),tr("收到用户报警"),QMessageBox::Ok);
            lastC=0x04;
            log();
            break;
        case 0x05://停止报告指令
            ui->label_2->setText("电梯停止");
            ui->label_2->setStyleSheet("color:red;");
            if(lastC!=0x05){
                lastC=0x05;
                log();
            }
            break;
        case 0x06://运行报告指令
            ui->label_2->setText("电梯运行中");
            ui->label_2->setStyleSheet("color:green;");
            if(lastC!=0x06){
                lastC=0x06;
                log();
            }

            break;
        case 0x07:
            ui->label_2->setText("已制动，请在维修后重启系统");
            ui->label_2->setStyleSheet("color:red;");
            lastC=0x07;
            log();
            break;
        default:
            break;
        }
    }
    return;
}
