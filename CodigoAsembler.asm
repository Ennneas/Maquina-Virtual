<<<<<<< Updated upstream
MOV [10], 0x41
SHL [10], 8
OR [10], 'a'
MOV EDX, DS
ADD EDX, 12; EDX apunta a DS+12
MOV [12], 13
SHL [12], 32
JC ESCRITURA
MOV [12], 1
ESCRITURA: LDL ECX, 1
LDH ECX, 4
MOV EAX, 0x01 
SYS 0x2
STOP
=======
inicio: MOV EAX, 32767      ; EAX = 32767 (0x7FFF)
    SHL EAX, 20         ; EAX << 20 -> excede 32 bits -> genera C=1
    JC  hubocarry

    MOV [50], 0         ; no hubo carry -> guarda 0
    JMP fin


hubocarry: MOV [50], 1         ; hubo carry -> guarda 1


fin: MOV EDX, DS         ; EDX = puntero al segmento de datos
    ADD EDX, 50         ; EDX apunta a la celda [50]
    MOV ECX, 0x00010001 ; ECX: 1 celda de tamaño 1 byte (o ajustar según tu SYS)
    LDH ECX, 1          ; tamaño de celda = 1
    LDL ECX, 1          ; cantidad de celdas = 1
    MOV EAX, 0x01       ; modo de impresión = decimal
    SYS 2               ; llamada WRITE: muestra el valor de [50] por consola

    STOP
>>>>>>> Stashed changes
