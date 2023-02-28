/*
 * lift_scheduler.c
 *
 *  Created on: Dec 17, 2022
 *      Author: LiricLiu
 */

#include "main.h"

#define STATUS_ACC 0
#define STATUS_RUN 1
#define STATUS_SLD 2
#define STATUS_STP 3
#define STATUS_OGT 4
#define STATUS_CGT 5

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1

unsigned int getLocation(){
	extern unsigned int m_currentLocation;
	return m_currentLocation;
}

unsigned int getPreSlowDownDistance(){
	return 0;
}

unsigned int getSpeed(){
	extern int m_currentSpeed;
	return m_currentSpeed>0?m_currentSpeed:-m_currentSpeed;
}

void setExpectedSpeed(unsigned int speed,unsigned char direction){
	extern unsigned int m_currentLocation;
	extern int m_currentAccl;
	extern int m_currentSpeed;
	m_currentSpeed=direction?-speed:speed;
	return;
}

/*
 * buttonPressed[0]：电梯内的按钮
 * buttonPressed[1]：电梯外的上行按钮
 * buttonPressed[2]：电梯外的下行按钮
 */
unsigned char buttonPressed[3][5]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t gatestat;

uint8_t getFloor(){
	extern unsigned int m_currentLocation;
	return m_currentLocation/FLOOR_HEIGHT_MM+1;
}
uint8_t getFloor1(uint8_t dir){
	extern unsigned int m_currentLocation;
	if(dir==0)
		return m_currentLocation/FLOOR_HEIGHT_MM+1;
	else
		return m_currentLocation/FLOOR_HEIGHT_MM+2;
}

unsigned int nextStopPosition(unsigned int position,unsigned char direction){
	unsigned int ret;
	if(direction==0){
		for(int i=getFloor1(0)-1;i<MAX_FLOOR;i++){
			if ((buttonPressed[1][i]==1)||(buttonPressed[0][i]==1)||(buttonPressed[2][i]==1)) {
				ret=i;
				return FLOOR_HEIGHT_MM*ret;
			}
		}
	}else{
		for(int i=getFloor1(1)-2;i>=0;i--){
			if ((buttonPressed[2][i]==1)||(buttonPressed[0][i]==1)||(buttonPressed[1][i]==1)) {
				ret=i;
				return FLOOR_HEIGHT_MM*ret;
			}
		}
	}
}
unsigned char ifAllReqDone(){
	for(int i=0;i<MAX_FLOOR;i++){
		for(int j=0;j<3;j++){
			if(buttonPressed[j][i]==1) return 0;
		}
	}
	return 1;
}

unsigned int getArrvlWhenDir2(){
	for(int i=0;i<MAX_FLOOR;i++){
		for(int j=0;j<3;j++){
			if(buttonPressed[j][i]==1) return i;
		}
	}
	return 0;
}
uint8_t gateFlag=0;
uint8_t emergency=0;

