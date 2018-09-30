#define PART_TM4C1294NCPDT
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "hw_i2c.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "i2c.h"
#include "pin_map.h"
#include "sysctl.h"
#include "hw_sysctl.h"
#include "uart.h"

#include "hw_ints.h"
#include "systick.h"
#include "interrupt.h"

#define SYSTICK_FREQUENCY 1000 //1000 Hz
#define MY_FREQUENCY 5 //Sampling rate 5HZ

//*******************************lqh*******************************************
#define SMPLRT_DIV		0x19 //陀螺仪采样率，典型值：0x07（125HZ）
#define CONFIG				0x1A //低通滤波频率，典型值：0x06（5Hz）
#define GYRO_CONFIG		0x1B //陀螺仪自检及测量范围，典型值：0x18（不自检，2000deg/s）
#define ACCEL_CONFIG	0x1C //加速计自检，测量范围及高通滤波频率，典型值：0x01（不自检，2G，5Hz）

#define GYRO_ADDRESS		0x68 //7 bit b1101000
#define ACCEL_ADDRESS	0x68 //7 
#define MAG_ADDRESS		0x0C //7 bit b0001100

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x68，只读)

//*******************************lqh*******************************************

//*******************************lqh*******************************************
#define HMC5983_ADDRESS 0x1E  //I2C address

#define HMC5983_CONFIG_A 	0x00	//Config A, Rate and Mode
#define HMC5983_CONFIG_B 	0x01	//Gain & Resolution
#define HMC5983_MODE			0x02	//Mode(1:Single or 0:Continuous)
#define HMC5983_DXH				0x03	//Data X MSB
#define HMC5983_DXL				0x04	//Data X LSB
#define HMC5983_DZH				0x05	//Data Z MSB
#define HMC5983_DZL				0x06	//Data Z LSB
#define HMC5983_DYH				0x07	//Data Y MSB
#define HMC5983_DYL				0x08	//Data Y LSB
#define HMC5983_STATUS		0x09	//Status Reg,
#define HMC5983_ID_A			0x0A	//ID Reg A:0x48
#define HMC5983_ID_B			0x0B	//ID Reg B:0x34
#define HMC5983_ID_C			0x0C	//ID Reg C:0x33
#define HMC5983_TH				0x31	//Temperature MSB
#define HMC5983_TL				0x32	//Temperature LSB
//*******************************lqh*******************************************


//*****************************************************************************
//
//I2C GPIO chip address and resigster define
//
//*****************************************************************************
#define TCA6424_I2CADDR 					0x22
#define PCA9557_I2CADDR						0x18

#define PCA9557_INPUT							0x00
#define	PCA9557_OUTPUT						0x01
#define PCA9557_POLINVERT					0x02
#define PCA9557_CONFIG						0x03

#define TCA6424_CONFIG_PORT0			0x0c
#define TCA6424_CONFIG_PORT1			0x0d
#define TCA6424_CONFIG_PORT2			0x0e

#define TCA6424_INPUT_PORT0				0x00
#define TCA6424_INPUT_PORT1				0x01
#define TCA6424_INPUT_PORT2				0x02

#define TCA6424_OUTPUT_PORT0			0x04
#define TCA6424_OUTPUT_PORT1			0x05
#define TCA6424_OUTPUT_PORT2			0x06




void 		Delay(uint32_t value);
void 		S800_GPIO_Init(void);

uint8_t I2C_ReadWriteByte(int, uint8_t, uint8_t,bool,uint8_t);//general function

uint8_t 	I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);

uint8_t 	I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);

uint8_t I2C_WriteByte(int I2C_Num, uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t I2C_ReadByte(int I2C_Num, uint8_t DevAddr, uint8_t RegAddr);
//--------
void Read_Accel(int); 	//for MPU9250, input:I2C_Num
void Read_Gyro(int);	//for MPU9250, input:I2C_Num
void Read_Mag(int);	//for MPU9250, input:I2C_Num
void MPU9250_Read(int);	//Read data from MPU9250 to agm and convert to numTrans

void TCA6424_Config(int);	//for TCA6424, input: I2C_Num
void PCA9557_Config(int);	//for PCA9557, input: I2C_Num
//--------


void display(short);

void UARTStringPut(const char *);

void S800_UART_Init(void);

void S800_I2C0_Init(void);

//------
void S800_I2C_Init(uint16_t);	//I2C_Enable_Mask

void MPU9250_Config(int);	//MPU9250 Config,input I2C_Num

void HMC5983_Config(int);	//HMC5983 Config,input I2C_Num

void HMC5983_Read(int);		//HMC5983 read data, input I2C_Num

//------

volatile uint8_t result;
uint32_t ui32SysClock;
uint8_t seg7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x079,0x71,0x5c};

