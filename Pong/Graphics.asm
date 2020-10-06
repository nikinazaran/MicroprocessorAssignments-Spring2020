title (exe) Graphics System Calls 

.model small 
.stack 64   

.data 
     start_col dw   295 
     start_row dw   150
     finish_col dw  300
     finish_row dw  190
     board_down dw  0
     board_up dw    0
     BALL_SC DW     200
     BALL_SR DW     160
     BALL_FC DW     210
     BALL_FR DW     170
     DIR DB         1 ; 1=UL, 2=UR, 3=DL, 4=DR
     INNER_FL DW    190
     INNER_CL DW    40
     INNER_WL DW    100
     SCORE  DW      0
     SCORE1 DB      48
     SCORE2 DB      48
     SCORE1_LIMIT DB   51
     SCORE2_LIMIT DB   48
     SCORE_C DB     23        
     SCORE_R DB     2
     COLOR DB       0Fh
     CYW DW         "CONGRATS! YOU WON!$"
     BLOCK_SC DW    120
     BLOCK_FC DW    136
     BLOCK_SR DW    120
     BLOCK_FR DW    136
     BLOCK_STR DB   0
     BLOCK_STR_C DB 0
     BLOCK_STR_R DB 0
     BLOCK_ISON DB  0
     

.code 

#start=led_display.exe#

main proc far
    mov ax, @data
    mov ds, ax   
    call clear_screen    
    call set_graphic_mode

;--------------Set initial score--------------    

    MOV DH, SCORE_R
    MOV DL, SCORE_C
    MOV BH, 0
    MOV AH, 2
    INT 10H
    
    MOV AH, 2
    MOV DL, SCORE1
    INT 21H
    INT 21H
           
    MOV AX, SCORE
    OUT 199, AX
    
    CALL MAKE_BLOCK
    
               
;--------------Drawing the board--------------    

    call draw_rectangle    
    
;--------------Drawing the frame--------------

    MOV AH, 0Ch
    MOV AL, 0Fh
;-------WALL-------
    MOV DX, 35
F1: 
    MOV CX, 95
F2:
    INT 10h
    INC CX
    CMP CX, 101
    JNZ F2
    
    INC DX
    CMP DX, 196
    JNZ F1
;-------CEILING-------    
    MOV DX, 35
F3:
    MOV CX, 95
F4:
    INT 10h
    INC CX
    CMP CX, 286
    JNZ F4
    
    INC DX
    CMP DX, 41
    JNZ F3
;-------FLOOR-------    
    MOV DX, 190
F5:
    MOV CX, 95
F6:
    INT 10h
    INC CX
    CMP CX, 286
    JNZ F6
    
    INC DX
    CMP DX, 196
    JNZ F5             

;--------------Drawing the ball--------------

    CALL DRAW_BALL  
    
;--------------Moving the ball--------------                                                            

BALL_MOVE:
    CALL DELAY
    CALL BALL_DIR
    CALL DELAY
    MOV AH, 01h
    INT 16h
    JNZ KEYBOARD_IN
    JMP BALL_MOVE
    
    

;--------------Keyboard--------------                                                        

KEYBOARD_IN:
    MOV AH, 00H                   
    INT 16H
    CMP AL, 115
    JNZ n1
    CMP FINISH_ROW, 190
    JZ BALL_MOVE
    JMP SHIFT_DOWN
n1: 
    CMP AL, 119
    JNZ n2
    CMP START_ROW, 40
    JZ BALL_MOVE
    JMP SHIFT_UP
n2:
    CMP AL, 113
    JZ DONE
    
    JMP BALL_MOVE


SHIFT_DOWN:  
    MOV AX, finish_row
    ADD AX, 10      
    MOV BOARD_DOWN, AX    
SD:                  
    call shift_rectangle_D
    call delayS
    MOV  BX, FINISH_ROW
    JPO  MD
    CALL BALL_DIR
    CALL DELAYN                                             
