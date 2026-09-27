inicio: MOV EAX, 10
    MOV EBX, -20
    SWAP EAX, EBX
    JN  NEGATIVO

    MOV [50], 0         ; no NEGATIVO -> guarda 0
    JMP fin
NEGATIVO: MOV [50], 1         ; NEGATIVO -> guarda 1

fin: MOV EDX, DS         ; EDX = puntero al segmento de datos
    ADD EDX, 50         ; EDX apunta a la celda [50]
    MOV ECX, 0x00010001 ; ECX: 1 celda de tamaño 1 byte (o ajustar según tu SYS)
    LDH ECX, 4          ; tamaño de celda = 1
    LDL ECX, 1          ; cantidad de celdas = 1
    MOV EAX, 0x01       ; modo de impresión = decimal
    SYS 2               ; llamada WRITE: muestra el valor de [50] por consola

    STOP
