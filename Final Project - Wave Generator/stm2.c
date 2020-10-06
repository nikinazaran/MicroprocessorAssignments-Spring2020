#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

void initUSART();
unsigned char readUSART();
void LED_blink(int value);
void delayMs(int);
void initPortA();
void initPortB();
void initPortC();
void initLCD();
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_ready();
unsigned char figs[];
int i, j;
int chosenWave, time, freq;
int t, count;
int sins[400] = {3250, 3301, 3352, 3403, 3454, 3504, 3555, 3606, 3657, 3707, 3758, 3808, 3858, 3909, 3958, 4008, 4058, 4107, 4156, 4205,
                 4254, 4302, 4350, 4398, 4446, 4493, 4540, 4587, 4633, 4679, 4725, 4770, 4815, 4860, 4904, 4948, 4991, 5034, 5076, 5118,
                 5160, 5201, 5241, 5282, 5321, 5360, 5399, 5437, 5474, 5511, 5548, 5583, 5619, 5653, 5687, 5721, 5754, 5786, 5818, 5848,
                 5879, 5908, 5938, 5966, 5994, 6021, 6047, 6073, 6097, 6122, 6145, 6168, 6190, 6212, 6232, 6252, 6271, 6290, 6307, 6324,
                 6340, 6356, 6370, 6384, 6397, 6410, 6421, 6432, 6442, 6451, 6459, 6467, 6474, 6480, 6485, 6489, 6493, 6496, 6498, 6499,
                 6500, 6499, 6498, 6496, 6493, 6489, 6485, 6480, 6474, 6467, 6459, 6451, 6442, 6432, 6421, 6410, 6397, 6384, 6370, 6356,
                 6340, 6324, 6307, 6290, 6271, 6252, 6232, 6212, 6190, 6168, 6145, 6122, 6097, 6073, 6047, 6021, 5994, 5966, 5938, 5908,
                 5879, 5848, 5818, 5786, 5754, 5721, 5687, 5653, 5619, 5583, 5548, 5511, 5474, 5437, 5399, 5360, 5321, 5282, 5241, 5201,
                 5160, 5118, 5076, 5034, 4991, 4948, 4904, 4860, 4815, 4770, 4725, 4679, 4633, 4587, 4540, 4493, 4446, 4398, 4350, 4302,
                 4254, 4205, 4156, 4107, 4058, 4008, 3958, 3909, 3858, 3808, 3758, 3707, 3657, 3606, 3555, 3504, 3454, 3403, 3352, 3301,
                 3250, 3199, 3148, 3097, 3046, 2996, 2945, 2894, 2843, 2793, 2742, 2692, 2642, 2591, 2542, 2492, 2442, 2393, 2344, 2295,
                 2246, 2198, 2150, 2102, 2054, 2007, 1960, 1913, 1867, 1821, 1775, 1730, 1685, 1640, 1596, 1552, 1509, 1466, 1424, 1382,
                 1340, 1299, 1259, 1218, 1179, 1140, 1101, 1063, 1026, 989, 952, 917, 881, 847, 813, 779, 746, 714, 682, 652, 621, 592,
                 562, 534, 506, 479, 453, 427, 403, 378, 355, 332, 310, 288, 268, 248, 229, 210, 193, 176, 160, 144, 130, 116, 103, 90,
                 79, 68, 58, 49, 41, 33, 26, 20, 15, 11, 7, 4, 2, 1, 0, 1, 2, 4, 7, 11, 15, 20, 26, 33, 41, 49, 58, 68, 79, 90, 103, 116,
                 130, 144, 160, 176, 193, 210, 229, 248, 268, 288, 310, 332, 355, 378, 403, 427, 453, 479, 506, 534, 562, 592, 621, 652,
                 682, 714, 746, 779, 813, 847, 881, 917, 952, 989, 1026, 1063, 1101, 1140, 1179, 1218, 1259, 1299, 1340, 1382, 1424, 1466,
                 1509, 1552, 1596, 1640, 1685, 1730, 1775, 1821, 1867, 1913, 1960, 2007, 2054, 2102, 2150, 2198, 2246, 2295, 2344, 2393,
                 2442, 2492, 2542, 2591, 2642, 2692, 2742, 2793, 2843, 2894, 2945, 2996, 3046, 3097, 3148, 3199};

int triangleValues[100] = {0, 650, 1300, 1950, 2600, 3250, 3900, 4550, 5200, 5850, 6500, 7150, 7800, 8450, 9100, 9750, 10400, 11050,
                           11700, 12350, 13000, 13650, 14300, 14950, 15600, 16250, 16900, 17550, 18200, 18850, 19500, 20150, 20800,
                           21450, 22100, 22750, 23400, 24050, 24700, 25350, 26000, 26650, 27300, 27950, 28600, 29250, 29900, 30550,
                           31200, 31850, 32500, 33150, 33800, 34450, 35100, 35750, 36400, 37050, 37700, 38350, 39000, 39650, 40300,
                           40950, 41600, 42250, 42900, 43550, 44200, 44850, 45500, 46150, 46800, 47450, 48100, 48750, 49400, 50050,
                           50700, 51350, 52000, 52650, 53300, 53950, 54600, 55250, 55900, 56550, 57200, 57850, 58500, 59150, 59800,
                           60450, 61100, 61750, 62400, 63050, 63700, 64350};
int stepValues[20] = {0, 6553, 13106, 19659, 26212, 32765, 39318, 45871, 52424, 58977, 65530, 58977, 52424, 45871, 39318, 32765, 26212, 19659, 13106, 6553};