MD: MOV AX, BOARD_DOWN
    cmp finish_row, AX
    jnz SD
    JMP BALL_MOVE
    
    
    
SHIFT_UP:
    MOV AX, START_ROW
    SUB AX, 10      
    MOV BOARD_UP, AX    
SU:                  
    call shift_rectangle_U
    call delayS
    MOV BX, START_ROW
    JPO MU
    CALL BALL_DIR
    CALL DELAYN
MU: MOV AX, BOARD_UP
    cmp START_row, AX
    jnz SU
    JMP BALL_MOVE
    

DONE:
    mov ax, 4c00h ; exit to operating system.
    int 21h

endp main

;---------------------------------------------

ADD_SCORE PROC
    CALL MAKE_BLOCK
    
    INC SCORE
    MOV AX, SCORE
    OUT 199, AX
               

    CMP SCORE2, 57
    JNZ l1
    MOV DH, SCORE_R
    MOV DL, SCORE_C
    MOV BH, 0
    MOV AH, 2
    INT 10H
    INC SCORE1
    MOV DL, SCORE1
    MOV AH, 2
    INT 21H
    MOV DL, 48
    INT 21H
    MOV SCORE2, 48
    
    MOV AL, SCORE1
    CMP AL, SCORE1_LIMIT
    JNZ ENDADDSCORE
    MOV AL, SCORE2
    CMP AL, SCORE2_LIMIT
    JNZ ENDADDSCORE
    
    ;CONGRATS, YOU WON
    
    MOV DH, 10
    MOV DL, SCORE_C
    MOV BH, 0
    MOV AH, 2
    INT 10H
    
    MOV DX, OFFSET CYW
    MOV AH, 09H
    INT 21H
    
    mov ax, 4c00h ; exit to operating system.
    int 21h
    

l1:
    MOV DH, SCORE_R
    MOV DL, SCORE_C
    MOV BH, 0
    MOV AH, 2 
    INC DL
    INT 10H
    INC SCORE2
    MOV DL, SCORE2
    MOV AH, 2
    INT 21H
    
    MOV AL, SCORE1
    CMP AL, SCORE1_LIMIT
    JNZ ENDADDSCORE
    MOV AL, SCORE2
    CMP AL, SCORE2_LIMIT
    JNZ ENDADDSCORE
    
    ;CONGRATS, YOU WON
    
    MOV DH, 13
    MOV DL, 16
    MOV BH, 0
    MOV AH, 2
    INT 10H
    
    MOV DX, OFFSET CYW
    MOV AH, 09H
    INT 21H
    
    mov ax, 4c00h ; exit to operating system.
    int 21h
    
ENDADDSCORE:

    RET           
ENDP ADD_SCORE    

;---------------------------------------------

DRAW_BALL PROC
    
    MOV AH, 0Ch
    MOV AL, COLOR
    MOV DX, BALL_SR
B1:
    MOV CX, BALL_SC
B2:
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE B2
    
    INC DX
    CMP DX, BALL_FR
    JLE B1

    RET
ENDP DRAW_BALL

;---------------------------------------------

CHANGE_COLOR PROC
CC:    
    MOV AH, 00h
    INT 1Ah
    MOV AL, DL
    MOV BL, 16
    DIV BL
    CMP AH, 0
    JZ  CC
    CMP AH, COLOR
    JZ  CC
    MOV COLOR, AH
    CALL DRAW_BALL    
    RET
ENDP CHANGE_COLOR

;---------------------------------------------

BALL_DIR PROC ; 1=UL, 2=UR, 3=DL, 4=DR

;---UL---
DIR1:
    CMP DIR, 1
    JNZ DIR2
    
    CMP BLOCK_ISON, 1
    JNZ W1
    MOV AX, BALL_SR
    SUB AX, BLOCK_FR
    CMP AX, 1
    JNZ BLC1
    MOV AX, BALL_SC
    CMP AX, BLOCK_FC
    JG  BLC1
    MOV AX, BALL_FC
    CMP AX, BLOCK_SC
    JL  BLC1
    CALL BLOCK_DEC_STR
    MOV DIR, 3
    CALL BALL_MOVE_DOWN_LEFT
    RET
    
