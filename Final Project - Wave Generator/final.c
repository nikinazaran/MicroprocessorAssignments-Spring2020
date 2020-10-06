#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

unsigned char sid[8] = {'9', '5', '2', '4', '3', '0', '6', '7'};
unsigned char waveNamesFL[25] = "1.Sin 2.Sqaure 3.Triangle";
unsigned char waveNamesSL[28] = "4.Sawtooth 5.Step 6.Abs(sin)";
int KPinput = 0, actualTime, actualFreq;
int timeResult = 0;
int freqResult = 0;
unsigned int chosenWave = 0;
bool sharpPressed = false;
char timeLenght[5], freqLenght[4];
unsigned char digitPattern[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
bool isPressed = false;
int currentlyPressed = 0;
int i = 0, j = 0;
unsigned char figs[4];
unsigned char show[4];

void initPortA();
void initPortB();
void initPortC();
void initLCD();
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_ready();
void initUSART();
void writeUSART(unsigned int ch);

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

void shiftNum(int n)
{
    int i;
    for (i = 2; i >= 0; i--)
    {
        sid[i + 1] = sid[i];
    }
    sid[0] = digitPattern[n];
}

void checkForWaveTypeInput()
{
    int input = GPIOB->IDR;
    currentlyPressed = 0;
    GPIOB->ODR = 0XEFFF;
    input = GPIOB->IDR;

    switch (input)
    {
    case 0xEEFF:
        if (!isPressed)
        {
            KPinput = 1;
            isPressed = true;
        }
        currentlyPressed = 1;
        break;
    case 0xEDFF:
        if (!isPressed)
        {
            KPinput = 2;
            isPressed = true;
        }
        currentlyPressed = 1;
        break;
    case 0xEBFF:
        if (!isPressed)
        {
            KPinput = 3;
            isPressed = true;
        }
        currentlyPressed = 1;
        break;
    }

    GPIOB->ODR = 0XDFFF;
    input = GPIOB->IDR;

    switch (input)
    {
    case 0xDEFF:
        if (!isPressed)
        {
            KPinput = 4;
            isPressed = true;
        }
        currentlyPressed = 2;
        break;
    case 0xDDFF:
        if (!isPressed)
        {
            KPinput = 5;
            isPressed = true;
        }
        currentlyPressed = 2;
        break;
    case 0xDBFF:
        if (!isPressed)
        {
            KPinput = 6;
            isPressed = true;
        }
        currentlyPressed = 2;
        break;
    }

    GPIOB->ODR = 0XBFFF;
    input = GPIOB->IDR;
    switch (input)
    {
    case 0xBEFF:
        if (!isPressed)
        {
            KPinput = 7;
            isPressed = true;
        }
        currentlyPressed = 3;
        break;
    case 0xBDFF:
        if (!isPressed)
        {
            KPinput = 8;
            isPressed = true;
        }
        currentlyPressed = 3;
        break;
    case 0xBBFF:
        if (!isPressed)
        {
            KPinput = 9;
            isPressed = true;
        }
        currentlyPressed = 3;
        break;
    }

    GPIOB->ODR = 0X7FFF;
    input = GPIOB->IDR;
    switch (input)
    {
    case 0x7DFF:
        if (!isPressed)
        {
            KPinput = 0;
            isPressed = true;
        }
        currentlyPressed = 4;
        break;
    case 0x7EFF:
        if (!isPressed)
        {
            KPinput = 10; // *
            isPressed = true;
        }
        currentlyPressed = 4;
        break;
    case 0x7BFF:
        if (!isPressed)
        {
            KPinput = 11; // #
            isPressed = true;
        }
        currentlyPressed = 4;
        break;
    }
    if (currentlyPressed == 0)
    {
        isPressed = false;
    }
}

int main()
{
    initPortA();
    initPortB();
    initPortC();
    initLCD();

    LCD_command(1);
    LCD_command(0x80);
    LCD_data(sid[0]);
    LCD_data(sid[1]);
    LCD_data(sid[2]);
    LCD_data(sid[3]);
    LCD_data(sid[4]);
    LCD_data(sid[5]);
    LCD_data(sid[6]);
    LCD_data(sid[7]);
    delayMs(50);
    LCD_command(1);
    LCD_command(2);
    LCD_command(0x80);

    for (i = 0; i < 25; i++)
    {
        LCD_data(waveNamesFL[i]);
    }
    LCD_command(0xC0);
    for (i = 0; i < 28; i++)
    {
        LCD_data(waveNamesSL[i]);
    }

    while (chosenWave == 0)
    {
        checkForWaveTypeInput();
        if (KPinput <= 6 && KPinput != 0)
            chosenWave = KPinput;
    }

    //LCD_data(digitPattern[chosenWave]);

    while (!sharpPressed)
    {
        ADC1->CR2 |= 0x40000000; /* start a conversion */
        while (!(ADC1->SR & 2))
        {
        }                      /* wait for conv complete */
        timeResult = ADC1->DR; /* read conversion result */
        actualTime = ((timeResult * 9500) / 4096) + 500;
        sprintf(timeLenght, "%d", actualTime);
        LCD_command(1);
        LCD_command(2);
        LCD_command(0x80);
        for (i = 0; i < sizeof(timeLenght); i++)
            LCD_data(timeLenght[i]);
        checkForWaveTypeInput();
        if (KPinput == 11)
            sharpPressed = true;
    }

    sharpPressed = false;
    KPinput = 12;
    while (!sharpPressed)
    {
        ADC1->CR2 |= 0x40000000; /* start a conversion */
        while (!(ADC1->SR & 2))
        {
        }                      /* wait for conv complete */
        freqResult = ADC1->DR; /* read conversion result */
        actualFreq = ((freqResult * 1000) / 4096);
        sprintf(freqLenght, "%d", actualFreq);
        LCD_command(1);
        LCD_command(2);
        LCD_command(0x80);
        for (i = 0; i < sizeof(freqLenght); i++)
            LCD_data(freqLenght[i]);
        checkForWaveTypeInput();
        if (KPinput == 11)
            sharpPressed = true;
    }

    figs[0] = actualTime << 3;
    figs[0] |= chosenWave;
    figs[1] = actualTime >> 5;
    figs[2] = actualTime >> 13;
    figs[2] |= actualFreq << 1;
    figs[3] = actualFreq >> 7;

    LCD_command(1);
    LCD_command(2);
    LCD_command(0x80);

    for (i = 3; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            LCD_data(((figs[i] >> j) & 1) + '0');
        }
        LCD_data(' ');
    }

    delayMs(10);
    initUSART();
    writeUSART(0);
    writeUSART(figs[0]);
    delayMs(5);
    writeUSART(figs[1]);
    delayMs(5);
    writeUSART(figs[2]);
    delayMs(5);
    writeUSART(figs[3]);
    delayMs(500);
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
    GPIOB->MODER = 0x55002000;
}