//--------
void dataConv(short*,int,int); //extract data from 'agm', convert to char, put into 'numTrans'
char numTrans[66]; //store data for UART, ([1 sign]+[5 digit]+[1 space])*9 + [1 I2C_num] + ['\n'] + ['\0']

uint8_t buf[10];
short G_X,G_Y,G_Z;
short A_X,A_Y,A_Z;
short M_X,M_Y,M_Z;


short agm[9];				//temperary variable to store data
uint32_t cnt=0;
uint8_t led=0xff;
uint16_t I2C_Enable_Mask=0x12FF;	//I2C [0,1,2,3 | 4,5,6,7| 8 9 2' 7'| 8']
//--------

uint32_t mycnt=0;

volatile uint16_t systick_10ms_couter,systick_myfreq_couter;
volatile uint8_t systick_10ms_status,systick_myfreq_status;

int led_status = 0;
int main(void)
{
		//use internal 16M oscillator, HSI
		ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_INT |SYSCTL_USE_OSC), 16000000);		
		//S800_GPIO_Init();
		//S800_I2C0_Init();
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);				//Enable PortN
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));			//Wait for the GPIO moduleN ready
		
		SysTickPeriodSet(ui32SysClock/SYSTICK_FREQUENCY);
		SysTickEnable();
		SysTickIntEnable();																		//Enable Systick interrupt
		
		GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
		
		S800_I2C_Init(I2C_Enable_Mask);
		
		HMC5983_Config(0);
	
		MPU9250_Config(0);
	
		HMC5983_Config(1);
		HMC5983_Config(2);
		HMC5983_Config(3);
		HMC5983_Config(4);
		HMC5983_Config(5);
		HMC5983_Config(6);
		HMC5983_Config(7);
		HMC5983_Config(8);
		HMC5983_Config(9);
	
		S800_UART_Init();
	
		G_X=0;
	
		while (1)	{
			
			//display(mycnt);
			//I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x40);
			//I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x40);
			//Delay(800000);
			//I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);
			//Read_Gyro();
			//I2C0_WriteByte(0x00,PCA9557_OUTPUT,0xff);	
			//I2C2_ReadByte(GYRO_ADDRESS,WHO_AM_I);
			//I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0x00);
			//I2C0_ReadByte(PCA9557_I2CADDR,PCA9557_INPUT);
			//I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0x01);
			
			if(systick_myfreq_status)
			{
				systick_myfreq_status=0;
				//Delay(500000);
				HMC5983_Read(0);
				//MPU9250_Read(0);
				UARTStringPut(numTrans);
				HMC5983_Read(1);
				UARTStringPut(numTrans);
				HMC5983_Read(2);
				UARTStringPut(numTrans);
				HMC5983_Read(3);
				UARTStringPut(numTrans); 
				HMC5983_Read(4);
				UARTStringPut(numTrans);
				HMC5983_Read(5);
				UARTStringPut(numTrans);
				HMC5983_Read(6);
				UARTStringPut(numTrans);
				HMC5983_Read(7);
				UARTStringPut(numTrans);
				HMC5983_Read(8);
				UARTStringPut(numTrans);
				HMC5983_Read(9);
				UARTStringPut(numTrans);
				MPU9250_Read(0);
				UARTStringPut(numTrans);
			}
		}
}

void SysTick_Handler(void)
{
	if (systick_myfreq_couter != 0)
		systick_myfreq_couter--;
	else
	{
		systick_myfreq_couter = SYSTICK_FREQUENCY/MY_FREQUENCY;
		systick_myfreq_status = 1;
	}
	
	if (systick_10ms_couter	!= 0)
		systick_10ms_couter--;
	else
	{
		systick_10ms_couter = SYSTICK_FREQUENCY/100;
		systick_10ms_status = 1;
	}		
}

