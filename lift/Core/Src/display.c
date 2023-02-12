/*
 * display.c
 *
 *  Created on: Dec 21, 2022
 *      Author: LiricLiu
 */

/*
 * lcd_drv.c
 *	ST7789V SPI clock freq in read mode is up to 15MHz, while write mode is up to 60MHz.
 *  Created on: Jul 24, 2022
 *      Author: liricliu
 */

#include "main.h"
#include "config.h"

SPI_HandleTypeDef* _hspi1_=NULL;
uint8_t graphicFrame[320][170][3];

uint8_t Shuzibiao[10][7]={
	0x01,0x01,0x01,0x00,0x01,0x01,0x01,//0
	0x00,0x00,0x01,0x00,0x00,0x01,0x00,//1
	0x01,0x00,0x01,0x01,0x01,0x00,0x01,//2
	0x01,0x00,0x01,0x01,0x00,0x01,0x01,//3
	0x00,0x01,0x01,0x01,0x00,0x01,0x00,//4
	0x01,0x01,0x00,0x01,0x00,0x01,0x01,//5
	0x01,0x01,0x00,0x01,0x01,0x01,0x01,//6
	0x01,0x00,0x01,0x00,0x00,0x01,0x00,//7
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,//8
	0x01,0x01,0x01,0x01,0x00,0x01,0x01 //9
};


#define WriteComm(a) lcddSendCommand(a)
#define WriteData(a) lcddSendData(a)

void lcddSendCommand(uint8_t a){
	//uint8_t* address = 0x60000000;
	//HAL_SRAM_Write_8b(_hsram1_, address, &a, 1);
	//*address=a;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
	HAL_SPI_Transmit(_hspi1_, &a, 1, 10);
}

void lcddSendData(uint8_t a){
	//uint8_t* address1 = 0x60010000;
	//*address1=a;
	//HAL_SRAM_Write_8b(_hsram1_, address1, &a, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
	HAL_SPI_Transmit(_hspi1_, &a, 1, 10);
}

void lcddSendRGB(uint8_t r,uint8_t g,uint8_t b){
	WriteComm(0x2c);
	WriteData(r<<2);
	WriteData(g<<2);
	WriteData(b<<2);
}

void lcddRefresh(uint8_t* gBuffer){
	if(gBuffer==NULL) gBuffer=(uint8_t*)graphicFrame;
	else gBuffer=(uint8_t*)graphicFrame;
	lcddSendCommand(0x2c);//send color command
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
	HAL_SPI_Transmit(_hspi1_, gBuffer, 65535, 10);
	gBuffer+=65535;
	HAL_SPI_Transmit(_hspi1_, gBuffer, 65535, 10);
	gBuffer+=65535;
	HAL_SPI_Transmit(_hspi1_, gBuffer, 32130, 10);

	//}
}

void lcddReset(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 0);
	osDelay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
	lcddRefresh(NULL);
}

