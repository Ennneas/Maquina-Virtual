\\DATA 100
\\EXTRA 500
\\STACK 100
\\INCLUDE "lista.asm"

STR_TITULO EQU "=== Test Lista Enlazada en Extra Segment ===\n"

main: MOV EDX, KS
    ADD EDX, STR_TITULO
    SYS 4

    ; 1. Inicializar la lista vacia (cabeza = 0) y offset de ES en 0
    MOV EDX, DS
    MOV EAX, 0
    MOV [EDX+0], EAX      ; [DS+0] = 0 (cabeza inicial en NULL)
    MOV [EDX+4], EAX      ; [DS+4] = 0 (offset libre de ES arranca en 0)

    ; 2. Insertamos 3 nodos dinamicamente en ES
    MOV EAX, 10
    CALL INSERTAR

    MOV EAX, 20
    CALL INSERTAR

    MOV EAX, 30
    CALL INSERTAR

    ; 3. Recorrer la lista enlazada desde la cabeza hasta el final
    MOV EDX, DS
    MOV EBX, [EDX+0]      ; EBX = puntero al primer nodo

BUCLE: CMP EBX, 0
    JZ FIN_LISTA          ; si el puntero es 0, terminamos

    ; Imprimir el valor del nodo actual con SYS 2 (modo 1 = decimal)
    MOV EDX, EBX          ; apunta a [EBX+0] (el dato)
    MOV EAX, 1            ; modo decimal
    LDH ECX, 4            ; celdas de 4 bytes
    LDL ECX, 1            ; imprimir 1 celda
    SYS 2

    ; Avanzar al siguiente nodo: EBX = nodo->siguiente
    MOV EBX, [EBX+4]
    JMP BUCLE

FIN_LISTA: RET