BLC1:
    MOV AX, BALL_SC
    SUB AX, BLOCK_FC
    CMP AX, 1
    JNZ W1
    MOV AX, BALL_SR
    CMP AX, BLOCK_FR
    JG  W1
    MOV AX, BALL_FR
    CMP AX, BLOCK_SR
    JL  W1
    CALL BLOCK_DEC_STR
    MOV DIR, 2
    CALL BALL_MOVE_UP_RIGHT
    RET
    
W1:
    MOV AX, BALL_SR    
    SUB AX, INNER_CL
    CMP AX, 1
    JNZ CHECK1
    MOV DIR, 3
    CALL BALL_MOVE_DOWN_LEFT
    RET
    
CHECK1:
    MOV AX, BALL_SC    
    SUB AX, INNER_WL
    CMP AX, 1
    JNZ MOV1
    MOV DIR, 2
    CALL BALL_MOVE_UP_RIGHT 
    RET
MOV1:
    CALL BALL_MOVE_UP_LEFT
    RET
        
;---UR---    
DIR2:
    CMP DIR, 2
    JNZ DIR3
    
    CMP BLOCK_ISON, 1
    JNZ W2
    MOV AX, BALL_SR
    SUB AX, BLOCK_FR
    CMP AX, 1
    JNZ BLC2
    MOV AX, BALL_SC
    CMP AX, BLOCK_FC
    JG  BLC2
    MOV AX, BALL_FC
    CMP AX, BLOCK_SC
    JL  BLC2
    CALL BLOCK_DEC_STR
    MOV DIR, 4
    CALL BALL_MOVE_DOWN_RIGHT
    RET
    
BLC2:
    MOV AX, BLOCK_SC
    SUB AX, BALL_FC
    CMP AX, 1
    JNZ W2
    MOV AX, BALL_SR
    CMP AX, BLOCK_FR
    JG  W2
    MOV AX, BALL_FR
    CMP AX, BLOCK_SR
    JL  W2
    CALL BLOCK_DEC_STR
    MOV DIR, 1
    CALL BALL_MOVE_UP_LEFT
    RET
    
W2:    
    MOV AX, BALL_SR    
    SUB AX, INNER_CL
    CMP AX, 1
    JNZ CHECK2
    MOV DIR, 4
    CALL BALL_MOVE_DOWN_RIGHT
    RET
    
CHECK2: ;MAY OR MAY NOT HIT THE BOARD 
    MOV AX, START_COL
    SUB AX, BALL_FC
    CMP AX, 1
    JNZ MOV2
    MOV AX, BALL_SR
    CMP AX, FINISH_ROW
    JGE GAMEOVER ;GAME OVER
    MOV AX, BALL_FR
    CMP AX, START_ROW
    JLE GAMEOVER ;GAME OVER
    ;SCORE++
    CALL ADD_SCORE
    CALL CHANGE_COLOR
    MOV DIR, 1
    CALL BALL_MOVE_UP_LEFT
    RET

MOV2:
    CALL BALL_MOVE_UP_RIGHT
    RET
    
;---DL---    
DIR3:       
    CMP DIR, 3
    JNZ DIR4
    
    CMP BLOCK_ISON, 1
    JNZ W3
    MOV AX, BLOCK_SR
    SUB AX, BALL_FR
    CMP AX, 1
    JNZ BLC3
    MOV AX, BALL_SC
    CMP AX, BLOCK_FC
    JG  BLC3
    MOV AX, BALL_FC
    CMP AX, BLOCK_SC
    JL  BLC3
    CALL BLOCK_DEC_STR
    MOV DIR, 1
    CALL BALL_MOVE_UP_LEFT
    RET
    
