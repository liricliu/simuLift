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
	return 0;
}

unsigned int getPreSlowDownDistance(){
	return 0;
}

unsigned int getSpeed(){
	return 0;
}

void setExpectedSpeed(unsigned int speed,unsigned char direction){

}

/*
 * buttonPressed[0]：电梯内的按钮
 * buttonPressed[1]：电梯外的上行按钮
 * buttonPressed[2]：电梯外的下行按钮
 */
unsigned char buttonPressed[3][5]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned int nextStopPosition(unsigned int position,unsigned char direction){
	return position;
}

void liftScheduler(void* arg){
	unsigned int status=STATUS_STP;
	unsigned char direction=DIRECTION_UP;
	unsigned int location=0;
	unsigned int pre_distance=0;
	for(;;)
	  {
	    osDelay(1);
	    HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_2);
	    switch(status){
	    case STATUS_ACC://加速
	    	//获取当前的速度
	    	location=getLocation();
	    	pre_distance=getPreSlowDownDistance();
	    	if(location+pre_distance>=nextStopPosition());
	    	break;
	    case STATUS_RUN://匀速
	    	break;
	    case STATUS_SLD://减速
	    	break;
	    case STATUS_STP://停靠
	    	break;
	    case STATUS_OGT://开门
	    	break;
	    case STATUS_CGT://关门
	    	break;
	    }

	  }
}