void initPortC()
{
    // Clock for GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    // Moder port C: PC0 to PC4 output
    GPIOC->MODER = 0x000000D5;

    RCC->APB2ENR |= 0x00000100; /* enable ADC1 clock */
    ADC1->CR2 = 0;              /* SW trigger */
    ADC1->SQR3 = 13;            /* conversion sequence starts at ch 13 */
    ADC1->SQR1 = 0;             /* conversion sequence length 1 */
    ADC1->CR2 |= 1;             /* enable ADC1 */
}

void initUSART()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  /* Enable GPIOB clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; /* Enable USART1 clock */

    /* Configure PA2 for USART2_TX */
    //GPIOA->AFR[0] &= ~0x0F00;
    GPIOB->AFR[0] |= 0x07000000; /* alt7 for USART1 */
    //GPIOB->MODER &= ~0x0030;
    GPIOB->MODER |= 0x2000; /* enable alternate function for PB6 */

    USART1->BRR = 0x0683;  /* 9600 baud @ 16 MHz */
    USART1->CR1 = 0x000C;  /* enable Tx, 8-bit data */
    USART1->CR2 = 0x0000;  /* 1 stop bit */
    USART1->CR3 = 0x0000;  /* no flow control */
    USART1->CR1 |= 0x2000; /* enable USART1 */
}

void writeUSART(unsigned int ch)
{
    delayMs(10);
    while (!(USART1->SR & 0x0080))
    {
    } // wait until Tx buffer empty
    USART1->DR = (ch & 0xFF);
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
    GPIOC->ODR &= 0xFFFE;      // Force RS = 0
    GPIOC->ODR |= 0x0002;      // Force RW = 1
    do
    {
        GPIOC->ODR |= 0x0004; // Force E = 1
        delayMs(1);
        status = GPIOA->IDR;
        GPIOC->ODR &= 0x111B; // Force E = 0
        delayMs(1);
    } while (status & 0x80);
    GPIOC->ODR &= 0xFFFD;      // Force RW = 0
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

void delay(int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        int input = GPIOB->IDR;
        for (j = 0; j < 20000; j++)
        {
            currentlyPressed = 0;
            GPIOB->ODR = 0XEFFF;
            input = GPIOB->IDR;

            switch (input)
            {
            case 0xEEFF:
                if (!isPressed)
                {
                    shiftNum(1);
                    isPressed = true;
                }
                currentlyPressed = 1;
                break;
            case 0xEDFF:
                if (!isPressed)
                {
                    shiftNum(2);
                    isPressed = true;
                }
                currentlyPressed = 1;
                break;
            case 0xEBFF:
                if (!isPressed)
                {
                    shiftNum(3);
                    isPressed = true;
                }
                currentlyPressed = 1;
                break;
            }

            GPIOB->ODR = 0XDFFF;
            input = GPIOB->IDR;

            switch (input)
            {
            case 0xDEFF:
                if (!isPressed)
                {
                    shiftNum(4);
                    isPressed = true;
                }
                currentlyPressed = 2;
                break;
            case 0xDDFF:
                if (!isPressed)
                {
                    shiftNum(5);
                    isPressed = true;
                }
                currentlyPressed = 2;
                break;
            case 0xDBFF:
                if (!isPressed)
                {
                    shiftNum(6);
                    isPressed = true;
                }
                currentlyPressed = 2;
                break;
            }

            GPIOB->ODR = 0XBFFF;
            input = GPIOB->IDR;
            switch (input)
            {
            case 0xBEFF:
                if (!isPressed)
                {
                    shiftNum(7);
                    isPressed = true;
                }
                currentlyPressed = 3;
                break;
            case 0xBDFF:
                if (!isPressed)
                {
                    shiftNum(8);
                    isPressed = true;
                }
                currentlyPressed = 3;
                break;
            case 0xBBFF:
                if (!isPressed)
                {
                    shiftNum(9);
                    isPressed = true;
                }
                currentlyPressed = 3;
                break;
            }

            GPIOB->ODR = 0X7FFF;
            input = GPIOB->IDR;
            switch (input)
            {
            case 0x7DFF:
                if (!isPressed)
                {
                    shiftNum(0);
                    isPressed = true;
                }
                currentlyPressed = 4;
                break;
            }
            if (currentlyPressed == 0)
            {
                isPressed = false;
            }
        }
    }
}
