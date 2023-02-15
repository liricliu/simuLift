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
void step(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
	for(int t=0;t<4;t++){
		osDelay(5);
			if(t%2){
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			}else{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
			}
	}
}
void step1(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
	for(int t=0;t<4;t++){
		osDelay(5);
			if(t%2){
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			}else{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
			}
	}
}
void virtualMachine(void* arg){
	//currentLocation=0;
	//currentAccl=0;
	//currentSpeed=0;
	unsigned char t=0;
	m_currentLocation=0;
	while(1){
		if(m_currentSpeed>0){
			step();
		}else{
			if(m_currentSpeed<0){
				step1();
			}else{
				osDelay(20);
			}
		}
		//currentSpeed+=currentAccl/100;
		m_currentLocation+=m_currentSpeed/50;
	}
}
