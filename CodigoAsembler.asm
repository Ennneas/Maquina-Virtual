; =====================================================================
; Programa de prueba para la Máquina Virtual (MV2)
; Prueba de la subrutina SLEN (Longitud de Cadena) - Versión Corregida
; =====================================================================

; --- Declaración de Constantes de Texto (Const Segment) ---
MSJ_SALUDO EQU "Hola Mundo desde la MV2!\n"

; --- Directivas de Configuración de Segmentos ---
\\DATA 1024
\\STACK 1024

; =====================================================================
; Punto de Entrada Principal (Entry Point)
; =====================================================================

; =====================================================================
; Subrutina: SLEN
; Parámetro: [BP + 8] = Puntero de 4 bytes a la cadena
; Retorno:   ECX      = Cantidad de caracteres (longitud)
; =====================================================================
SLEN: PUSH BP
    MOV BP, SP

    MOV ECX, 0              ; Inicializar contador de longitud
    PUSH EDX                ; Preservar el registro EDX del llamador

    MOV EDX, [BP+8]         ; Leer el puntero al string pasado por la pila

loop_slen: CMP b[EDX], 0           ; ¿Llegamos al carácter nulo '\0'?
    JZ fin_slen             ; Si es '\0', salir del bucle

    ADD ECX,1               ; Incrementar el contador
    ADD EDX,1                 ; Avanzar 1 byte al siguiente carácter
    JMP loop_slen

fin_slen: POP EDX                 ; Restaurar el EDX original
    POP BP                  ; Restaurar el BP original
    RET                     ; Retornar al main (desapila IP)

main: MOV EDX, KS             ; Cargar la base del Const Segment
    ADD EDX, MSJ_SALUDO     ; Sumar el offset de la constante "MSJ_SALUDO"

    ; 1. BREAKPOINT: guarda la imagen antes de imprimir
    SYS 0xF

    ; 2. Imprimir el mensaje por pantalla antes de medirlo
    SYS 0x04                ; Imprime la cadena apuntada por EDX

    ; 3. Apilar el parámetro (puntero a la cadena) para la subrutina SLEN
    PUSH EDX
    CALL SLEN               ; Llamada a la subrutina (coincidencia exacta de etiqueta)
    ADD SP, 4               ; Limpiar el parámetro apilado (4 bytes)

    ; 4. BREAKPOINT: guarda la imagen con ECX = longitud de la cadena
    SYS 0xF

    RET                     ; Finalizar el programa limpiamente