BLC3:
    MOV AX, BALL_SC
    SUB AX, BLOCK_FC
    CMP AX, 1
    JNZ W3
    MOV AX, BALL_SR
    CMP AX, BLOCK_FR
    JG  W3
    MOV AX, BALL_FR
    CMP AX, BLOCK_SR
    JL  W3
    CALL BLOCK_DEC_STR
    MOV DIR, 4
    CALL BALL_MOVE_DOWN_RIGHT
    RET
    
W3:
    MOV AX, INNER_FL    
    SUB AX, BALL_FR
    CMP AX, 1
    JNZ CHECK3
    MOV DIR, 1
    CALL BALL_MOVE_UP_LEFT
    RET
    
CHECK3:
    MOV AX, BALL_SC    
    SUB AX, INNER_WL
    CMP AX, 1
    JNZ MOV3
    MOV DIR, 4
    CALL BALL_MOVE_DOWN_RIGHT
    RET
    
MOV3:    
    CALL BALL_MOVE_DOWN_LEFT
    RET
    
;---DR---    
DIR4:
    
    CMP BLOCK_ISON, 1
    JNZ W4
    MOV AX, BLOCK_SR
    SUB AX, BALL_FR
    CMP AX, 1
    JNZ BLC4
    MOV AX, BALL_SC
    CMP AX, BLOCK_FC
    JG  BLC4
    MOV AX, BALL_FC
    CMP AX, BLOCK_SC
    JL  BLC4
    CALL BLOCK_DEC_STR
    MOV DIR, 2
    CALL BALL_MOVE_UP_RIGHT
    RET
    
BLC4:
    MOV AX, BLOCK_SC
    SUB AX, BALL_FC
    CMP AX, 1
    JNZ W4
    MOV AX, BALL_SR
    CMP AX, BLOCK_FR
    JG  W4
    MOV AX, BALL_FR
    CMP AX, BLOCK_SR
    JL  W4
    CALL BLOCK_DEC_STR
    MOV DIR, 1
    CALL BALL_MOVE_DOWN_LEFT
    RET  
      
      
W4:
    MOV AX, INNER_FL    
    SUB AX, BALL_FR
    CMP AX, 1
    JNZ CHECK4
    MOV DIR, 2
    CALL BALL_MOVE_UP_RIGHT
    RET
    
CHECK4: ;MAY OR MAY NOT HIT THE BOARD      
    MOV AX, START_COL
    SUB AX, BALL_FC
    CMP AX, 1
    JNZ MOV4
    MOV AX, BALL_SR
    CMP AX, FINISH_ROW
    JGE GAMEOVER ;GAME OVER
    MOV AX, BALL_FR
    CMP AX, START_ROW
    JLE GAMEOVER ;GAME OVER
    ;SCORE++
    CALL ADD_SCORE
    CALL CHANGE_COLOR
    MOV DIR, 3
    CALL BALL_MOVE_DOWN_LEFT
    RET   
       
MOV4:    
    CALL BALL_MOVE_DOWN_RIGHT
    RET
    
GAMEOVER:
    mov ax, 4c00h ; exit to operating system.
    int 21h    
    
ENDP BALL_DIR




clear_screen proc
    mov al, 06h ; scroll down
    mov bh, 00h
    mov cx, 0000h
    mov dx, 184fh
    int 10h
                 
    ret                    
endp clear_screen 

set_graphic_mode proc
    mov ah, 00h
    mov al, 13h
    int 10h 
    
    ret
endp set_graphic_mode


draw_rectangle proc 
    mov ah, 0ch 
    mov al, 0Fh
      
    mov dx, start_row
loop1:
    mov cx, start_col

loop2:
    int 10h
    inc cx
    cmp cx, finish_col
    jnz loop2
    
    inc dx
    cmp dx, finish_row
    jnz loop1
    
    ret
    
endp draw_rectangle 
                                
