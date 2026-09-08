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
            {
                tamanioCS+= x;
            }
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
    strcpy(nombreArch, argv[1]);
    printf("Nombre archivo: %s", nombreArch);
    leer_codigo(MP, tabla_segmentos, nombreArch);
    inicializa_registros(tabla_segmentos, registros);
}