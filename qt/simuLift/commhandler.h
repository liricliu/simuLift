#ifndef COMMHANDLER_H
#define COMMHANDLER_H

#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息

#define MAX_COMMAND_SIZE 5
#define MAX_DATA_LENGTH 4

class CommHandler
{
public:
    CommHandler();
    void onRX_1byte(unsigned char c);
    QSerialPort     *serial;                            // 定义全局的串口对象（第三步）

private:
    unsigned char buffer[MAX_COMMAND_SIZE];
    unsigned char dataByteCounter;//数据字节计数器
};

#endif // COMMHANDLER_H