int main(void)
{
    unsigned char c;
    initPortA();
    initPortB();
    //initPortC();
    //initLCD();

    // LCD_command(1);
    // LCD_command(0x80);

    initUSART(); /* initialize USART1 */

    while (1)
    {
        c = readUSART();
        if (c == 0)
            break;
    }
    for (i = 0; i < 4; i++)
    {
        figs[i] = readUSART();
    }
    chosenWave = figs[0] & 0x07;
    time = ((figs[2] & 0x01) << 13) | (figs[1] << 5) | (figs[0] >> 3);
    freq = (figs[3]) | (figs[2] >> 1);
    t = 1000 / freq;
    count = time * freq;
    switch (chosenWave)
    {
    case 1: //sine
        for (i = 0; i < count / 4; i++)
        {
            for (j = 0; j < 400; j++)
            {
                GPIOA->ODR = sins[j];
                delayMs(t / 100);
            }
            // for (j = 99; j >= 50; j--)
            // {
            //     GPIOA->ODR = sins[j];
            //     delayMs(t / 100);
            // }
            // for (j = 49; j >= 0; j--)
            // {
            //     GPIOA->ODR = sins[j];
            //     delayMs(t / 100);
            // }
            // for (j = 0; j < 50; j++)
            // {
            //     GPIOA->ODR = sins[j];
            //     delayMs(t / 100);
            // }
        }
        break;
    case 2: //square
        for (i = 0; i < count / 2; i++)
        {
            GPIOA->ODR = 65500;
            delayMs(t);
            GPIOA->ODR = 0;
            delayMs(t);
        }

        break;
    case 3: //triangle
        for (i = 0; i < count / 2; i++)
        {
            for (j = 0; j < 100; j++)
            {
                GPIOA->ODR = triangleValues[j];
                delayMs(t / 100);
            }
            for (j = 99; j >= 0; j--)
            {
                GPIOA->ODR = triangleValues[j];
                delayMs(t / 100);
            }
        }
        // for (i = 0; i < count / 2; i++)
        // {
        //     for (j = 0; j < 65535; j += 650)
        //     {
        //         GPIOA->ODR = j;
        //         delayMs(t / 100);
        //     }
        //     for (j = 65535; j >= 0; j -= 650)
        //     {
        //         GPIOA->ODR = j;
        //         delayMs(t / 100);
        //     }
        // }
        break;
    case 4: //sawtooth
        for (i = 0; i < count; i++)
        {
            for (j = 0; j < 100; j++)
            {
                GPIOA->ODR = triangleValues[j];
                delayMs(t / 100);
            }
        }
        break;
    case 5: // step
        for (i = 0; i < count; i++)
        {
            for (j = 0; j < 20; j++)
            {
                GPIOA->ODR = stepValues[j];
                delayMs(t / 10);
            }
        }
        break;
    case 6: //abs(sin)
        for (i = 0; i < count / 2; i++)
        {
            for (j = 0; j < 200; j++)
            {
                GPIOA->ODR = (sins[j] - 3250) * 2;
                delayMs(t / 100);
            }
        }
        break;
    default:
        break;
    }
}

void LED_blink(int value)
{
    value %= 16; /* cap the max count at 15 */

    for (; value > 0; value--)
    {
        GPIOA->BSRR = 0x00000020; /* turn on LED */
        delayMs(10);
        GPIOA->BSRR = 0x00200000; /* turn off LED */
        delayMs(10);
    }
    //delayMs(50);
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
    GPIOB->MODER = 0x00008000;
}

void initPortC()
{
    // Clock for GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    // Moder port C: PC0 to PC4 output
    GPIOC->MODER = 0x55555555;
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
    GPIOC->ODR |= command << 3;
    GPIOC->ODR |= 0x0004; // Force E = 1
    delayMs(1);
    GPIOC->ODR &= 0xFFFB; // Force E = 0
}

void LCD_ready(void)
{
    char status;
    // Moder port A
    GPIOC->MODER = 0x00000015; // Port A is input
    GPIOC->ODR &= 0xFFFE;      // Force RS = 0
    GPIOC->ODR |= 0x0002;      // Force RW = 1
    do
    {
        GPIOC->ODR |= 0x0004; // Force E = 1
        delayMs(1);
        status = GPIOC->IDR;
        GPIOC->ODR &= 0x111B; // Force E = 0
        delayMs(1);
    } while (status & 0x80);
    GPIOC->ODR &= 0xFFFD;      // Force RW = 0
    GPIOC->MODER = 0x55555555; // Port A is output
}

void LCD_data(unsigned char data)
{
    GPIOC->ODR = 0x0001; // Force RS = 1 and RW = 0
    GPIOC->ODR |= data << 3;
    GPIOC->ODR |= 0x0004; // Force E = 1
    delayMs(1);
    GPIOC->ODR &= 0x111B; // Force E = 0
    delayMs(2);
}

void initUSART()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  /* Enable GPIOB clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; /* Enable USART1 clock */

    //GPIOA->AFR[0] &= ~0xF000;
    GPIOB->AFR[0] |= 0x70000000; /* alt7 for USART1 */
    // GPIOB->MODER &= ~0x00C0;
    GPIOB->MODER |= 0x8000; /* enable alternate function for PB7 */

    USART1->BRR = 0x0683;  /* 9600 baud @ 16 MHz */
    USART1->CR1 = 0x000C;  /* enable Rx, 8-bit data */
    USART1->CR2 = 0x0000;  /* 1 stop bit */
    USART1->CR3 = 0x0000;  /* no flow control */
    USART1->CR1 |= 0x2000; /* enable USART1 */
}

unsigned char readUSART()
{
    while (!(USART1->SR & 0x0020))
    {
    } // wait until char arrives
    return USART1->DR & 0xff;
}

void delayMs(int n)
{
    int i;
    int j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 40000; j++)
        {
        }
    }
}
