#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define IP 0
#define OPC 1
#define OP1 2
#define OP2 3
#define LAR 4
#define MAR 5
#define MBR 6
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC 16
#define CC 17
#define CS 26
#define DS 27
#define MAX_MEMORIA 16384
typedef struct 
{   
    char mnemonico[5];
    int codigo;
}Operacion;
void ejecuta_instruccion(char MP[],int registros[],int tabla_segmentos[],Operacion VMnemonicos[]){
    char primer_byte;
    int top1,top2,opc;
    if (registros[IP] <= registros[DS] + (tabla_segmentos[0] & 0x00FF)) {//suponiendo que el CS siempre esta en el 0 de la tabla de segmentos 
        primer_byte=MP[registros[IP]];
        top1=(primer_byte & 0b11000000);
        top2=(primer_byte & 0b00110000);
        opc=(primer_byte & 0b00011111);
        if (!valida_operacion(opc,VMnemonicos))
            printf("Operacion Invalida");//ERROR INSTRUCCION INVALIDA
        else{
            cargar_operandos(op1,op2,MP);
        }
    }else//SEGMENTATION FAULT
}
void inicializa_registros(int tabla_segmentos[], int registros[])
{
    int i, j;
    registros[CS]=tabla_segmentos[0] & 0xFF00;
    registros[DS]=tabla_segmentos[1] & 0xFF00;
    registros[IP]=tabla_segmentos[0] & 0xFF00;
}
void inicializar_tabla(int tabla_segmentos[], int tamanioCS)
{
    int i = 2;
    while (i < 7)
    {
        tabla_segmentos[i] = -1;
        i++;
    }
    tabla_segmentos[0]=tamanioCS;
    tabla_segmentos[1]=tamanioCS<<16 & (MAX_MEMORIA - tamanioCS); //Asigno los 2 bytes mas significativos del tamaño a la base del DS y en los 2 menos significativos su tamaño
}
void leer_codigo(char MP[MAX_MEMORIA], int tabla_segmentos[], char nombreArch[])
{
    int i = 0, version, tamanioCS=0, j = 0;
    char x, identificador[6];
    FILE *Ar;
    Ar = fopen(nombreArch, "rb");
    while (fread(&x, sizeof(char), 1, Ar) == 1)
    {
        if (i <= 7)
        {
            if (i < 5)
                identificador[i] = x;
            else 
                if (i == 5)
            {
                identificador[i] = '\0';
                version = x;
            }
            else
                tamanioCS+= x;

        }
        if (strcmp(identificador, "VMX25") != 0)
            break;
        else
        {
            MP[i] = x;
        }
        i++;
    }
    inicializar_tabla(tabla_segmentos, tamanioCS);
}
void main(char argc, char *argv[])
{
    char nombreArch[256];
    int tabla_segmentos[8];
    int registros[32];
    char MP[MAX_MEMORIA];
    Operacion VMnemonicos[28];
    strcpy(nombreArch, argv[1]);
    leer_codigo(MP,tabla_segmentos,nombreArch);
    
}