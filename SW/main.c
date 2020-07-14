#include "stm32f4xx.h"
#include "RHBE.h"

volatile static isControlStruct isControl;
uint8_t counterMotorRotate, counterABSPulse;

int main(){
	//Variables is set initial values.
	isControl.isButton = FALSE;
	isControl.isMotorRotateEnough = FALSE;
	isControl.isComplete = FALSE;
	counterMotorRotate = 0;
	counterABSPulse = 0;
	
	//Button, LED and USART is set.
	RHBE_InitButtonInterrupt();
	RHBE_InitLED();
	RHBE_InitUSART();
	USART_SendString(USART2, "Program Started\r\n");
	
	while(1){
		while(!isControl.isButton);
		while(!isControl.isMotorRotateEnough);
		while(!isControl.isComplete);
	}
}

/*
This interrupt function controls start button. It will set up configuration after button is pressed.
*/
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == 1){ //Which interrupts flag was set?
		USART_SendString(USART2, "Motor has been started.\r\n");
		
		RHBE_InitPWM(); //Motor started spinning.
		RHBE_SetPWMDutyCycle(50);  //Duty Cycle was set %50.
		
		RHBE_InitFirstTimer(); //Timer has been set to calculate sufficent motor spinning.
		
		USART_SendString(USART2, "PWM Init.\r\nTimer Init\r\n"); //User has been informed.
		
		EXTI_ClearITPendingBit(EXTI_Line0); //Cleaning interrupt flag.	
	}
}


/*
This interrupt function determines whether motor is spinned enough or not?
*/
void TIM2_IRQHandler(){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)){
		counterMotorRotate++;
		
		if(counterMotorRotate >= 5){  //After some time motor will reach desired speed.
			TIM_Cmd(TIM2, DISABLE);
			isControl.isMotorRotateEnough = TRUE; // Value should be False end of program.
			USART_SendString(USART2, "Motor has been rotated enough.\r\n");
			
			RHBE_InitABSSignalInterrupt();
			USART_SendString(USART2, "ABS Interrupt has been set.\r\n");
			
			USART_SendString(USART2, "Encoder Init.\r\n");
			RHBE_InitEncoder();
			TIM_SetCounter(TIM4, 0);  //Encoder counter is set to 0.
			
			counterMotorRotate = 0;
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //Cleaning interrupt flag.
	}

}


/*
This interrupt function is triggered by ABS signal ouput.
When ABS complete one pulse, this function will be executed.
*/
void EXTI1_IRQHandler(){
	uint32_t Encoder;
	
	if(EXTI_GetITStatus(EXTI_Line1) == 1){ 
		Encoder = TIM_GetCounter(TIM4); //Get the encoder value between two pulse.
		TIM_SetCounter(TIM4, 0); // Prepare timer counter for next pulse measurement.
		counterABSPulse++;

		if(counterABSPulse <= 96){
			if(Encoder > 72 && Encoder < 56){  //if encoder counting exceed our limits throw an error and notify user as Not OK.
				RHBE_SetPWMDutyCycle(1);
				
				isControl.isComplete = TRUE;
				isControl.isResult = FALSE;
				isControl.isButton = FALSE;
				isControl.isMotorRotateEnough = FALSE;
				
				USART_SendString(USART2, "NG");
				
				counterABSPulse = 0;
			}
			else { // if there is not problem in one revolution, that means part is OK.
				RHBE_SetPWMDutyCycle(0); //Stop motor.
				
				isControl.isResult = TRUE;
				isControl.isComplete = TRUE;
				isControl.isButton = FALSE;
				isControl.isMotorRotateEnough = FALSE;
				
				USART_SendString(USART2, "OK");  //Notify user.
				
				counterABSPulse = 0;
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);	//cleaning interrupt flag.
	}
}
