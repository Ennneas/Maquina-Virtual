#include "Mv.h"
#include "funciones.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void disassembler(int top1, int top2, int registros[], int tabla_segmentos[], char MNEM[5], char MP[])
{
    int direfis, registro, inmediato, longop;
    char OP_A[20] = "\0", OP_B[20] = "\0", Cad_Instru[32] = "";

    direfis = Conversor_Memoria_Fisica(tabla_segmentos, registros[IP]);

    if (top1 == 3)
    {
        registro = registros[OP1] & 0b00000000000000000000000000011111;
        switch (registro)
        {
        case IP:
            strcpy(OP_A, "[IP +");
            break;
        case OPC:
            strcpy(OP_A, "[OPC+");
            break;
        case OP1:
            strcpy(OP_A, "[OP1+");
            break;
        case OP2:
            strcpy(OP_A, "[OP2+");
            break;
        case LAR:
            strcpy(OP_A, "[LAR+");
            break;
        case MAR:
            strcpy(OP_A, "[MAR+");
            break;
        case MBR:
            strcpy(OP_A, "[MBR+");
            break;
        case EAX:
            strcpy(OP_A, "[EAX+");
            break;
        case EBX:
            strcpy(OP_A, "[EBX+");
            break;
        case ECX:
            strcpy(OP_A, "[ECX+");
            break;
        case EDX:
            strcpy(OP_A, "[EDX+");
            break;
        case EEX:
            strcpy(OP_A, "[EEX+");
            break;
        case EFX:
            strcpy(OP_A, "[EFX+");
            break;
        case AC:
            strcpy(OP_A, "[AC +");
            break;
        case CC:
            strcpy(OP_A, "[CC +");
            break;
        case CS:
            strcpy(OP_A, "[CS +");
            break;
        case DS:
            strcpy(OP_A, "[DS +");
            break;
        }
        inmediato = (registros[OP1] & 0x00FFFF00) >> 8;
        snprintf(OP_A + 5, sizeof(OP_A) - 5, "%d", inmediato);

        int longop = strlen(OP_A);
        OP_A[longop] = ']';
        OP_A[longop + 1] = '\0';
    }
    else
    {
        if (top1 == 2)
        {
            inmediato = (registros[OP1] & 0x0000FFFF);
            sprintf(OP_A, "%d", inmediato); //
        }
        else if (top1 == 1)
        {
            registro = registros[OP1] & 0b00000000000000000000000000011111;
            switch (registro)
            {
            case IP:
                strcpy(OP_A, "IP ");
                break;
            case OPC:
                strcpy(OP_A, "OPC");
                break;
            case OP1:
                strcpy(OP_A, "OP1");
                break;
            case OP2:
                strcpy(OP_A, "OP2");
                break;
            case LAR:
                strcpy(OP_A, "LAR");
                break;
            case MAR:
                strcpy(OP_A, "MAR");
                break;
            case MBR:
                strcpy(OP_A, "MBR");
                break;
            case EAX:
                strcpy(OP_A, "EAX");
                break;
            case EBX:
                strcpy(OP_A, "EBX");
                break;
            case ECX:
                strcpy(OP_A, "ECX");
                break;
            case EDX:
                strcpy(OP_A, "EDX");
                break;
            case EEX:
                strcpy(OP_A, "EEX");
                break;
            case EFX:
                strcpy(OP_A, "EFX");
                break;
            case AC:
                strcpy(OP_A, "AC ");
                break;
            case CC:
                strcpy(OP_A, "CC ");
                break;
            case CS:
                strcpy(OP_A, "CS ");
                break;
            case DS:
                strcpy(OP_A, "DS ");
                break;
            }
        }
    }
    if (top2 == 3)
    {
        registro = registros[OP2] & 0b00000000000000000000000000011111;
        switch (registro)
        {
        case IP:
            strcpy(OP_B, "[IP +");
            break;
        case OPC:
            strcpy(OP_B, "[OPC+");
            break;
        case OP1:
            strcpy(OP_B, "[OP1+");
            break;
        case OP2:
            strcpy(OP_B, "[OP2+");
            break;
        case LAR:
            strcpy(OP_B, "[LAR+");
            break;
        case MAR:
            strcpy(OP_B, "[MAR+");
            break;
        case MBR:
            strcpy(OP_B, "[MBR+");
            break;
        case EAX:
            strcpy(OP_B, "[EAX+");
            break;
        case EBX:
            strcpy(OP_B, "[EBX+");
            break;
        case ECX:
            strcpy(OP_B, "[ECX+");
            break;
        case EDX:
            strcpy(OP_B, "[EDX+");
            break;
        case EEX:
            strcpy(OP_B, "[EEX+");
            break;
        case EFX:
            strcpy(OP_B, "[EFX+");
            break;
        case AC:
            strcpy(OP_B, "[AC +");
            break;
        case CC:
            strcpy(OP_B, "[CC +");
            break;
        case CS:
            strcpy(OP_B, "[CS +");
            break;
        case DS:
            strcpy(OP_B, "[DS +");
            break;
        }
        inmediato = (registros[OP2] & 0x00FFFF00) >> 8;
        snprintf(OP_B + 5, sizeof(OP_B) - 5, "%d", inmediato);

        int longop = strlen(OP_B);
        OP_B[longop] = ']';
        OP_B[longop + 1] = '\0';
    }
    else
    {
        if (top2 == 2)
        {
            inmediato = (registros[OP2] & 0x0000FFFF);
            sprintf(OP_B, "%d", inmediato);
        }
        else if (top2 == 1)
        {
            registro = registros[OP2] & 0b00000000000000000000000000011111;
            switch (registro)
            {
            case IP:
                strcpy(OP_B, "IP ");
                break;
            case OPC:
                strcpy(OP_B, "OPC");
                break;
            case OP1:
                strcpy(OP_B, "OP1");
                break;
            case OP2:
                strcpy(OP_B, "OP2");
                break;
            case LAR:
                strcpy(OP_B, "LAR");
                break;
            case MAR:
                strcpy(OP_B, "MAR");
                break;
            case MBR:
                strcpy(OP_B, "MBR");
                break;
            case EAX:
                strcpy(OP_B, "EAX");
                break;
            case EBX:
                strcpy(OP_B, "EBX");
                break;
            case ECX:
                strcpy(OP_B, "ECX");
                break;
            case EDX:
                strcpy(OP_B, "EDX");
                break;
            case EEX:
                strcpy(OP_B, "EEX");
                break;
            case EFX:
                strcpy(OP_B, "EFX");
                break;
            case AC:
                strcpy(OP_B, "AC ");
                break;
            case CC:
                strcpy(OP_B, "CC ");
                break;
            case CS:
                strcpy(OP_B, "CS ");
                break;
            case DS:
                strcpy(OP_B, "DS ");
                break;
            }
        }
    }
    int longitud = top1 + top2 + 1;
    for (int b = 0; b < longitud; b++)
    {
        char byteStr[4];
        sprintf(byteStr, "%02X ", (unsigned char)MP[direfis + b]);
        strcat(Cad_Instru, byteStr);
    }
    if (strcmp(MNEM, "STOP") != 0)
        printf("\n[%04X] %s  | %s  %s  %s", direfis, Cad_Instru, MNEM, OP_A, OP_B);
    else
        printf("\n[%04X] %s  | %s", direfis, Cad_Instru, MNEM);
}
void ejecuta_instruccion(char MP[], int registros[], int tabla_segmentos[], Tmnemonicos VMnemonicos[], char flag_dissasembler)
{
    char primer_byte, MNEM[5];
    int top1, top2, opc, indice_Mnemonico, Memoria_fisica_IP = Conversor_Memoria_Fisica(tabla_segmentos, registros[IP]);
    if (Memoria_fisica_IP != -1)
    {
        primer_byte = MP[Memoria_fisica_IP];

        top2 = (primer_byte & 0b11000000) >> 6; // tipo de operando B
        top1 = (primer_byte & 0b00110000) >> 4; // tipo de operando A
        opc = (primer_byte & 0b00011111);       // Codigo de Operacion
        if (valida_instruccion(opc, VMnemonicos, &indice_Mnemonico))
        { // rompo con la programacion estructurada ajkajaj
            carga_operandos(top1, top2, registros, MP, registros[IP] + 1);
            if (flag_dissasembler == 'd')
            {
                strcpy(MNEM, VMnemonicos[indice_Mnemonico].mnemonico);
                disassembler(top1, top2, registros, tabla_segmentos, MNEM, MP);
            }
            registros[IP] += top1 + top2 + 1;
            VMnemonicos[indice_Mnemonico].ejecuta(top1, top2, registros, MP, tabla_segmentos); // ejecuta la operacion correspondiente al codigo de operacion leido
        }
        else if (opc == 0x0F)
        { // instruccion STOP,IMPLICITA
            if (flag_dissasembler == 'd')
            {
                strcpy(MNEM, "STOP");
                disassembler(top1, top2, registros, tabla_segmentos, MNEM, MP);
            }
            registros[IP] = -1;
        }
        else
        { // INSTRUCCION INVALIDA
            registros[IP] = -1;
        }
    }
    else
    {
        registros[IP] = -1; // en el caso que no haya un stop
        printf("\n SEGMENTATION FAULT");
    }
}
int calculartamanioPS(char *parametros[], int cantparam, char flag_param)
{
    int tam = 0, i;
    if (flag_param == 'p')
    {
        for (i = 0; i < cantparam; i++)
            tam += strlen(parametros[i]) + 1; // string + '\0'
        tam += 4 * cantparam;                 // un puntero de 4 bytes por string
    }
    return tam;
}

