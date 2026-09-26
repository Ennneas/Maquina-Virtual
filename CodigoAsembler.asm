MOV [10], 0x41
SHL [10], 8
OR [10], 'a'
MOV EDX, DS
ADD EDX, 12; EDX apunta a DS+12
; Armamos el primer 2147483647 en EAX
LDL EAX, 0xFFFF
LDH EAX, 0x7FFF
; Armamos el segundo 2147483647 en EBX
LDL EBX, 0xFFFF
LDH EBX, 0x7FFF
; Movemos el primero a la memoria
MOV [12], EAX
; Sumamos el segundo a la memoria
ADD [12], EBX
JV ESCRITURA
MOV [12], 1
ESCRITURA: LDL ECX, 1
LDH ECX, 4
MOV EAX, 0x01 
SYS 0x2
STOP