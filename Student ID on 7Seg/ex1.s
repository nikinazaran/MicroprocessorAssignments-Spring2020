; ******* Register definitions *******

;As per STM32F401 datasheet and reference manual

		
RCC_AHB1ENR  EQU  0x40023830  ;Clock control for AHB1 peripherals (includes GPIO)

;GPIO-A control registers
GPIOA_MODER  EQU  0x40020000  ;set GPIO pin mode as Input/Output/Analog
GPIOA_IDR    EQU  0x40020010  ;GPIO pin input data
GPIOA_ODR    EQU  0x40020014  ;GPIO pin output data

;GPIO-B control registers
GPIOB_MODER  EQU  0x40020400  ;set GPIO pin mode as Input/Output/Analog
GPIOB_IDR    EQU  0x40020410  ;GPIO pin input data
GPIOB_ODR    EQU  0x40020414  ;GPIO pin output data



; **************************



; Export functions so they can be called from other file

  EXPORT SystemInit
  EXPORT __main

  AREA  MYCODE, CODE, READONLY
    
SID DCB 4, 3, 0, 6, 7	
; ******* Function SystemInit *******
; * Called from startup code
; * Calls - None
; * Enables GPIO clock 
; **************************

SystemInit FUNCTION

  ; Enable GPIO clock (enable all GPIOs)
  LDR    R1, =RCC_AHB1ENR  ;Pseudo-load address in R1
  LDR    R0, [R1]      	   ;Copy contents at address in R1 to R0
  ORR.W  R0, #0x03         ;Bitwise OR entire word in R0, result in R0
  STR    R0, [R1]          ;Store R0 contents to address in R1

  ; Set mode as output
  LDR    R1, =GPIOA_MODER  ;Two bits per pin so bits 24 to 31 control pins 12 to 15
  LDR    R0, [R1]      
  MOV    R0, #0x55555555   ;Mode bits set to '01' makes the pin mode as output
  STR    R0, [R1]
  
  ;set mode as input and output (PB12 TO PB15 OUTPUT , PB8 TO PB10 INPUT)
  LDR    R1,  =GPIOB_MODER
  LDR    R0,  [R1]
  MOV    R0, #0x55000000
  STR    R0, [R1]
  

  ENDFUNC
  
;MAIN_LOOP is the main loop of program
;NEXT WE JUMP TO LABEL XXX_PRESSED 
;NEXT WE STORE THE XXX IN REGISTER R2
;WRITE THE SQUARED VALUE IN PORT A ( 7 SEGMENT )
;NEXT WE JUMP TO MAIN_LOOP :)

__main FUNCTION
	
  LDR    R1, =GPIOA_ODR
  MOV    R0,#0xFFFF
  STR    R0, [R1]
  
  
MAIN_LOOP
  LDR    R2, =GPIOB_ODR
  MOV    R3, #0xEFFF
  STR    R3, [R2]
  LDR    R2, =GPIOB_IDR
  LDR    R0, [R2]
  MOV    R1, R0
  MOV    R4, #0xFEFF
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ONE_PRESSED
  
  MOV    R4, #0xFDFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    TWO_PRESSED
  
  MOV    R4, #0xFBFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    THREE_PRESSED
  
  LDR    R2, =GPIOB_ODR
  MOV    R3, #0xDFFF
  STR    R3, [R2]
  LDR    R2, =GPIOB_IDR
  LDR    R0, [R2]
  MOV    R1, R0
  MOV    R4, #0xFEFF
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    FOUR_PRESSED
  
  MOV    R4, #0xFDFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    FIVE_PRESSED
  
  MOV    R4, #0xFBFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ZERO_PRESSED
  
  LDR    R2, =GPIOB_ODR
  MOV    R3, #0xBFFF
  STR    R3, [R2]
  LDR    R2, =GPIOB_IDR
  LDR    R0, [R2]
  MOV    R1, R0
  MOV    R4, #0xFEFF
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ZERO_PRESSED
  
  MOV    R4, #0xFDFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ZERO_PRESSED
  
  MOV    R4, #0xFBFF
  MOV    R0, R1
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ZERO_PRESSED
  
  
  LDR    R2, =GPIOB_ODR
  MOV    R3, #0x7FFF
  STR    R3, [R2]
  LDR    R2, =GPIOB_IDR
  LDR    R0, [R2]
  MOV    R1, R0
  MOV    R4, #0xFDFF
  ORR    R0, R0, R4
  CMP    R0, R4
  BEQ    ZERO_PRESSED
  
  
  B     MAIN_LOOP
  
ONE_PRESSED
  LDR    R1, =GPIOA_ODR
  LDR    R2, =SID
  LDRSB	 R3, [R2]
  MOV	 R4, R3
  MUL	 R3, R3, R3
  MUL	 R3, R3, R4
ONE_LOOP
  CMP	 R3, #100
  BLS	 ONE_SHOW
  SUBS	 R3, R3, #100  
  B		 ONE_LOOP
ONE_SHOW
  STR    R3, [R1]
  B 	 MAIN_LOOP
  
TWO_PRESSED
  LDR    R1, =GPIOA_ODR
  LDR    R2, =SID
  ADDS	 R2, R2, #1
  LDRSB	 R3, [R2]
  MOV	 R4, R3
  MUL	 R3, R3, R3
  MUL	 R3, R3, R4
TWO_LOOP
  CMP	 R3, #100
  BLS	 TWO_SHOW
  SUBS	 R3, R3, #100  
  B		 TWO_LOOP
TWO_SHOW
  STR    R3, [R1]
  B 	 MAIN_LOOP
  
THREE_PRESSED
  LDR    R1, =GPIOA_ODR
  LDR    R2, =SID
  ADDS	 R2, R2, #2
  LDRSB	 R3, [R2]
  MOV	 R4, R3
  MUL	 R3, R3, R3
  MUL	 R3, R3, R4
THREE_LOOP
  CMP	 R3, #100
  BLS	 THREE_SHOW
  SUBS	 R3, R3, #100  
  B		 THREE_LOOP
THREE_SHOW
  STR    R3, [R1]
  B 	 MAIN_LOOP

FOUR_PRESSED
  LDR    R1, =GPIOA_ODR
  LDR    R2, =SID
  ADDS	 R2, R2, #3
  LDRSB	 R3, [R2]
  MOV	 R4, R3
  MUL	 R3, R3, R3
  MUL	 R3, R3, R4
FOUR_LOOP
  CMP	 R3, #100
  BLS	 FOUR_SHOW
  SUBS	 R3, R3, #100  
  B		 FOUR_LOOP
FOUR_SHOW
  STR    R3, [R1]
  B 	 MAIN_LOOP

FIVE_PRESSED
  LDR    R1, =GPIOA_ODR
  LDR    R2, =SID
  ADDS	 R2, R2, #4
  LDRSB	 R3, [R2]
  MOV	 R4, R3
  MUL	 R3, R3, R3
  MUL	 R3, R3, R4
FIVE_LOOP
  CMP	 R3, #100
  BLS	 FIVE_SHOW
  SUBS	 R3, R3, #100  
  B		 FIVE_LOOP
FIVE_SHOW
  STR    R3, [R1]
  B 	 MAIN_LOOP
  
  
ZERO_PRESSED
  LDR    R1, =GPIOA_ODR
  MOV    R2,#0
  STR    R2, [R1]
  B 	 MAIN_LOOP
    
  END