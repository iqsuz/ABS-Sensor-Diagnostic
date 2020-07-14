#ifndef __RHBE_H
	#define __RHBE_H

	#define RHBE_PERIOD 16779
	#define RHBE_PRESCALER 0
	#define RHBE_PWM_CHANNEL CCR2
	#define RHBE_PWM_TIM TIM5
	
	
	typedef enum{
		FALSE = 0,
		TRUE = 1
	}bool;
	
	typedef struct{
		bool isButton;
		bool isMotorRotateEnough;
		bool isComplete;
		bool isResult;
	}isControlStruct;
	

	void RHBE_InitButtonInterrupt(void);
	void RHBE_InitLED(void);
	void RHBE_InitPWM(void);
	void RHBE_SetPWMDutyCycle(uint8_t cycle);
	void RHBE_InitEncoder(void);
	void RHBE_InitFirstTimer(void);
	void RHBE_SelectionEdge(EXTITrigger_TypeDef edge);
	void RHBE_InitABSSignalInterrupt(void);
	void RHBE_InitUSART(void);
	void USART_SendString(USART_TypeDef* USARTx, char* s);

#endif
