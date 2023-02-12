/*
 * mchn_ctrl.c
 *
 *  Created on: Dec 21, 2022
 *      Author: LiricLiu
 */
#include "config.h"
#include "main.h"

unsigned int m_currentLocation;
int m_currentAccl;
int m_currentSpeed;

unsigned char lock;

void virtualMachine(void* arg){
	//currentLocation=0;
	//currentAccl=0;
	//currentSpeed=0;
	m_currentLocation=0;
	while(1){
		osDelay(10);
		//currentSpeed+=currentAccl/100;
		m_currentLocation+=m_currentSpeed/100;
	}
}
