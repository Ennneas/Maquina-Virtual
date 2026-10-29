#ifndef funciones_h
#define funciones_h
#include "TIPOS.h"

void configurar_debug(char [], int);
void lee_memoria(int, int[], int[], unsigned char[]);
int valida_instruccion(int, Tmnemonicos[], int *);
void carga_operandos(int, int, int[], unsigned char[], int);
int Conversor_Memoria_Fisica(int[], int);
void cargar_mnemonicos(Tmnemonicos[]);
int lectura(int, int, int[], int[], unsigned char[]);
void escritura(int, int, int[], int[], unsigned char[], int);
void actualizaCC(int[], int, int);

//Funciones de strings
void string_read(int registros[], int tabla_segmentos[], unsigned char MP[]);
void string_write(int registros[], int tabla_segmentos[], unsigned char MP[]);

//Funciones de debugger
void ejecutar_breakpoint(int [], int [], unsigned char[]);
void verificar_paso_a_paso(int [], int [], unsigned char []);

//Funciones de Mnemonicos
void MOV(int, int, int[], unsigned char[], int[]);
void ADD(int, int, int[], unsigned char[], int[]);
void SUB(int, int, int[], unsigned char[], int[]);
void MUL(int, int, int[], unsigned char[], int[]);
void DIV(int, int, int[], unsigned char[], int[]);
void CMP(int, int, int[], unsigned char[], int[]);
void AND(int, int, int[], unsigned char[], int[]);
void OR(int, int, int[], unsigned char[], int[]);
void XOR(int, int, int[], unsigned char[], int[]);
void SWAP(int, int, int[], unsigned char[], int[]);
void SHL(int, int, int[], unsigned char[], int[]);
void SHR(int, int, int[], unsigned char[], int[]);
void SAR(int, int, int[], unsigned char[], int[]);
void LDL(int, int, int[], unsigned char[], int[]);
void LDH(int, int, int[], unsigned char[], int[]);
void RND(int, int, int[], unsigned char[], int[]);
void SYS(int, int, int[], unsigned char[], int[]);
void JMP(int, int, int[], unsigned char[], int[]);
void JP(int, int, int[], unsigned char[], int[]);
void JN(int, int, int[], unsigned char[], int[]);
void JZ(int, int, int[], unsigned char[], int[]);
void JC(int, int, int[], unsigned char[], int[]);
void JV(int, int, int[], unsigned char[], int[]);
void JNP(int, int, int[], unsigned char[], int[]);
void JNN(int, int, int[], unsigned char[], int[]);
void JNZ(int, int, int[], unsigned char[], int[]);
void NOT(int, int, int[], unsigned char[], int[]);
#endif