void HMC5983_Config(int I2C_Num)
{
	I2C_WriteByte(I2C_Num,HMC5983_ADDRESS,HMC5983_CONFIG_A,0x70);	//8-average, 15Hz default,normal measurement
	Delay(160000);
	I2C_WriteByte(I2C_Num,HMC5983_ADDRESS,HMC5983_CONFIG_B,0x00);	//Gain=2,
	Delay(160000);
	//I2C_WriteByte(I2C_Num,HMC5983_ADDRESS,HMC5983_MODE,0x00);	//Continuous measurement mode
}

void HMC5983_Read(int I2C_Num)
{
	//buf[0] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_CONFIG_A);
	//buf[8] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_ID_A);	//ID A:0x48
	I2C_WriteByte(I2C_Num,HMC5983_ADDRESS,HMC5983_MODE,0x01);		//Single Measurement
	Delay(2000);
	buf[0] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DXH);		//Read XH
	buf[1] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DXL);		//Read XL
	buf[2] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DZH);		//Read ZH
	buf[3] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DZL);		//Read ZL
	buf[4] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DYH);		//Read YH
	buf[5] = I2C_ReadByte(I2C_Num,HMC5983_ADDRESS,HMC5983_DYL);		//Read YL
	
	
	
	M_X = (buf[0]<<8)| buf[1];
	M_Y = (buf[4]<<8)| buf[5];
	M_Z = (buf[2]<<8)| buf[3];
	agm[0] = M_X; agm[1] = M_Y; agm[2] = M_Z;
	//agm[0] = M_Z;
	dataConv(agm, 3, I2C_Num);
}

//convert from short array to string for UART, and append I2C number
//data:short array, n:arrary length, I2C_Num: 0<=I2C_Num<=9
void dataConv(short * data,int n,int I2C_Num)
{
	int i,j;short tmp;
	for(i=0;i<n;i++)
	{
		tmp = data[i];
		if(data[i]<0)
		{
			numTrans[i*7]='-';
			tmp= -tmp;
		}
		else{
			numTrans[i*7]='0';
		}
		numTrans[i*7+6]=' ';
		for(j=5;j>0;j--)
		{
			numTrans[i*7+j]='0'+tmp%10;
			tmp=tmp/10;
		}
	}
	if(I2C_Num<10)
		numTrans[n*7]='0'+I2C_Num;
	else
		numTrans[n*7]='A'+I2C_Num-10;
	numTrans[n*7+1]='\n';
	numTrans[n*7+2]='\0';
}
void display(short data)
{
			if(data<0)
			{
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x04); // sign bit for 16-bit short
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x40); // minus
				I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
				data = -data;
			}
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, 0x80);			// Select leftest bit
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[data%10]);		// Display number		
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
			data = data/10;
			if(data == 0) return ;
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, 0x40);
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[data%10]);//ten
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
			data = data/10;
			if(data == 0) return ;
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x20);
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[data%10]);//hundred
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
			data = data/10;
			if(data == 0) return ;
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x10);
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[data%10]);//thousand
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
			data = data/10;
			if(data == 0) return ;
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0x08);
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,seg7[data%10]);//ten thousand
			I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);
			
}
void Read_Accel(int I2C_Num)
{
	buf[0]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_XOUT_L);
	buf[1]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_XOUT_H);
	A_X = (buf[1]<<8)|buf[0];
	A_X /= 164;
	
	buf[2]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_YOUT_L);
	buf[3]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_YOUT_H);
	A_Y = (buf[3]<<8)|buf[2];
	A_Y /= 164;
	
	buf[4]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_ZOUT_L);
	buf[5]=I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,ACCEL_ZOUT_H);
	A_Z = (buf[5]<<8)|buf[4];
	A_Z /= 164;
	
	buf[7] = I2C_ReadByte(I2C_Num,ACCEL_ADDRESS,WHO_AM_I);
}

