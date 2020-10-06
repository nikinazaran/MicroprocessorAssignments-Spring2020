#include <stm32f4xx.h>
#include <stdbool.h>

unsigned char current_number[4] = { 0x07, 0x7D, 0x3F, 0x4F};
unsigned char digitPattern[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
bool isPressed = false;
int currentlyPressed = 0;

void INIT_PORTA();
void INIT_PORTB();

void shiftNum(int n){
	int i;
	for(i=2; i>=0; i--){
		current_number[i+1] = current_number[i];
	}
	current_number[0] = digitPattern[n];
}

void delay(int n){
	int i, j;
	for(i=0; i<n; i++){
		int input = GPIOB->IDR;
		for (j=0; j<20000; j++){
			currentlyPressed = 0;
			GPIOB->ODR = 0XEFFF;
			input = GPIOB->IDR;
			
			switch(input){
					case 0xEEFF: 
					if(!isPressed){
					shiftNum(1);
					isPressed=true;}
					currentlyPressed = 1;
					break;
				case 0xEDFF: 
					if(!isPressed){
					shiftNum(2);
					isPressed=true;}
					currentlyPressed = 1;
					break;
				case 0xEBFF: 
					if(!isPressed){
					shiftNum(3);
					isPressed=true;}
					currentlyPressed = 1;
					break;
			}
			
			
			GPIOB->ODR = 0XDFFF;
			input = GPIOB->IDR;
			
				switch(input){
					case 0xDEFF: 
						if(!isPressed){
						shiftNum(4);
						isPressed=true;}
						currentlyPressed = 2;
						break;
					case 0xDDFF: 
						if(!isPressed){
						shiftNum(5);
						isPressed=true;}
						currentlyPressed = 2;
						break;
					case 0xDBFF: 
						if(!isPressed){
						shiftNum(6);
						isPressed=true;}
						currentlyPressed = 2;
						break;
				}
			
			GPIOB->ODR = 0XBFFF;
			input = GPIOB->IDR;
				switch(input){
					case 0xBEFF: 
						if(!isPressed){
						shiftNum(7);
						isPressed=true;}
						currentlyPressed = 3;
						break;
					case 0xBDFF: 
						if(!isPressed){
						shiftNum(8);
						isPressed=true;}
						currentlyPressed = 3;
						break;
					case 0xBBFF: 
						if(!isPressed){
						shiftNum(9);
						isPressed=true;}
						currentlyPressed = 3;
						break;
				}

			GPIOB->ODR = 0X7FFF;
			input = GPIOB->IDR;
				switch(input){
					case 0x7DFF:
							if(!isPressed){
							shiftNum(0);
							isPressed=true;}
							currentlyPressed = 4;
							break;
				}
			if(currentlyPressed == 0){
				isPressed = false;
			}
			
		}
	}
}

void checkForInput(){
	int input = GPIOB->IDR;
	GPIOB->ODR = 0XEFFF;
		
		switch(input){
			case 0xEEFF: 
				shiftNum(1);
				break;
			case 0xEDFF: 
				shiftNum(2);
				break;
			case 0xEBFF: 
				shiftNum(3);
				break;
		}
		GPIOB->ODR = 0XDFFF;
		input = GPIOB->IDR;
		switch(input){
			case 0xDEFF: 
				shiftNum(4);break;
			case 0xDDFF: 
				shiftNum(5);break;
			case 0xDBFF: 
				shiftNum(6);break;
		}
		GPIOB->ODR = 0XBFFF;
		input = GPIOB->IDR;
		switch(input){
			case 0xBEFF: 
				shiftNum(7);break;
			case 0xBDFF: 
				shiftNum(8);break;
			case 0xBBFF: 
				shiftNum(9);break;
		}
		GPIOB->ODR = 0X7FFF;
		input = GPIOB->IDR;
		switch(input){
			case 0x7DFF: 
				shiftNum(0);break;
		}
}

int main(){
	INIT_PORTA();
	INIT_PORTB();
	while(1){
		
		GPIOA->ODR = current_number[0];
		GPIOA->ODR |= 0X100;
		delay(1);
		//checkForInput();
		GPIOA->ODR = 0X000;
		delay(1);
		//checkForInput();
		GPIOA->ODR = current_number[1];
		GPIOA->ODR |= 0X200;
		delay(1);
		//checkForInput();
		GPIOA->ODR = 0X000;
		delay(1);
		//checkForInput();
		GPIOA->ODR = current_number[2];
		GPIOA->ODR |= 0X400;
		delay(1);
		//checkForInput();
		GPIOA->ODR = 0X000;
		delay(1);
		//checkForInput();
		GPIOA->ODR = current_number[3];
		GPIOA->ODR |= 0X800;
		delay(1);
		//checkForInput();
		GPIOA->ODR = 0X000;
		delay(1);
		//checkForInput();
		
		
 }
	while(1){
		int input = GPIOB->IDR;
		GPIOB->ODR = 0XEFFF;
		
		switch(input){
			case 0xEEFF: 
				GPIOA->ODR = 0XF1;
				break;
			case 0xEDFF: 
				GPIOA->ODR = 0XF2;
				break;
			case 0xEBFF: 
				GPIOA->ODR = 0XF3;
				break;
		}
		GPIOB->ODR = 0XDFFF;
		input = GPIOB->IDR;
		switch(input){
			case 0xDEFF: 
				GPIOA->ODR = 0X4;break;
			case 0xDDFF: 
				GPIOA->ODR = 0X5;break;
			case 0xDBFF: 
				GPIOA->ODR = 0X6;break;
		}
		GPIOB->ODR = 0XBFFF;
		input = GPIOB->IDR;
		switch(input){
			case 0xBEFF: 
				GPIOA->ODR = 0X7;break;
			case 0xBDFF: 
				GPIOA->ODR = 0X8;break;
			case 0xBBFF: 
				GPIOA->ODR = 0X9;break;
		}
		GPIOB->ODR = 0X7FFF;
		input = GPIOB->IDR;
		switch(input){
			case 0x7DFF: 
				GPIOA->ODR = 0X0;break;
		}
	}
}

void INIT_PORTA(){
	//clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//MODER PORT A
	GPIOA->MODER = 0x55555555;
}

void INIT_PORTB(){
	//clock for GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//MODER PORT B
	GPIOB->MODER = 0x55000000;
}

