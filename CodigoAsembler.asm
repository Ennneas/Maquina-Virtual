; =====================================================================
; Programa de prueba para la Máquina Virtual (MV2)
; Prueba integral con 4 BREAKPOINTS (SYS 0xF):
;   BP #1: En main, al iniciar (verifica tabla de segmentos, pila y DS[0]=ES)
;   BP #2: En MALLOC, tras reservar en ES (verifica EAX y avance de DS[0])
;   BP #3: En SCOPY, tras copiar a ES (verifica bytes copiados en ES)
;   BP #4: En fin_programa (verifica estado final de registros y memoria)
; =====================================================================

; --- Declaración de Constantes de Texto (Const Segment) ---
MSJ_SALUDO EQU "Hola Mundo desde KS (sin -p)!"
MSJ_COPIA  EQU "Copiado en ES: "
MSJ_FIN    EQU "\n"

; --- Directivas de Configuración de Segmentos ---
\\DATA 1024
\\EXTRA 1024
\\STACK 1024

; =====================================================================
; Subrutina: SLEN
; Parámetro: [BP + 8] = Puntero de 4 bytes a la cadena
; Retorno:   ECX      = Cantidad de caracteres (longitud sin '\0')
; =====================================================================
SLEN: PUSH BP
    MOV BP, SP

    MOV ECX, 0              ; Inicializar contador de longitud
    PUSH EDX                ; Preservar EDX del llamador
    MOV EDX, [BP+8]         ; Leer puntero a la cadena desde la pila

loop_slen: CMP b[EDX], 0    ; ¿Llegamos al carácter nulo '\0'?
    JZ fin_slen

    ADD ECX, 1              ; Incrementar contador
    ADD EDX, 1              ; Avanzar al siguiente byte
    JMP loop_slen

fin_slen: POP EDX           ; Restaurar EDX
    POP BP                  ; Restaurar BP
    RET

; =====================================================================
; Subrutina: MALLOC (Reserva de memoria dinámica en Extra Segment)
; Parámetro: [BP + 8] = Cantidad de bytes a reservar
; Retorno:   EAX      = Puntero lógico al bloque reservado en ES
; Usa:       [DS + 0] = Puntero libre actual del Heap (en ES)
; =====================================================================
MALLOC: PUSH BP
    MOV BP, SP

    MOV EAX, [DS]           ; EAX = dirección libre actual en ES
    ADD [DS], [BP+8]        ; Avanzar el puntero del Heap la cantidad pedida

    ; >>> BREAKPOINT #2: verificar EAX (bloque asignado) y [DS] (nuevo tope de ES)
    SYS 0xF

    POP BP
    RET

; =====================================================================
; Subrutina: SCOPY (Copia una cadena a memoria dinámica en ES)
; Parámetros: [BP + 8]  = Puntero origen (PS o KS)
;             [BP + 12] = Puntero destino (ES)
; =====================================================================
SCOPY: PUSH BP
    MOV BP, SP
    PUSH EDX
    PUSH EBX

    MOV EDX, [BP+8]         ; EDX = origen
    MOV EBX, [BP+12]        ; EBX = destino en ES

loop_scopy: CMP b[EDX], 0   ; ¿Fin de cadena origen?
    JZ fin_scopy

    MOV [EBX], [EDX]        ; Copiar carácter hacia ES
    ADD EDX, 1
    ADD EBX, 1
    JMP loop_scopy

fin_scopy: MOV b[EBX], 0    ; Colocar terminador '\0' en ES

    ; >>> BREAKPOINT #3: verificar la cadena recién copiada en ES
    SYS 0xF

    POP EBX
    POP EDX
    POP BP
    RET

; =====================================================================
; Punto de Entrada Principal (Entry Point)
; Pila al iniciar main:
;   [BP + 8]  = argc (cantidad de parámetros pasados con -p)
;   [BP + 12] = argv (puntero al arreglo de parámetros en PS)
; =====================================================================
main: PUSH BP
    MOV BP, SP

    ; 1. Inicializar el puntero del Heap en DS[0] apuntando al inicio de ES
    MOV [DS], ES

    ; >>> BREAKPOINT #1: estado inicial (segmentos, SP/BP y [DS]=ES)
    SYS 0xF

    ; 2. Verificar si existe Param Segment (flag -p)
    CMP PS, -1
    JZ modo_ks              ; Si PS == -1, no se pasó -p: probar con KS

    ; --- Modo CON parámetros (-p): recorrer todos los strings del PS ---
    MOV EFX, PS             ; EFX = cursor actual dentro del Param Segment
    MOV EEX, 0              ; EEX = índice de parámetro actual (0 .. argc)

loop_params: CMP EEX, [BP+8]
    JP fin_programa         ; Si EEX > argc, ya copiamos todos los parámetros

    ; Medir longitud del parámetro actual en PS
    PUSH EFX
    CALL SLEN               ; ECX = longitud del string actual
    ADD SP, 4

    ; Reservar (ECX + 1) bytes dinámicamente en ES
    MOV EBX, ECX
    ADD EBX, 1
    PUSH EBX
    CALL MALLOC             ; EAX = nuevo bloque reservado en ES
    ADD SP, 4

    ; Copiar el parámetro desde PS (EFX) hacia ES (EAX)
    PUSH EAX                ; Destino en ES ([BP+12] en SCOPY)
    PUSH EFX                ; Origen en PS  ([BP+8] en SCOPY)
    CALL SCOPY
    ADD SP, 8

    ; Imprimir desde el Extra Segment (ES)
    MOV EDX, KS
    ADD EDX, MSJ_COPIA
    SYS 0x04
    MOV EDX, EAX            ; EDX apunta al bloque dinámico en ES
    SYS 0x04
    MOV EDX, KS
    ADD EDX, MSJ_FIN
    SYS 0x04

    ; Avanzar EFX al siguiente string dentro de PS (longitud + 1 por el '\0')
    ADD EFX, ECX
    ADD EFX, 1
    ADD EEX, 1
    JMP loop_params

    ; --- Modo SIN parámetros: copiar MSJ_SALUDO de KS a ES ---
modo_ks: MOV EFX, KS
    ADD EFX, MSJ_SALUDO

    PUSH EFX
    CALL SLEN
    ADD SP, 4

    MOV EBX, ECX
    ADD EBX, 1
    PUSH EBX
    CALL MALLOC
    ADD SP, 4

    PUSH EAX
    PUSH EFX
    CALL SCOPY
    ADD SP, 8

    MOV EDX, KS
    ADD EDX, MSJ_COPIA
    SYS 0x04
    MOV EDX, EAX
    SYS 0x04
    MOV EDX, KS
    ADD EDX, MSJ_FIN
    SYS 0x04

fin_programa: SYS 0xF       ; >>> BREAKPOINT #4: estado final de registros y ES
    POP BP
    RET
