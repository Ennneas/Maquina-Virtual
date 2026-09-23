#include "Mv.h"
#include "funciones.h"
void ejecuta_instruccion(char MP[], int registros[], int tabla_segmentos[], Tmnemonicos VMnemonicos[])
{
    char primer_byte;
    int top1, top2, opc, indice_Mnemonico, Memoria_fisica_IP = Conversor_Memoria_Fisica(tabla_segmentos, registros[IP]);
    if (Memoria_fisica_IP != -1)
    {
        primer_byte = MP[Memoria_fisica_IP];
        top2 = (primer_byte & 0b11000000)>>6; // tipo de operando B
        top1 = (primer_byte & 0b00110000)>>4; // tipo de operando A
        opc = (primer_byte & 0b00011111);  // Codigo de Operacion
        if (valida_instruccion(opc, VMnemonicos, &indice_Mnemonico)){ // rompo con la programacion estructurada ajkajaj
            carga_operandos(top1, top2, registros, MP, registros[IP] + 1);
            registros[IP] += top1 + top2 + 1;
            VMnemonicos[indice_Mnemonico].ejecuta(top1, top2, registros, MP, tabla_segmentos); // ejecuta la operacion correspondiente al codigo de operacion leido
        }
        else if (opc == 0x0F)
        { // instruccion STOP
            registros[IP] = -1;
        }
        else
        { // INSTRUCCION INVALIDA
            printf("\n instruccion :%x invalida", opc);
            registros[IP] = -1;
        }
    }
    else
    {
        registros[IP] = -1; // en el caso que no haya un stop
        printf("\n SEGMENTATION FAULT");
    }
}
void inicializa_registros(int tabla_segmentos[], int registros[], int poscodes, int posdatas)
{
    int i, j;
    registros[CS] = poscodes << 16;
    registros[DS] = posdatas << 16; 
    registros[IP] = poscodes << 16 ;
}
void inicializar_tabla(int tabla_segmentos[], int tamanioCS)
{
    int i = 2, ds = 1, cs = 0;
    while (i < 7)
    {
        tabla_segmentos[i] = -1;
        i++;
    }
    printf("TAMANOIO:%x",tamanioCS);
    tabla_segmentos[cs] = tamanioCS;                                     // siempre que el cs este en la pos 0 de la tabla
    tabla_segmentos[ds] = tamanioCS << 16 | ((MAX_MEMORIA - tamanioCS)); // Asigno los 2 bytes mas significativos del tamaño a la base del DS y en los 2 menos significativos su tamaño
}
void leer_codigo(char MP[MAX_MEMORIA], int tabla_segmentos[], char nombreArch[])
{
    int i = 0, version, tamanioCS = 0, j = 0, k = 0;
    char x, identificador[6];
    FILE *Ar;
    Ar = fopen(nombreArch, "rb");
    if (Ar != NULL)
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
                else if (i == 6)
                    tamanioCS = x;
                else
                {
                    tamanioCS = tamanioCS << 8;
                    tamanioCS += x;
                }
            }
            else if (strcmp(identificador, "VMX26") == 0 && version == 1) // agrego version es != 1 por las dudas
            {
                MP[k] = x;
                k++;
            }
            else
                break;
            i++;
        }
    else
        printf("\n Archivo inexsistente ");
    if (i >= 8)
    {
        inicializar_tabla(tabla_segmentos, tamanioCS);
    }
    fclose(Ar);
}
int main(int argc, char *argv[])
{
    char nombreArch[256];
    int tabla_segmentos[TAM_TABLA];
    int registros[CANT_REGS];
    unsigned char MP[MAX_MEMORIA];
    Tmnemonicos VMnemonicos[CANT_MNE];
    strcpy(nombreArch, argv[1]);
    leer_codigo(MP, tabla_segmentos, nombreArch);
    inicializa_registros(tabla_segmentos, registros,0,1);
    cargar_mnemonicos(VMnemonicos);
    // ciclo de lectura de MP hasta  SEGMENTATION FAULT (IP=-1)
    while (registros[IP] != -1)
        ejecuta_instruccion(MP, registros, tabla_segmentos, VMnemonicos);
    return 0;
}