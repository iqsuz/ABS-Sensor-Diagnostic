#include "stm32f4xx.h"
#include "RHBE.h"

volatile static isControlStruct isControl;
uint8_t i,j;

int main(){
	isControl.isButton = FALSE;
	isControl.isMotorRotateEnough = FALSE;
	isControl.isComplete = FALSE;
	i = 0;
	j = 0;
	
	RHBE_InitEdgeInterrupt();
	RHBE_InitLED();
	RHBE_InitUSART();
	USART_SendString(USART2, "Program Started\r\n");
	
	while(1){
		while(!isControl.isButton);
		while(!isControl.isMotorRotateEnough);
		while(!isControl.isComplete);
		
	}
}

void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == 1){
		//isControl.isButton = TRUE;
		USART_SendString(USART2, "Butona basildi.\r\n");
		
		RHBE_InitPWM();
		RHBE_InitFirstTimer();
		USART_SendString(USART2, "PWM Init.\r\nTimer Init\r\n");
		
		RHBE_SetPWMDutyCycle(50);
		USART_SendString(USART2, "Motor was started.\r\n");
		
		EXTI_ClearITPendingBit(EXTI_Line0);	
	}
}

void TIM2_IRQHandler(){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)){
		i++;
		
		if(i >= 5){
			TIM_Cmd(TIM2, DISABLE);
			isControl.isMotorRotateEnough = TRUE; // Deger program sonunda FALSE yap1lmal1!!!
			USART_SendString(USART2, "Motor has been rotated enough.\r\n");
			
			RHBE_InitABSSignalInterrupt();
			USART_SendString(USART2, "ABS Interrupt has been set.\r\n");
			
			USART_SendString(USART2, "Encoder Init.\r\n");
			RHBE_InitEncoder();
			TIM_SetCounter(TIM4, 0);
			
			i = 0;
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}

}

void EXTI1_IRQHandler(){
	uint32_t Encoder;
	
	if(EXTI_GetITStatus(EXTI_Line1) == 1){
		Encoder = TIM_GetCounter(TIM4);
		TIM_SetCounter(TIM4, 0);
		j++;

		if(j <= 200){
			if(Encoder > 72 && Encoder < 56){
				RHBE_SetPWMDutyCycle(1);
				
				isControl.isComplete = TRUE;
				isControl.isResult = FALSE;
				isControl.isButton = FALSE;
				isControl.isMotorRotateEnough = FALSE;
				
				USART_SendString(USART2, "NG");
				
				j = 0;
			}
			
			if(j == 200){
				RHBE_SetPWMDutyCycle(1);
				
				isControl.isResult = TRUE;
				isControl.isComplete = TRUE;
				isControl.isButton = FALSE;
				isControl.isMotorRotateEnough = FALSE;
				
				USART_SendString(USART2, "OK");
				
				j = 0;
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);	
	}
}
