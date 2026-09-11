#ifndef funciones_h
#define funciones_h
#include "TIPOS.h"
void cargar_mnemonicos(Tmnemonicos []);
void MOV(int , int , int [], char [],int []);
void ADD(int , int , int [], char [],int []);
void SUB(int , int , int [], char [],int []);
void MUL(int , int , int [], char [],int []);
void DIV(int , int , int [], char [],int []);
void CMP(int , int , int [], char [],int []);
void AND(int , int , int [], char [],int []);
void OR(int , int , int [], char [],int []);
void XOR(int , int , int [], char [],int []);
void SWAP(int , int , int [], char [],int []);
void SHL(int , int , int [], char [],int []);
void SHR(int , int , int [], char [],int []);
void SAR(int , int , int [], char [],int []);
void LDL(int , int , int [], char [],int []);
void LDH(int , int , int [], char [],int []);
void RND(int , int , int [], char [],int []);
void SYS(int , int , int [], char [],int []);
void JMP(int , int , int [], char [],int []);
void JP(int , int , int [], char [],int []);
void JN(int , int , int [], char [],int []);
void JZ(int , int , int [], char [],int []);
void JC(int , int , int [], char [],int []);
void JV(int , int , int [], char [],int []);
void JNP(int , int , int [], char [],int []);
void JNN(int , int , int [], char [],int []);
void JNZ(int , int , int [], char [],int []);
void NOT(int , int , int [], char [],int []);
void STOP(int , int , int [], char [],int []);
#endif