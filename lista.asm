; ==========================================================
; MODULO: lista.asm (Memoria Dinamica en ES y Listas)
; ==========================================================
; Convencion en variables de DS:
; [DS + 0] = cabeza de la lista (puntero al primer nodo)
; [DS + 4] = offset del proximo byte libre dentro de ES

; SUBRUTINA: ALLOC
; Entrada: ECX = cantidad de bytes a reservar
; Salida:  EDX = direccion logica del bloque en ES (ES + offset libre)
ALLOC: MOV EDX, ES
    MOV EEX, DS
    MOV EFX, [EEX+4]       ; EFX = offset libre actual
    ADD EDX, EFX          ; EDX = ES + offset (puntero al bloque)
    ADD EFX, ECX          ; incrementa el proximo libre
    MOV [EEX+4], EFX      ; actualiza en memoria DS
    RET

; SUBRUTINA: INSERTAR (agrega un nodo a la cabeza de la lista)
; Entrada: EAX = valor numerico a guardar
INSERTAR: MOV ECX, 8      ; cada nodo mide 8 bytes (4 dato + 4 siguiente)
    CALL ALLOC            ; EDX devuelve la direccion del nuevo nodo
    MOV [EDX+0], EAX      ; nuevo->dato = EAX
    MOV EEX, DS
    MOV EFX, [EEX+0]      ; EFX = cabeza actual
    MOV [EDX+4], EFX      ; nuevo->siguiente = cabeza anterior
    MOV [EEX+0], EDX      ; cabeza = nuevo nodo
    RET