void inicializar_tabla(int tabla_segmentos[], int registros[], char *parametros[], int cantparam, int tamanioCS, int tamanioDS, int tamanioES, int tamanioSS, int tamanioKS, char flag_param,int offsetEP)
{
    int tamanios[6] = {calculartamanioPS(parametros, n, flag_param), tamanioKS, tamanioCS, tamanioDS, tamanioES, tamanioSS}; // PS, KS, CS, DS, ES, SS (en ese orden)
    // PS=31, KS=30, CS=26, DS=27, ES=28, SS=29
    int reg_seg[6] = {31, 30, 26, 27, 28, 29};
    int i;

    int base = 0, n = 0;
    for (i = 0; i < 6; i++)
    {
        if (tamanios[i] > 0)
        {
            tabla_segmentos[n] = (base << 16) | tamanios[i];
            registros[reg_seg[i]] = n << 16;
            base += tamanios[i];
            n++;
        }
        else
        {
            registros[reg_seg[i]] = 0xFFFFFFFF;
        }
    }

    if (base > memoria_total)
    {
        printf("Memoria insuficiente");
    }else{
        registros[IP]=registros[CS]+offsetEP;
        registros[BP]=registros[SP];
    }
}
void leer_codigo(char MP[MAX_MEMORIA], int tabla_segmentos[], char nombreArch[], char *parametros[], int cantparam, char flag_param)
{
    int i = 0, version, tamanioCS = 0, j = 0, k = 0, tamanioDS = 0, tamanioES = 0, tamanioSS = 0, tamanioKS = 0, offsetEP = 0;
    char x, identificador[6];
    FILE *Ar;
    Ar = fopen(nombreArch, "rb");
    if (Ar != NULL)
        while (fread(&x, sizeof(char), 1, Ar) == 1)
        {
            if (i <= 17)
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
                else if (i == 7)
                {
                    tamanioCS = tamanioCS << 8;
                    tamanioCS += x;
                }
                else if (i == 8)
                    tamanioDS = x;
                else if (i == 9)
                {
                    tamanioDS = tamanioDS << 8;
                    tamanioDS += x;
                }
                else if (i == 10)
                    tamanioES = x;
                else if (i == 11)
                {
                    tamanioES = tamanioSS << 8;
                    tamanioES += x;
                }
                else if (i == 12)
                    tamanioSS = x;
                else if (i == 13)
                {
                    tamanioSS = tamanioSS << 8;
                    tamanioSS += x;
                }
                else if (i == 14)
                    tamanioKS = x;
                else if (i == 15)
                {
                    tamanioKS = tamanioKS << 8;
                    tamanioKS += x;
                }
                else if (i == 16)
                    offsetEP = x;
                else
                {
                    offsetEP = offsetEP << 8;
                    offsetEP += x;
                }
            }
            else if (strcmp(identificador, "VMX26") == 0 && version == 2) // agrego version es != 1 por las dudas
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
    if (i >= 17)
    {
        inicializar_tabla(tabla_segmentos, registros, parametros, cantparam, tamanioCS, tamanioDS, tamanioES, tamanioSS, tamanioKS, flag_param,offsetEP);
    }
    fclose(Ar);
}