void Read_Gyro(int I2C_Num)
{
	buf[0]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_XOUT_L);
	buf[1]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_XOUT_H);
	G_X = (buf[1]<<8)|buf[0];
	G_X /= 16.4;
	
	buf[2]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_YOUT_L);
	buf[3]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_YOUT_H);
	G_Y = (buf[3]<<8)|buf[2];
	G_Y /= 16.4;
	
	buf[4]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_ZOUT_L);
	buf[5]=I2C_ReadByte(I2C_Num,GYRO_ADDRESS,GYRO_ZOUT_H);
	G_Z = (buf[5]<<8)|buf[4];
	G_Z /= 16.4;
}

void Read_Mag(int I2C_Num)
{
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,0x37,0x02);//turn on bypass
	Delay(10000);
	
	I2C_WriteByte(I2C_Num,MAG_ADDRESS,0x0A,0x11);
	Delay(100000);
	
	buf[8]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,0x00);
	buf[0]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_XOUT_L);
	buf[1]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_XOUT_H);
	M_X = (buf[1]<<8)|buf[0];
	
	buf[2]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_YOUT_L);
	buf[3]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_YOUT_H);
	M_Y = (buf[3]<<8)|buf[2];
	
	buf[4]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_ZOUT_L);
	buf[5]=I2C_ReadByte(I2C_Num,MAG_ADDRESS,MAG_ZOUT_H);
	M_Z = (buf[5]<<8)|buf[4];
}

void MPU9250_Read(int I2C_Num)
{
	Read_Accel(I2C_Num);
	Read_Gyro(I2C_Num);
	//Read_Mag(I2C_Num);
	agm[0]=A_X;agm[1]=A_Y;agm[2]=A_Z;
	agm[3]=G_X;agm[4]=G_Y;agm[5]=G_Z;
	//agm[6]=M_X;agm[7]=M_Y;agm[8]=M_Z;
	dataConv(agm,6,I2C_Num);
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

void S800_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);				//Enable PortF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);				//Enable PortJ	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));			//Wait for the GPIO moduleJ ready	
	
  	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1);		//Set the PJ0,PJ1 as input pin
	GPIOPadConfigSet(GPIO_PORTJ_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void MPU9250_Config(int I2C_Num)
{
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,PWR_MGMT_1,0x00);
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,SMPLRT_DIV,0x07);
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,CONFIG,0x06);
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,GYRO_CONFIG,0x18);
	I2C_WriteByte(I2C_Num,GYRO_ADDRESS,ACCEL_CONFIG,0x01);
}

