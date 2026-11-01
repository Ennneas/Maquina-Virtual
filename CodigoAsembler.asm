\\DATA 100
\\STACK 100

; ==========================================================
; Cadenas constantes en el Const Segment (KS)
; ==========================================================
STR_TITULO  EQU "=== Test Completo MV2: SUB, Subrutinas, KS, Params e I/O ===\n"
STR_SUBRUT  EQU "[SUBRUTINA] Subrutina auxiliar ejecutada con CALL y RET!\n"
STR_PARAMS  EQU "[PARAMETROS] Primer argumento recibido por terminal: "
STR_NOPARAM EQU "[PARAMETROS] No se pasaron argumentos adicionales con -p\n"
STR_PEDIR   EQU "\n[I/O TECLADO] Ingrese un texto o su nombre: "
STR_MOSTRAR EQU "[I/O TECLADO] El texto ingresado fue: "
STR_FIN     EQU "\n[EXIT] Fin del programa. Retornando con RET...\n"
NEWLINE     EQU "\n"

; ==========================================================
; Subrutina auxiliar llamada desde main
; ==========================================================
SUBRUTINA: MOV EDX, KS
    ADD EDX, STR_SUBRUT
    SYS 4
    RET

; ==========================================================
; Subrutina principal (Entry Point)
; ==========================================================
main: MOV EDX, KS
    ADD EDX, STR_TITULO
    SYS 4

    ; --- Test 1: Operaciones de SUB ---
    MOV EAX, 10
    SUB EAX, 3              ; EAX = 7
    SUB EAX, -4             ; EAX = 11 (inmediato negativo)
    MOV EBX, 0
    SUB EBX, 1              ; EBX = -1 (0xFFFFFFFF), CC: N=1
    MOV EDX, 100000
    SUB EDX, 40000          ; EDX = -5536

    ; --- Test 2: Subrutinas (CALL y RET) ---
    CALL SUBRUTINA

    ; --- Test 3: Lectura de Parametros (argc y argv) ---
    MOV EAX, [SP+4]         ; EAX = argc
    MOV EBX, [SP+8]         ; EBX = argv

    CMP EAX, 1
    JP HAY_PARAM

    MOV EDX, KS
    ADD EDX, STR_NOPARAM
    SYS 4
    JMP TEST_IO

HAY_PARAM: MOV EDX, KS
    ADD EDX, STR_PARAMS
    SYS 4
    MOV EDX, [EBX+4]        ; argv[1]
    SYS 4
    MOV EDX, KS
    ADD EDX, NEWLINE
    SYS 4

    ; --- Test 4: Entrada/Salida de Strings por Teclado (SYS 3 y SYS 4) ---
TEST_IO: MOV EDX, KS
    ADD EDX, STR_PEDIR
    SYS 4

    ; SYS 3: lee hasta 50 caracteres del teclado y los guarda en DS + 0
    MOV EDX, DS
    MOV ECX, 50
    SYS 3

    ; SYS 4: imprime el mensaje y luego el string leido en DS + 0
    MOV EDX, KS
    ADD EDX, STR_MOSTRAR
    SYS 4
    MOV EDX, DS
    SYS 4
    MOV EDX, KS
    ADD EDX, NEWLINE
    SYS 4

    ; --- Test 5: Fin del programa con RET ---
    MOV EDX, KS
    ADD EDX, STR_FIN
    SYS 4

    RET
