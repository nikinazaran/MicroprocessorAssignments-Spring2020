#include <stm32f4xx.h>
#include <stdbool.h>

unsigned char current_number[4] = { '3', '0', '6', '7'};
unsigned char digitPattern[] = {'0','1','2','3','4','5','6','7','8','9'};
bool isPressed = false;
int currentlyPressed = 0;

void initPortA();
void initPortB();
void initPortC();
void initLCD();
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_ready();

void delayMs(int n)
{
	int i;
	int j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < 60000; j++)
		{
		}
	}
}


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



int main()
{
	initPortA();
	initPortB();
	initPortC();
	initLCD();
	
	while (1)
	{
		LCD_command(1);
		LCD_command(0x80);
		LCD_data(current_number[0]);
		LCD_data(current_number[1]);
		LCD_data(current_number[2]);
		LCD_data(current_number[3]);
		checkForInput();
	}
}


void initPortA()
{
	// Clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Moder port A
	GPIOA->MODER = 0x55555555;
}

void initPortB()
{
	// Clock for GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// Moder port B: PB12 to PB15 output & PB0 to PB11 input
	GPIOB->MODER = 0x55000000;
}

void initPortC()
{
	// Clock for GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// Moder port C: PC0 to PC4 output
	GPIOC->MODER = 0x00000055;
}

void initLCD(void)
{
	LCD_command(0x30);
	LCD_command(0x38);
	LCD_command(0x06);
	LCD_command(0x01);
	LCD_command(0x0F);
}

void LCD_command(unsigned char command)
{
	//LCD_ready();
	GPIOC->ODR &= 0xFFFC; // Force RS = 0 & RW = 0
	GPIOA->ODR = command;
	GPIOC->ODR |= 0x0004; // Force E = 1
	delayMs(1);
	GPIOC->ODR &= 0x111B; // Force E = 0
}

void LCD_ready(void)
{
	char status;
	// Moder port A
	GPIOA->MODER = 0x00000000; // Port A is input
	GPIOC->ODR &= 0xFFFE; // Force RS = 0
	GPIOC->ODR |= 0x0002; // Force RW = 1
	do {
		GPIOC->ODR |= 0x0004; // Force E = 1
		delayMs(1);
		status = GPIOA->IDR;
		GPIOC->ODR &= 0x111B; // Force E = 0
		delayMs(1);
	} while (status & 0x80);
	GPIOC->ODR &= 0xFFFD; // Force RW = 0
	GPIOA->MODER = 0x55555555; // Port A is output
}

void LCD_data(unsigned char data)
{
	GPIOC->ODR = 0x0001; // Force RS = 1 and RW = 0
	GPIOA->ODR = data;
	GPIOC->ODR |= 0x0004; // Force E = 1
	delayMs(1);
	GPIOC->ODR &= 0x111B; // Force E = 0
	delayMs(2);
}
