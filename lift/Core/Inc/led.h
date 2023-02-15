/*
 * led.h
 *
 *  Created on: Dec 21, 2022
 *      Author: LiricLiu
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#define LIFT_F1_LED(ctrl) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,!ctrl)
#define LIFT_F2_LED(ctrl) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,!ctrl)
#define LIFT_F3_LED(ctrl) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,!ctrl)
#define LIFT_F4_LED(ctrl) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,!ctrl)
#define LIFT_F5_LED(ctrl) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,!ctrl)

#define LIFT_F1_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
#define LIFT_F2_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)
#define LIFT_F3_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)
#define LIFT_F4_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
#define LIFT_F5_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)

#define F1_UP_LED(ctrl) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,!ctrl)
#define F2_UP_LED(ctrl) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,!ctrl)
#define F3_UP_LED(ctrl) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,!ctrl)
#define F4_UP_LED(ctrl) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,!ctrl)

#define F5_DOWN_LED(ctrl) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,!ctrl)
#define F2_DOWN_LED(ctrl) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,!ctrl)
#define F3_DOWN_LED(ctrl) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,!ctrl)
#define F4_DOWN_LED(ctrl) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,!ctrl)

#define F1_UP_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)
#define F2_UP_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)
#define F3_UP_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)
#define F4_UP_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)

#define F2_DOWN_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)
#define F3_DOWN_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)
#define F4_DOWN_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)
#define F5_DOWN_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)

#define RST_BTN() !HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define ALM_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define OPEN_BTN() !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define CLOSE_BTN() !HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)

#endif /* INC_LED_H_ */