int main(int argc, char *argv[])
{
    char nombreArch[256] = "\0", flag_dis = '\0', flag_param = '\0', nombre_vmi[256] = "\0";
    int tabla_segmentos[TAM_TABLA];
    int registros[CANT_REGS];
    unsigned char MP[MAX_MEMORIA];
    Tmnemonicos VMnemonicos[CANT_MNE];
    int caantparam = 0, i, memoria_kib = 16;
    char *parametros[30];

    // Insertar funcion configurar debug despues de que se lean los argumentos del programa

    for (i = 1; i < argc; i++)
    {
        if (flag_p)
        {
            parametros[cantparam++] = argv[i]; // después de -p, todo es parámetro
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            flag_p = 'p';
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            flag_d = 'd';
        }
        else if (strncmp(argv[i], "m=", 2) == 0)
        {
            memoria_kib = atoi(argv[i] + 2); // atoi convierte texto en entero
        }
        else if (strstr(argv[i], ".vmx"))
        {
            nombreArch = argv[i];
        }
        else if (strstr(argv[i], ".vmi"))
        {
            nombre_vmi = argv[i];
        }
    }

    if (nombre_vmx == NULL && nombre_vmi == NULL)
    {
        printf("Falta el archivo .vmx y/o .vmi\n");
        return 1;
    }

    leer_codigo(MP, tabla_segmentos, nombreArch);
    inicializa_registros(tabla_segmentos, registros, 0, 1);
    cargar_mnemonicos(VMnemonicos);

    // ciclo de lectura de MP hasta  SEGMENTATION FAULT (IP=-1)
    while (registros[IP] != -1 && Conversor_Memoria_Fisica(tabla_segmentos, registros[IP]) != -1)
    {
        ejecuta_instruccion(MP, registros, tabla_segmentos, VMnemonicos, flag_dis);
        verificar_paso_a_paso(registros, tabla_segmentos, MP);
    }

    return 0;
}