;----------------------
shift_rectangle_D proc
    mov ah, 0ch 
    mov al, 0
    mov dx, start_row
    mov cx, start_col 
D1:  
    int 10h
    inc cx
    cmp cx, finish_col
    jnz D1
              
              
    mov al, 0Fh
    mov dx, finish_row
    mov cx, start_col 
D2:
    int 10h    
    inc cx
    cmp cx, finish_col
    jnz D2 
    
    inc start_row
    inc finish_row
    
    ret                        
endp shift_rectangle_D

;----------------------
shift_rectangle_U proc
    mov ah, 0ch 
    mov al, 0
    mov dx, FINISH_row
    mov cx, start_col 
U1:  
    int 10h
    inc cx
    cmp cx, finish_col
    jnz U1
              
              
    mov al, 0Fh
    mov dx, START_row
    mov cx, start_col 
U2:
    int 10h    
    inc cx
    cmp cx, finish_col
    jnz U2 
    
    DEC start_row
    DEC finish_row
    
    ret                        
endp shift_rectangle_U

;----------------------
delay proc
    ;MOV CX, 4fffH
    MOV CX, 5500h
loop5:
    loop loop5
    ret
endp delay    

delayS proc
    ;MOV CX, 4444H
    MOV CX, 3500H
loop6:
    loop loop6
    ret
endp delayS

delayN proc
    ;MOV CX, 4444H
    MOV CX, 2000H
loop7:
    loop loop7
    ret
endp delayN           
            
;----------------------
BALL_MOVE_UP_LEFT PROC

    MOV AH, 0Ch
    MOV AL, 0
    MOV DX, BALL_FR
    MOV CX, BALL_SC
UL1:    
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE UL1
    
    MOV CX, BALL_FC
    MOV DX, BALL_SR
UL2:
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE UL2
        
    DEC BALL_SR
    DEC BALL_FR
    DEC BALL_SC
    DEC BALL_FC
        
    MOV AL, COLOR
    MOV DX, BALL_SR
    MOV CX, BALL_SC
UL3:
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE UL3
    
    MOV CX, BALL_SC
    MOV DX, BALL_SR
UL4:    
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE UL4    
        
    RET
ENDP BALL_MOVE_UP_LEFT

;----------------------
BALL_MOVE_UP_RIGHT PROC

    MOV AH, 0Ch
    MOV AL, 0
    MOV DX, BALL_FR
    MOV CX, BALL_SC
UR1:    
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE UR1
    
    MOV CX, BALL_SC
    MOV DX, BALL_SR
UR2:
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE UR2
        
    DEC BALL_SR
    DEC BALL_FR
    INC BALL_SC
    INC BALL_FC
        
    MOV AL, COLOR
    MOV DX, BALL_SR
    MOV CX, BALL_SC
UR3:
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE UR3
    
    MOV CX, BALL_FC
    MOV DX, BALL_SR
UR4:    
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE UR4    
        
    RET
ENDP BALL_MOVE_UP_RIGHT

;----------------------
BALL_MOVE_DOWN_LEFT PROC

    MOV AH, 0Ch
    MOV AL, 0
    MOV DX, BALL_SR
    MOV CX, BALL_SC
DL1:    
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE DL1
    
    MOV CX, BALL_FC
    MOV DX, BALL_SR
DL2:
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE DL2
        
    INC BALL_SR
    INC BALL_FR
    DEC BALL_SC
    DEC BALL_FC
        
    MOV AL, COLOR
    MOV DX, BALL_FR
    MOV CX, BALL_SC
DL3:
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE DL3
    
    MOV CX, BALL_SC
    MOV DX, BALL_SR
DL4:    
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE DL4    
        
    RET
ENDP BALL_MOVE_DOWN_LEFT

;----------------------
BALL_MOVE_DOWN_RIGHT PROC

    MOV AH, 0Ch
    MOV AL, 0
    MOV DX, BALL_SR
    MOV CX, BALL_SC