void liftScheduler(void* arg){
	unsigned int status=STATUS_STP;
	unsigned char direction=DIRECTION_UP;
	unsigned int lastSTPFloor=1;
	gateFlag=0;
	emergency=0;
	unsigned int location=0;
	unsigned int next_location=0;
	extern UART_HandleTypeDef huart4;
	setExpectedSpeed(0, 0);
	gatestat=2;
	for(;;)
	  {
	    osDelay(1);
	    //HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_2);
fuck:
		if(emergency==1){
			setExpectedSpeed(0, 0);
	    	status=STATUS_STP;
	    	uint8_t tempx=0x07;
	    	HAL_UART_Transmit(&huart4,&tempx, 1, 10);
	    	while(1);
	    	goto fuck;
	    }
	    switch(status){
	    case STATUS_ACC://加速
	    	//获取当前的速度
	    	break;
	    case STATUS_RUN://匀速
	    	if(direction==2){
	    		if(getArrvlWhenDir2()>getFloor()-1){
	    			direction=0;
	    		}else {
					direction=1;
				}
	    	}
	    	if(direction==0){
	    		if(getLocation()>=nextStopPosition(0, 0)){
	    			status=STATUS_STP;
	    			if(getFloor()!=lastSTPFloor) gateFlag=1;
	    			setExpectedSpeed(0, direction);
	    		}else{
	    			setExpectedSpeed(STABLE_SPEED_MM,direction);
	    		}
	    	}else{
	    		if(getLocation()<=nextStopPosition(0, 1)){
	    			status=STATUS_STP;
	    			if(getFloor()!=lastSTPFloor) gateFlag=1;
	    			setExpectedSpeed(0, direction);
	    		}else{
	    			setExpectedSpeed(STABLE_SPEED_MM,direction);
	    		}
	    	}
	    	break;
	    case STATUS_SLD://减速
	    	break;
	    case STATUS_STP://停靠
	    	buttonPressed[0][getFloor()-1]=0;//灭灯
	    	buttonPressed[1][getFloor()-1]=0;
	    	buttonPressed[2][getFloor()-1]=0;
	    	lastSTPFloor=getFloor();
	    	if(gateFlag==1&&getLocation()%FLOOR_HEIGHT_MM==0){
	    		unsigned short a=1;
	    		for(int i=0;i<getFloor()-1;i++){
	    			a*=2;
	    		}
	    		extern UART_HandleTypeDef huart5;
	    		uint8_t buff[5][7]={
		    	    0x7E,0x05,0x41,0x00,0x01,0x45,0xEF,//1
	    			0x7E,0x05,0x41,0x00,0x03,0x47,0xEF,//2
					0x7E,0x05,0x41,0x00,0x05,0x41,0xEF,//3
					0x7E,0x05,0x41,0x00,0x07,0x43,0xEF,//4
	    			0x7E,0x05,0x41,0x00,0x09,0x4D,0xEF//5
	    		};
	    		HAL_UART_Transmit(&huart5, buff[getFloor()-1], 7, 100);
	    		gatestat=0;
	    		uint8_t temp=0x02;
	    		HAL_UART_Transmit(&huart4,&temp, 1, 100);
kaimen:
	    		for(int i=0;i<100;i++){
	    			osDelay(50);
	    			if(OPEN_BTN()){
	    				gatestat=0;
	    				goto kaimen;
	    			}
	    			if(CLOSE_BTN()){
	    				gatestat=1;
	    				temp=0x03;
	    				HAL_UART_Transmit(&huart4,&temp, 1, 10);
	    				goto guanmen;
	    			}
	    		}
	    		gatestat=1;
	    		temp=0x03;
	    		HAL_UART_Transmit(&huart4,&temp, 1, 100);
guanmen:
				for(int i=0;i<100;i++){
	    			osDelay(50);
	    			if(OPEN_BTN()){
	    				temp=0x02;
	    				HAL_UART_Transmit(&huart4,&temp, 1, 10);
	    				gatestat=0;
	    				goto kaimen;
	    			}
	    			if(CLOSE_BTN()){
	    				gatestat=1;
	    				goto guanmen;
	    			}
	    		}
	    		temp=0x05;
	    		HAL_UART_Transmit(&huart4,&temp, 1, 100);
	    		gatestat=2;
	    		gateFlag=0;
	    	}
	    	if(ifAllReqDone()==1){
	    		uint8_t temp=0x05;
	    		HAL_UART_Transmit(&huart4,&temp, 1, 100);
	    		direction=2;
	    		if(OPEN_BTN()) {
	    			temp=0x02;
	    			HAL_UART_Transmit(&huart4,&temp, 1, 100);
	    			goto kaimen;
	    		}
	    	}else {
				status=STATUS_RUN;//切换到运行状态
				//换向
				if(direction==0){//检测上方楼层是否有请求
					for(int i=getFloor();i<MAX_FLOOR;i++){
						for(int j=0;j<3;j++){
							if(buttonPressed[j][i]==1){
								goto dontChangeDir;
							}
						}
					}
				}else{
					if(direction==1){
						for(int i=0;i<getFloor();i++){
							for(int j=0;j<3;j++){
								if(buttonPressed[j][i]==1){
									goto dontChangeDir;
								}
							}
						}
					}
				}
				(direction==0)?(direction=1):(direction=0);
dontChangeDir:
				if(getFloor()==MAX_FLOOR){//
					direction=1;
				}
				if(getFloor()==1){
					direction=0;
				}
			}
	    	if(status==STATUS_RUN){
	    		uint8_t temp=((direction==0)?0x06:0x08);
	    		HAL_UART_Transmit(&huart4,&temp, 1, 100);
	    	}
	    	break;
	    case STATUS_OGT://开门
	    	break;
	    case STATUS_CGT://关门
	    	break;
	    }

	  }
}
