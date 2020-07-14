/*
	USED ENVIROMENTAL HARDWARE
	TIM4 has been used for ENCODER counting
	B6 and B7 has been used for ENCODER channel A and B.
	TIM5 has been used to produce PWM output.
	A1 has been used for PWM output pin.
	A0 has been used to start program.
	TIM2 has been used to increase toque of DC motor.
	B1 has been used to produce interrupt from ABS signal.
	A2 and A3 has been used for serial communication in USART2.
	

*/


#include "stm32f4xx.h"
#include "RHBE.h"

/* 
===============================================================
	### How to use this function ###
===============================================================
This function is used to set external interrupt for start button.
When button that is connected to GPIOA pin 0 is pressed, microprocessor goes interrupt routine where program continues to initialize variables and other functions.

Interrupt hardware configurations
	GPIOA is being used.
	Pin is 0.
***Trigger edge is rising for default.
*/

void RHBE_InitButtonInterrupt(void){
	//Definations
	GPIO_InitTypeDef GPIO_Config;
	EXTI_InitTypeDef EXTI_Config;
	NVIC_InitTypeDef NVIC_Config;
	
	//Giving energy to GPIOA and EXTI buses.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//External interrupt pin configuation has been done.
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_Pin = GPIO_Pin_0;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_Config);
	
	//GPIOA's 0. pin has been assigned for external interrupt.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	//Configuration of external interrupt.
	EXTI_Config.EXTI_Line = EXTI_Line0;
	EXTI_Config.EXTI_LineCmd = ENABLE;
	EXTI_Config.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Config.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_Config);

	//requested interrupt.
	NVIC_Config.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Config);
	
}

void RHBE_InitLED(void)
{
	GPIO_InitTypeDef GPIO_Config;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_Pin = GPIO_Pin_12;
	GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_Config);
	
}

/* 
===============================================================
### How to use this function ###
===============================================================
This function can be used to produce PWM by using GPIOA's 1. pin.
Function is using TIM5 to produce PWM.

PWM hardware configurations
	GPIOA's 1.pin is being used for PWM output.
	TIM5 was used to produce PWM.
	To adjust PWM frequency define variable has been used in RHBE.h
			#define RHBE_PERIOD 399
			#define RHBE_PRESCALER 20
			#define RHBE_PWM_CHANNEL CCR2
			#define RHBE_PWM_TIM TIM5
	*** if you wanna change frequency of PWM you can play above definations.
*/

void RHBE_InitPWM(void)
{
	//definations
	GPIO_InitTypeDef GPIO_Config;
	TIM_TimeBaseInitTypeDef TIM_Config;
	TIM_OCInitTypeDef TIM_OC_Config;
	
	//Giving energy to GPIO and Timer units.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	//GPIO configuration for PWM output port and pin.
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_Pin = GPIO_Pin_1;
	GPIO_Config.GPIO_OType = GPIO_OType_PP;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_Config);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	
	//Timer configuration has been done.
	TIM_DeInit(RHBE_PWM_TIM);
	TIM_Config.TIM_ClockDivision = 0;
	TIM_Config.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Config.TIM_Period = RHBE_PERIOD;
	TIM_Config.TIM_Prescaler = RHBE_PRESCALER;
	TIM_TimeBaseInit(RHBE_PWM_TIM, &TIM_Config);
	
	//Timer was used for PWM. Below code can be used to produce PWM from timer.
	TIM_OC_Config.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_Config.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_Config.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_Config.TIM_Pulse = 0;
	TIM_OC2Init(RHBE_PWM_TIM, &TIM_OC_Config);
	
	TIM_OC2PreloadConfig(RHBE_PWM_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(RHBE_PWM_TIM,ENABLE);
	
	TIM_Cmd(RHBE_PWM_TIM,ENABLE);
	
}

/*
===============================================================
							### How to use this function ###
===============================================================
This function can be used to set PWM duty cycle denominated %percent.
Function can get value from 0 to 100 percent.
*/

