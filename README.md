# About
This is a repository for assignments given to students of the Microprocessor And Assembly course spring 2020 at Shahid Beheshti University.
Project 1 is designed for the 8086 processor.
I used Proteus and Keil ($\mu$Vision) for projects 2-5.

# 1. Pong
A single-player Pong game made with 8086 assembly. The color of the ball changes randomly, and
random obstacles appear on the screen. Use Dos Box to run it.

# 2. Blinking LED
A simple timer made with 8086, 8259, 8253 and 8255 chips. Use buttons to change the time.

# 3. Student ID on 7SEG
The last 5 digits of my student ID is saved in an array and a push-button is assigned to each element. When button ***'x'*** is pushed the result of ***ID[x]^3 mod 100*** is displayed on the 7-segments.

# 4. Keypad, 7SEG, LCD

 1. A: I use a keypad to input digits and they're displayed on 4 7-segments.
 2.  B: I use an alphanumeric LCD instead of the 7-segments to display the digits.

# 5. Final Project - Wave Generator
Made a signal generator using 2 STM32F401 microcontrollers connected by UART. The project contains ADC and DAC and it generates 6 signal types: *Sine, abs(sine), square, triangle, sawtooth and steps.*