//General I2C initialization
//input:16-bit mask, from right to left, I2C0~I2C9,I2C2(2),I2C7(2),I2C8(2)
void S800_I2C_Init(uint16_t Mask)
{
	
	//check I2C0
	if(0x0001&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIODirModeSet(GPIO_PORTB_BASE,GPIO_PIN_2|GPIO_PIN_3,GPIO_DIR_MODE_HW);
		GPIOPinConfigure(GPIO_PB2_I2C0SCL);
		GPIOPinConfigure(GPIO_PB3_I2C0SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
		GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
		GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C0_BASE,ui32SysClock, true);
	}
	
	//check I2C1
	if(0x0002&Mask)
	{
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
		GPIODirModeSet(GPIO_PORTG_BASE,GPIO_PIN_0|GPIO_PIN_1,GPIO_DIR_MODE_HW);//std func:PWM
		GPIOPinConfigure(GPIO_PG0_I2C1SCL);
		GPIOPinConfigure(GPIO_PG1_I2C1SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTG_BASE, GPIO_PIN_0);
		GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_1);
		GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C1_BASE,ui32SysClock, true);
	}
	
	//check I2C2
	if(0x0004&Mask)
	{
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
		//GPIODirModeSet(GPIO_PORTN_BASE,GPIO_PIN_5|GPIO_PIN_4,GPIO_DIR_MODE_HW);
		GPIOPinConfigure(GPIO_PN5_I2C2SCL);
		GPIOPinConfigure(GPIO_PN4_I2C2SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);
		GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);
		GPIOPadConfigSet(GPIO_PORTN_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTN_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C2_BASE,ui32SysClock, true);
	}
	
	//check I2C3
	if(0x0008&Mask)
	{
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
		GPIODirModeSet(GPIO_PORTK_BASE,GPIO_PIN_4|GPIO_PIN_5,GPIO_DIR_MODE_HW);//std func:PWM
		GPIOPinConfigure(GPIO_PK4_I2C3SCL);
		GPIOPinConfigure(GPIO_PK5_I2C3SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTK_BASE, GPIO_PIN_4);
		GPIOPinTypeI2C(GPIO_PORTK_BASE, GPIO_PIN_5);
		GPIOPadConfigSet(GPIO_PORTK_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTK_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C3_BASE,ui32SysClock, true);
		//GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x1);
	}
	
	//check I2C4
	if(0x0010&Mask)
	{
		//GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_PIN_0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C4);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
		GPIODirModeSet(GPIO_PORTK_BASE,GPIO_PIN_6|GPIO_PIN_7,GPIO_DIR_MODE_HW);//std func:GPIO
		GPIOPinConfigure(GPIO_PK6_I2C4SCL);
		GPIOPinConfigure(GPIO_PK7_I2C4SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTK_BASE, GPIO_PIN_6);
		GPIOPinTypeI2C(GPIO_PORTK_BASE, GPIO_PIN_7);
		GPIOPadConfigSet(GPIO_PORTK_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTK_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C4_BASE,ui32SysClock, true);
	}
	
	//check I2C5
	if(0x0020&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C5);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIODirModeSet(GPIO_PORTB_BASE,GPIO_PIN_4|GPIO_PIN_5,GPIO_DIR_MODE_HW);//std func:Ananlog
		GPIOPinConfigure(GPIO_PB4_I2C5SCL);
		GPIOPinConfigure(GPIO_PB5_I2C5SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_4);
		GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_5);
		GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C5_BASE,ui32SysClock, true);
	}
	
	//check I2C6
	if(0x0040&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C6);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7,GPIO_DIR_MODE_HW);//std func:Analog;GPIO
		GPIOPinConfigure(GPIO_PA6_I2C6SCL);
		GPIOPinConfigure(GPIO_PA7_I2C6SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
		GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C6_BASE,ui32SysClock, true);
	}
	
	//check I2C7
	if(0x0080&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C7);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5,GPIO_DIR_MODE_HW);//std func:A out
		GPIOPinConfigure(GPIO_PA4_I2C7SCL);
		GPIOPinConfigure(GPIO_PA5_I2C7SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_4);
		GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_5);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C7_BASE,ui32SysClock, true);
	}
	
	//check I2C8
	if(0x0100&Mask)
	{
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		//GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3,GPIO_DIR_MODE_HW);
		GPIOPinConfigure(GPIO_PA2_I2C8SCL);
		GPIOPinConfigure(GPIO_PA3_I2C8SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);
		GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C8_BASE,ui32SysClock, true);
	}
	
	//check I2C9
	if(0x0200&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C9);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		GPIODirModeSet(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1,GPIO_DIR_MODE_HW);//std func:GPIO;Analog
		GPIOPinConfigure(GPIO_PA0_I2C9SCL);
		GPIOPinConfigure(GPIO_PA1_I2C9SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_0);
		GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_1);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C9_BASE,ui32SysClock, true);
	}
	
	//check I2C2(2)
	if(0x0400&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
		GPIODirModeSet(GPIO_PORTL_BASE,GPIO_PIN_1|GPIO_PIN_0,GPIO_DIR_MODE_HW);//std func:GPIO
		GPIOPinConfigure(GPIO_PL1_I2C2SCL);
		GPIOPinConfigure(GPIO_PL0_I2C2SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
		GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
		GPIOPadConfigSet(GPIO_PORTL_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTL_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C2_BASE,ui32SysClock, true);
	}
	
	//check I2C7(2)
	if(0x0800&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C7);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		GPIODirModeSet(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1,GPIO_DIR_MODE_HW);//std func:SPI MISO;SPI MOSI
		GPIOPinConfigure(GPIO_PD0_I2C7SCL);
		GPIOPinConfigure(GPIO_PD1_I2C7SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
		GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
		GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C7_BASE,ui32SysClock, true);
	}
	//check I2C8(2)
	if(0x1000&Mask)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		GPIODirModeSet(GPIO_PORTD_BASE,GPIO_PIN_2|GPIO_PIN_3,GPIO_DIR_MODE_HW);//std func:Analog;SPI CLK
		GPIOPinConfigure(GPIO_PD2_I2C8SCL);
		GPIOPinConfigure(GPIO_PD3_I2C8SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_2);
		GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_3);
		GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		I2CMasterInitExpClk(I2C8_BASE,ui32SysClock, true);
	}
	
	
}