void RHBE_SetPWMDutyCycle(uint8_t cycle)
{
	uint16_t pulse;
	pulse = (((RHBE_PERIOD +1) * cycle) / 100) - 1;		//calculation from percentage to pulse.
	RHBE_PWM_TIM -> RHBE_PWM_CHANNEL = pulse;	//value has been assigned to directly to register.
}
/*
===============================================================
### How to use this function ###
===============================================================
This function enables TIM4 for encoder counter.
Function will set Timer channels as encoder counter.
Encoder A will be connected to GPIOB's 6th pin.
Encoder B will be connected to GPIOB's 7th pin.
TIM4 will count in every edge of signal of A and B.

Interrupt hardware configurations
	GPIOB is being used.
	Pin is 6 and 7.
*/

void RHBE_InitEncoder(void)
{
	//Created GPIO struct.
	GPIO_InitTypeDef GPIO_Config;
	
	//enabled clock source for GPIOB, TIM4
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//GPIO settings to for Timer Channels
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_Config);
	
	//Pins are connected to AF.
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_TIM4);

	//Timer was set as an two channel counter.
	TIM_EncoderInterfaceConfig (TIM4, TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Rising, 
                              TIM_ICPolarity_Rising);
  	TIM_SetAutoreload (TIM4, 0xffff);
	
	//Timer is being ran with above settings.
	TIM_Cmd(TIM4,ENABLE);

}

/*
===============================================================
### How to use this function ###
===============================================================
This function is to update edge selection of interrupt of ABS sensor signal by using EXTI_TriggerTypeDef.
*/

void RHBE_SelectionEdge(EXTITrigger_TypeDef edge)
{
	EXTI_InitTypeDef EXTI_Config;
	
	EXTI_DeInit();
	EXTI_Config.EXTI_Line = EXTI_Line0;
	EXTI_Config.EXTI_LineCmd = ENABLE;
	EXTI_Config.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Config.EXTI_Trigger = edge;
	EXTI_Init(&EXTI_Config);

}
/*
===============================================================
							### How to use this function ###
===============================================================
This function is to adjust torque timer for motor. Cycle time will increas 10% in every timer interrupt if motor do not rotate.
TIM2 has been used to create interrupt.
Cycle time is ... (should be calculate with timer calculator.)

*/


void RHBE_InitFirstTimer(void)
{
	TIM_TimeBaseInitTypeDef TIM_Config;
	NVIC_InitTypeDef NVIC_Config;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_Config.TIM_Prescaler = 1312;
	TIM_Config.TIM_Period = 63999;
	TIM_Config.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Config.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_Config);
	
	TIM_SetCounter(TIM2, 0);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	

	NVIC_Config.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Config);

}
/* 
===============================================================
### How to use this function ###
===============================================================
This function is to set external interrupt for RR Hub bearing signal.
RR hub bearing will produce signal which is 5V or GND.
In every changing system will go into to this function to understan DC motor rotation value.

Interrupt hardware configurations
	GPIOB is being used.
	Pin is 1.
	
***Trigger edge is rising-falling for default.
*/

void RHBE_InitABSSignalInterrupt(void)
{
	//Definations
	GPIO_InitTypeDef GPIO_Config;
	EXTI_InitTypeDef EXTI_Config;
	NVIC_InitTypeDef NVIC_Config;
	
	//Enabled clock signal for GPIOB and SYSCFG
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//External interrupt pin configuation has been done.
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_Pin = GPIO_Pin_1;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Config.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_Config);
	
	//GPIOB's 1. pin has been assigned for external interrupt.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource1);

	//Configuration of external interrupt.
	EXTI_Config.EXTI_Line = EXTI_Line1;
	EXTI_Config.EXTI_LineCmd = ENABLE;
	EXTI_Config.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Config.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_Config);

	//requested interrupt.
	NVIC_Config.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Config);
	
}


/* 
===============================================================
### How to use this function ###
===============================================================
This function is to initilaze USART 2 for serial communication.

	GPIO A has been used.
	Pin 2 and pin 3 has been used for communication.
*/


void RHBE_InitUSART(void)
{
	//Declared Init structs.
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//Enabled clock sources for USART2 and GPIOA.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//USART will be initialized based on below settings.
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	//Usart is being ran with above settings.
	USART_Init(USART2,&USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
	//USART pin has been arranged as AF.
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
}

void USART_SendString(USART_TypeDef* USARTx, char* s)
{
	while(*s)
	{
		while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));
		USART_SendData(USARTx, *s);
		s++;
	}

}
