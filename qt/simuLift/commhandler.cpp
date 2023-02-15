#include "commhandler.h"


CommHandler::CommHandler()
{
    //串口初始化
    serial=new QSerialPort();
}


void CommHandler::onRX_1byte(unsigned char c){
    if(c>0x80){//说明传输的是数据
        if(dataByteCounter>=MAX_DATA_LENGTH){
            //警告：请检查设备的连接状态
        }else{
            buffer[dataByteCounter]=c-0x80;
        }
        dataByteCounter++;
    }else{//说明传输的是指令
        switch (c) {
        case 0x01://位置报告指令
            dataByteCounter=0;
            //显示
            break;
        case 0x02://开门报告指令
            break;
        case 0x03://关门报告指令
            break;
        case 0x04://报警指令
            break;
        default:
            break;
        }
    }
    return;
}