void lcddSetup(SPI_HandleTypeDef* h){
	if(_hspi1_!=NULL) return;
	_hspi1_=h;
	//uint8_t* aaa=0x60000000;
	//*aaa=0x11;
	//while(1) {WriteComm(0x00);WriteComm(0xff);}
	WriteComm(0x11);
	osDelay(120);

	WriteComm(0x36);//MADCTL,设置显存增长方向
	WriteData(0x00);//全部正
	WriteComm(0x3a);//设置颜色格式
	WriteData (0x06);//0x06为18位模式

	WriteComm(0x2a);//设置横向显存范围
	WriteData (0x00);
	WriteData (0x23);
	WriteData (0x00);
	WriteData (0xcc);//239=ef
	WriteComm(0x2b);//设置纵向显存范围
	WriteData (0x00);
	WriteData (0x00);
	WriteData (0x01);
	WriteData (0x3f);//319

	WriteComm(0xb2);//porh setting
	WriteData (0x0c);
	WriteData (0x0c);
	WriteData (0x00);
	WriteData (0x33);
	WriteData (0x33);

	WriteComm(0xb7); //Gate Control
	WriteData (0x35);

	WriteComm(0xbb); //vcom setting
	WriteData (0x2e);

	WriteComm(0xc5);   //Vcom OFFSET Setting
	WriteData (0x4a);  //PLEASE TUNING VCOM OFFSET FOR MIN FLICKER


	WriteComm(0x35); //Tearing Effect
	WriteData (0x00);


	WriteComm(0xc0);//LCMCTRL
	WriteData (0x2c);

	WriteComm(0xc2);
	WriteData (0x01);

	WriteComm(0xc3);
	WriteData (0x1f);

	WriteComm(0xc4);
	WriteData (0x20);

	WriteComm(0xc6);
	WriteData (0x0f);	//	ef

	WriteComm(0xd0);
	WriteData (0xa4);
	WriteData (0xa1);

	WriteComm(0xe9);
	WriteData(0x11);
	WriteData(0x11);
	WriteData(0x02);

	WriteComm(0xe0);
	WriteData (0xf0);
	WriteData (0x11);
	WriteData (0x17);
	WriteData (0x0a);
	WriteData (0x0b);
	WriteData (0x06);
	WriteData (0x3a);
	WriteData (0x33);
	WriteData (0x4f);
	WriteData (0x06);
	WriteData (0x14);
	WriteData (0x1a);
	WriteData (0x2f);
	WriteData (0x34);

	WriteComm(0xe1);
	WriteData (0xf0);
	WriteData (0x11);
	WriteData (0x17);
	WriteData (0x0a);
	WriteData (0x0b);
	WriteData (0x06);
	WriteData (0x3a);
	WriteData (0x33);
	WriteData (0x4f);
	WriteData (0x06);
	WriteData (0x14);
	WriteData (0x1a);
	WriteData (0x2f);
	WriteData (0x34);

	WriteComm(0x21);
	WriteComm(0x29);

	for(int i=0;i<320;i++){
		for(int j=0;j<170;j++){
			graphicFrame[i][j][0]=0x66;
			graphicFrame[i][j][1]=0x33;
			graphicFrame[i][j][2]=0x00;
		}
	}
	lcddRefresh(graphicFrame);
}
#define gemGUI_GMEM_ARRAY graphicFrame
void displayDigit(uint8_t d){
	if(d>9) return;
	if(Shuzibiao[d][0]){
		for(int i=17;i<25;i++){
			for(int j=37;j<133;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][1]){
		for(int i=17;i<113;i++){
			for(int j=37;j<45;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][2]){
		 for(int i=17;i<113;i++){
			for(int j=125;j<133;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][3]){
		for(int i=105;i<113;i++){
			for(int j=37;j<133;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][4]){
		for(int i=113;i<199;i++){
			for(int j=37;j<45;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][5]){
		for(int i=113;i<199;i++){
			for(int j=125;j<133;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}
	if(Shuzibiao[d][6]){
		for(int i=199;i<207;i++){
			for(int j=37;j<133;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}
	}

}
void displayDirection(uint8_t dir,uint8_t slc){
	if(dir==2) return;
	if(dir==0){//上升
		switch(slc%3){
		case 0:
			for(int i=266;i<301;i++){
				for(int j=86+i-266;j>86-i+266;j--){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			break;
		case 1:
			for(int i=231;i<266;i++){
				for(int j=86+i-231;j>86-i+231;j--){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			for(int i=266;i<301;i++){
				for(int j=66;j<106;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			break;
		case 2:
			for(int i=231;i<266;i++){
				for(int j=66;j<106;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
		}
		/*for(int i=231;i<301;i++){
			for(int j=51;j<121;j++){
				graphicFrame[i][j][0]=0xff;
				graphicFrame[i][j][1]=0xff;
				graphicFrame[i][j][2]=0xff;
			}
		}*/
	}else{//下降
		switch(slc%3){
		case 0:
			for(int i=231;i<266;i++){
				for(int j=51+i-231;j<121+231-i;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			break;
		case 1:
			for(int i=266;i<301;i++){
				for(int j=51+i-266;j<121+266-i;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			for(int i=231;i<266;i++){
				for(int j=66;j<106;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
			break;
		case 2:
			for(int i=266;i<301;i++){
				for(int j=66;j<106;j++){
					graphicFrame[i][j][0]=0xff;
					graphicFrame[i][j][1]=0xff;
					graphicFrame[i][j][2]=0xff;
				}
			}
		}
	}

}

uint8_t getCurrentFloor(){
	extern unsigned int m_currentLocation;
	return m_currentLocation/FLOOR_HEIGHT_MM+1;
}
uint8_t getCurrentDirection(){
	extern int m_currentSpeed;
	return m_currentSpeed==0?2:(m_currentSpeed>0?0:1);
}


void display(void* arg){
	int x=0;
	lcddSetup((SPI_HandleTypeDef*)arg);

	while(1){
		for(int i=0;i<320;i++){
			for(int j=0;j<170;j++){
				graphicFrame[i][j][0]=0x66;
				graphicFrame[i][j][1]=0x33;
				graphicFrame[i][j][2]=0x00;
			}
		}
		displayDigit(getCurrentFloor());
		displayDirection(getCurrentDirection(), x);
		x++;
		if(x>8) x=0;
		//osDelay(100);
		//gemGUIPutchar0(getCurrentFloor(), 0, 0,0xff,0xff,0xff);
		lcddRefresh(graphicFrame);
	}
}