uint8_t I2C_WriteByte(int I2C_Num, uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData)
{
	uint32_t I2C_base;uint8_t rop;
	//switch I2C
	switch(I2C_Num)
	{
		case 0: I2C_base=I2C0_BASE;break;
		case 1: I2C_base=I2C1_BASE;break;
		case 2: I2C_base=I2C2_BASE;break;
		case 3: I2C_base=I2C3_BASE;break;
		case 4: I2C_base=I2C4_BASE;break;
		case 5: I2C_base=I2C5_BASE;break;
		case 6: I2C_base=I2C6_BASE;break;
		case 7: I2C_base=I2C7_BASE;break;
		case 8: I2C_base=I2C8_BASE;break;
		case 9: I2C_base=I2C9_BASE;break;
	}
	while(I2CMasterBusy(I2C_base));					// Wait if I2C master is busy
		
	I2CMasterSlaveAddrSet(I2C_base, DevAddr, false);		// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)		
	I2CMasterDataPut(I2C_base, RegAddr);				// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_START);	// Start the transmission and send multiple bytes
	while(I2CMasterBusy(I2C_base));					// Wait if I2C master is busy
	Delay(10);	
	rop = (uint8_t)I2CMasterErr(I2C_base);				// if there is error in the transmission, terminate the transmission
	if(rop) {
		I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_STOP);
		return rop;
	}
	Delay(10);
	I2CMasterDataPut(I2C_base, WriteData);				// Set the data (the data to the register specified by the above command) to be sent
	I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_FINISH);	// send the second byte and terminate the transmission when finished
	while(I2CMasterBusy(I2C_base));					// Wait if I2C master is busy
	Delay(10);
	rop = (uint8_t)I2CMasterErr(I2C_base);				// if there is error in the transmission, return the error status
	if(rop) {
		I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_STOP);
		return rop;
	}
	return 0;
}

uint8_t I2C_ReadByte(int I2C_Num, uint8_t DevAddr, uint8_t RegAddr)
{
	uint32_t I2C_base;uint8_t value;uint8_t rop;
	//switch I2C
	switch(I2C_Num)
	{
		case 0: I2C_base=I2C0_BASE;break;
		case 1: I2C_base=I2C1_BASE;break;
		case 2: I2C_base=I2C2_BASE;break;
		case 3: I2C_base=I2C3_BASE;break;
		case 4: I2C_base=I2C4_BASE;break;
		case 5: I2C_base=I2C5_BASE;break;
		case 6: I2C_base=I2C6_BASE;break;
		case 7: I2C_base=I2C7_BASE;break;
		case 8: I2C_base=I2C8_BASE;break;
		case 9: I2C_base=I2C9_BASE;break;
	}
	while(I2CMasterBusy(I2C_base));				// Wait if I2C master is busy

	I2CMasterSlaveAddrSet(I2C_base, DevAddr, false);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterDataPut(I2C_base, RegAddr);			// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C_base,I2C_MASTER_CMD_SINGLE_SEND);	// Start the transmission and send one byte
	while(I2CMasterBusBusy(I2C_base));			// Wait if I2C master is busy
	Delay(200);
	rop = (uint8_t)I2CMasterErr(I2C_base);
	if(rop) {																					// if there is error in the transmission, return the error status
		return rop;
	}
	Delay(10);

	I2CMasterSlaveAddrSet(I2C_base, DevAddr, true);			// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterControl(I2C_base,I2C_MASTER_CMD_SINGLE_RECEIVE);	// Start the transmission and read one byte
	while(I2CMasterBusBusy(I2C_base));				// Wait if I2C master is busy
	//here need to delay!
	//Delay(1000);
	Delay(300);
	value=I2CMasterDataGet(I2C_base);				// Get the data from the data register of the master
	//Delay(1000);
	return value;
}