DR11:    
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE DR11
    
    MOV CX, BALL_SC
    MOV DX, BALL_SR
DR22:
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE DR22
        
    INC BALL_SR
    INC BALL_FR
    INC BALL_SC
    INC BALL_FC
        
    MOV AL, COLOR
    MOV DX, BALL_FR
    MOV CX, BALL_SC
DR33:
    INT 10h
    INC CX
    CMP CX, BALL_FC
    JLE DR33
    
    MOV CX, BALL_FC
    MOV DX, BALL_SR
DR44:    
    INT 10h
    INC DX
    CMP DX, BALL_FR
    JLE DR44
        
    RET
ENDP BALL_MOVE_DOWN_RIGHT

;---------------------------------------------

MAKE_BLOCK PROC         
    CMP BLOCK_ISON, 1
    JZ NDONE
    
    ;GETTING RANDOM STRENGTH
    MOV AH, 00h
    INT 1Ah
    MOV AL, DL
    MOV BL, 10 
    DIV BL
    MOV BLOCK_STR, AH    
    ADD BLOCK_STR, 49
    CMP AH, 5
    JL  NI1
    RET

NI1:
    ;GETTING RANDOM START ROW
    MOV AH, 00h
    INT 1Ah
    MOV AL, DL
    MOV BL, 17
    DIV BL
    MOV AL, AH
    MOV BL, 8
    MUL BL
    ADD AX, 43
    MOV BLOCK_SR, AX
    ADD AX, 16
    MOV BLOCK_FR, AX
    
    MOV DX, 0
    MOV AX, BLOCK_SR
    MOV BX, 8
    DIV BX
    MOV BLOCK_STR_R, AL
    INC BLOCK_STR_R
                    
    ;GETTING RANDOM START COLUMN
    MOV AH, 00h
    INT 1Ah
    MOV AL, DL
    MOV BL, 21
    DIV BL
    MOV AL, AH
    MOV BL, 8
    MUL BL
    ADD AX, 107
    MOV BLOCK_SC, AX
    ADD AX, 16
    MOV BLOCK_FC, AX
    
    MOV DX, 0
    MOV AX, BLOCK_SC
    MOV BX, 8
    DIV BX
    MOV BLOCK_STR_C, AL
    INC BLOCK_STR_C
                        
    MOV AH, 0Ch
    MOV AL, 2
    MOV DX, BLOCK_SR
BL1:
    MOV CX, BLOCK_SC
BL2:
    INT 10h
    INC CX
    CMP CX, BLOCK_FC
    JLE BL2
    
    INC DX
    CMP DX, BLOCK_FR
    JLE BL1    
    
    MOV DH, BLOCK_STR_R
    MOV DL, BLOCK_STR_C
    MOV BH, 0
    MOV AH, 2
    INT 10H
    
    MOV AH, 2
    MOV DL, BLOCK_STR
    INT 21H
    
    MOV BLOCK_ISON, 1
NDONE:
    RET
ENDP MAKE_BLOCK

BLOCK_DEC_STR PROC
    DEC BLOCK_STR
    CMP BLOCK_STR, 48
    JNZ KAM
    MOV BLOCK_ISON, 0
    MOV AH, 0Ch
    MOV AL, 0
    MOV DX, BLOCK_SR
BLR1:
    MOV CX, BLOCK_SC
BLR2:
    INT 10h
    INC CX
    CMP CX, BLOCK_FC
    JLE BLR2
    
    INC DX
    CMP DX, BLOCK_FR
    JLE BLR1
    
    RET
    
KAM:
    MOV DH, BLOCK_STR_R
    MOV DL, BLOCK_STR_C
    MOV BH, 0
    MOV AH, 2
    INT 10H
    
    MOV AH, 2
    MOV DL, BLOCK_STR
    INT 21H 
     
    RET     
ENDP BLOCK_DEC_SRT


end main