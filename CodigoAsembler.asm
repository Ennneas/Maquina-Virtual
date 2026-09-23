MOV [4], 99
MOV [3], 'a'
MOV [2], 'l'
MOV [1], 'o'
MOV [DS], 'H'
MOV EDX, DS
MOV [EDX+4],25
MOV EBX , [EDX+4]
STOP