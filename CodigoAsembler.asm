\\DATA 100
\\STACK 100

main: MOV EAX, 10
    SUB EAX, 3              ; EAX = 7
    SUB EAX, -4             ; EAX = 11 (inmediato negativo)
    MOV EBX, 0
    SUB EBX, 1              ; EBX = -1 (0xFFFFFFFF), CC: N=1
    MOV EDX, 100000
    SUB EDX, 40000          ; EDX = 60000 (> 32768, antes fallaba)
    MOV ECX, SP
    SUB SP, 8               ; el indice del segmento SS no debe cambiar
    SYS 0xF                 ; breakpoint para inspeccionar
    ADD SP, 8
    STOP