#include <stdio.h>
#include <stdint.h>
#include <string.h>
void inicializa_registros(char tabla_segmentos[8][4], char registros[32][4])
{
    int i, j;
    registros[26][0] = tabla_segmentos[0][0];
    registros[26][1] = tabla_segmentos[0][1];
    registros[27][0] = tabla_segmentos[1][0];
    registros[27][1] = tabla_segmentos[1][1];
    registros[0][0] = registros[26][0];
    registros[0][1] = registros[26][1];
    for (i = 2; i < 4; i++)
    {
        registros[26][i] = registros[27][i] = registros[0][i] = 0;
    }
}
void inicializar_tabla(char tabla_segmentos[8][4], char tamanioCS[])
{
    int i = 0, j = 0;

    while (i < 7)
    {
        while (j < 3)
        {
            tabla_segmentos[i][j] = 0xFF;
            j++;
        }
        i++;
    }

    tabla_segmentos[0][0] = 0x0;
    tabla_segmentos[0][1] = 0x0;
    tabla_segmentos[0][2] = tamanioCS[0];
    tabla_segmentos[0][3] = tamanioCS[1];
    tabla_segmentos[1][0] = tamanioCS[0];
    tabla_segmentos[1][1] = tamanioCS[1];
    tabla_segmentos[1][2] = 0x41 - tamanioCS[0] * 16;
    tabla_segmentos[1][3] = 0xC2 - tamanioCS[1] * 16;
    ; // comprobar si hay alguna forma de hacerlo general(o sea CS!=0)
}
void leer_codigo(char MP[16834], char tabla_segmentos[8][4], char nombreArch[])
{
    int i = 0, version, tamanioCS[2], j = 0;
    char x, identificador[6];
    FILE *Ar;
    Ar = fopen(nombreArch, "rb");
    while (fread(&x, sizeof(char), 1, Ar) == 1)
    {
        if (i <= 7)
        {
            if (i < 5)
                identificador[i] = x;
            else if (i == 5)
            {
                identificador[i] = '\0';
                version = x;
            }
            else
            {
                tamanioCS[j] = x;
                j++;
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
    char tabla_segmentos[8][4];
    char registros[32][4];
    char MP[16384];
    strcpy(nombreArch, argv[1]);
    printf("Nombre archivo: %s", nombreArch);
    leer_codigo(MP, tabla_segmentos, nombreArch);
    inicializa_registros(tabla_segmentos, registros);
}