//General I2C Read and Write Byte
//I2C_Num: I2C number, DevAddr:Slave address, RegAddr:Register Address, rw:0 for read;1 for write, WriteData:Data to be written
uint8_t I2C_ReadWriteByte(int I2C_Num, uint8_t DevAddr, uint8_t RegAddr,bool rw,uint8_t WriteData)
{
	uint32_t I2C_base;uint8_t value,rop;
	//switch I2C
	switch(I2C_Num)
	{
		case 0: I2C_base=I2C0_BASE;break;
		case 1: I2C_base=I2C1_BASE;break;
		case 2: I2C_base=I2C2_BASE;break;
		case 3: I2C_base=I2C3_BASE;break;
		case 4: I2C_base=I2C4_BASE;break;
		case 5: I2C_base=I2C5_BASE;break;
		case 6: I2C_base=I2C6_BASE;break;
		case 7: I2C_base=I2C7_BASE;break;
		case 8: I2C_base=I2C8_BASE;break;
		case 9: I2C_base=I2C9_BASE;break;
	}
	
	while(I2CMasterBusy(I2C_base));// Wait if I2C master is busy
	
	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)
	
	
	// Set the data (the command of the expender chip) to be sent
	
	
	//r:Start the transmission and send one byte //w:Start the transmission and send multiple bytes
	if(!rw)
	{
		//if ReadByte
		I2CMasterSlaveAddrSet(I2C_base, DevAddr, false);
		I2CMasterDataPut(I2C_base, RegAddr);
		I2CMasterControl(I2C_base,I2C_MASTER_CMD_SINGLE_SEND);
	}
	else
	{
		//if WriteByte
		I2CMasterSlaveAddrSet(I2C_base, DevAddr, false);
		I2CMasterDataPut(I2C_base, RegAddr);
		I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_START);
		//I2CMasterControl(I2C_base, I2C_MASTER_CMD_SINGLE_SEND);
		
	}
	while(I2CMasterBusBusy(I2C_base));// Wait if I2C master is busy
	//Delay(200);
	rop = (uint8_t)I2CMasterErr(I2C_base);
	if(rop) {																					// if there is error in the transmission, return the error status
		if(rw) I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_STOP);
		return rop;
	}
	Delay(1);
	
	if(rw)
	{
		//if WriteByte
		
		// Set the data (the data to the register specified by the above command) to be sent
		I2CMasterDataPut(I2C_base, WriteData);
		// send the second byte and terminate the transmission when finished
		I2CMasterControl(I2C_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(I2C_base));			// Wait if I2C master is busy

		rop = (uint8_t)I2CMasterErr(I2C_base);		// if there is error in the transmission, return the error status
		return rop;
	}
	else
	{
		//if ReadByte
		I2CMasterSlaveAddrSet(I2C_base, DevAddr, true);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
		I2CMasterControl(I2C_base,I2C_MASTER_CMD_SINGLE_RECEIVE);// Start the transmission and read one byte
		while(I2CMasterBusBusy(I2C_base));	// Wait if I2C0 master is busy
		//here need to delay!
		Delay(2000);
		value=I2CMasterDataGet(I2C_base);	// Get the data from the data register of the master
		Delay(1);
		return value;
	}
	
}



void UARTStringPut(const char *cMessage)
{
	while(*cMessage!='\0')
		UARTCharPut(UART2_BASE,*(cMessage++));
		//UARTprintf("hello!\n");
}
void S800_UART_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
  	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);		//Enable PortA
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));	//Wait for the GPIO moduleA ready

	GPIOPinConfigure(GPIO_PD4_U2RX);// Set GPIO A0 and A1 as UART pins.
  	GPIOPinConfigure(GPIO_PD5_U2TX);

  	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	// Configure the UART for 115,200, 8-N-1 operation.
  	UARTConfigSetExpClk(UART2_BASE, ui32SysClock,1500000,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	UARTStringPut((uint8_t *)"\r\nHello, world!\r\n");
}

void TCA6424_Config(int I2C_Num)
{
	I2C_WriteByte(I2C_Num,TCA6424_I2CADDR,TCA6424_CONFIG_PORT0,0xff);//port0:input
	I2C_WriteByte(I2C_Num,TCA6424_I2CADDR,TCA6424_CONFIG_PORT1,0x00);//port1:output
	I2C_WriteByte(I2C_Num,TCA6424_I2CADDR,TCA6424_CONFIG_PORT2,0x00);//port2:output
	
	I2C_WriteByte(I2C_Num,TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0xff);//select all digits
	I2C_WriteByte(I2C_Num,TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x00);//turn off all LED tube
}
void PCA9557_Config(int I2C_Num)
{
	I2C_WriteByte(I2C_Num,PCA9557_I2CADDR,PCA9557_CONFIG,0x00);//config the port of PCA9557 as output
	I2C_WriteByte(I2C_Num,PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);//turn off LED1~8
}

void S800_I2C0_Init(void)
{
  	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);	// Provide clock to enable I2C0
  	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// I2C0 uses pin PB2 and PB3, which by default are used as GPIO. We need to program to change the use of PB2 and PB3
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);		// Program on GPIOPCTL to select I2C0 SCL to be connected to PB2
  	GPIOPinConfigure(GPIO_PB3_I2C0SDA);		// Program on GPIOPCTL to select I2C0 SDA to be connected to PB3
  	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);	// Program on GPIOAFSEL to swith PB2 to be used by hardware and program the pad control of this pin for this
  	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);		// Program on GPIOAFSEL to swith PB3 to be used by hardware and program the pad control of this pin for this

	I2CMasterInitExpClk(I2C0_BASE,ui32SysClock, true);						//enable I2C0 master, the input clock for I2C0 is the system clock and set I2C0 to run in fast mode (400kbps) or normal mode

	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT0,0x0ff);		//config port0 of TCA6424 as input
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT1,0x0);			//config port1 of TCA6424 as output
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT2,0x0);			//config port2 of TCA6424 as output

	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2,0xff);		//Select all digits
	I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1,0x0);			//Turn off all LED tubes
	


	I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_CONFIG,0x0);						//config the port of PCA9557 as output
	I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);					//turn off the LED1-8
	
	//---------
	I2C0_WriteByte(GYRO_ADDRESS,PWR_MGMT_1,0x00);
	I2C0_WriteByte(GYRO_ADDRESS,SMPLRT_DIV,0x07);
	I2C0_WriteByte(GYRO_ADDRESS,CONFIG,0x06);
	I2C0_WriteByte(GYRO_ADDRESS,GYRO_CONFIG,0x18);
	I2C0_WriteByte(GYRO_ADDRESS,ACCEL_CONFIG,0x01);
}

uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData)
{
	uint8_t rop;
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy
		
	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); // Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)		
	I2CMasterDataPut(I2C0_BASE, RegAddr);							// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);	// Start the transmission and send multiple bytes
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy
		
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);						// if there is error in the transmission, terminate the transmission
	if(rop) {
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);
		return rop;
	}

	I2CMasterDataPut(I2C0_BASE, WriteData);						// Set the data (the data to the register specified by the above command) to be sent
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);// send the second byte and terminate the transmission when finished
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy

	rop = (uint8_t)I2CMasterErr(I2C0_BASE);						// if there is error in the transmission, return the error status
	return rop;
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
	uint8_t value,rop;
	while(I2CMasterBusy(I2C0_BASE));								// Wait if I2C0 master is busy

	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterDataPut(I2C0_BASE, RegAddr);							// Set the data (the command of the expender chip) to be sent
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND);	// Start the transmission and send one byte
	while(I2CMasterBusBusy(I2C0_BASE));								// Wait if I2C0 master is busy
	rop = (uint8_t)I2CMasterErr(I2C0_BASE);
	if(rop) {																					// if there is error in the transmission, return the error status
		return rop;
	}
	Delay(1);

	I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true);	// Set the slave address in the master and indicate the direction of the following transmission (false means send, true means read)	
	I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE);// Start the transmission and read one byte
	while(I2CMasterBusBusy(I2C0_BASE));								// Wait if I2C0 master is busy
	//here need to delay!
	Delay(200);
	value=I2CMasterDataGet(I2C0_BASE);								// Get the data from the data register of the master
	Delay(1);
	return value;
}
