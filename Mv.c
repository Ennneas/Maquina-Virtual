#include "Mv.h"
#include "funciones.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void disassembler_constantes(char MP[], int tabla_segmentos[], int registros[], int tamanioKS)
{
    if (registros[KS] != -1 && tamanioKS > 0)
    {

        int base = Conversor_Memoria_Fisica(tabla_segmentos, registros[KS], 4);
        int i = 0;

        while (i < tamanioKS)
        {
            int ini = i;
            char hex[32] = "";
            char byteStr[4];

            // avanzar hasta el '\0' (o el fin del segmento)
            while (i < tamanioKS && MP[base + i] != '\0')
                i++;
            if (i < tamanioKS)
                i++; // incluir el '\0'

            int len = i - ini;
            int mostrar = (len > 7) ? 6 : len;

            // columna hexadecimal
            for (int k = 0; k < mostrar; k++)
            {
                sprintf(byteStr, "%02X ", (unsigned char)MP[base + ini + k]);
                strcat(hex, byteStr);
            }
            if (len > 7)
                strcat(hex, "..");

            // direccion + hex
            printf("\n [%04X] %s | \"", base + ini, hex);

            // columna de texto
            for (int k = 0; k < len; k++)
            {
                unsigned char c = (unsigned char)MP[base + ini + k];
                if (c == '\0')
                    break;
                putchar(isprint(c) ? c : '.');
            }
            putchar('"');
        }
    }
}

void disassembler(int top1, int top2, int registros[], int tabla_segmentos[], char MNEM[5], char MP[], int entrypoint)
{
    int direfis, registro, inmediato, longop, mod, sector;
    char OP_A[20] = "\0", OP_B[20] = "\0", Cad_Instru[32] = "", marca = ' ', suf, pref;
    static const char *NOMBRES[32] = {
        "IP ", "OPC", "OP1", "OP2", "LAR", "MAR", "MBR", "SP ", "BP ", "???",
        "EAX", "EBX", "ECX", "EDX", "EEX", "EFX", "AC ", "CC ",
        "???", "???", "???", "???", "???", "???", "???", "???",
        "CS ", "DS ", "ES ", "SS ", "KS ", "PS "};

    direfis = Conversor_Memoria_Fisica(tabla_segmentos, registros[IP], 4);

    if (top1 == 3)
    {
        registro = registros[OP1] & 0x1F;
        mod = (registros[OP1] >> 5) & 0x7;
        inmediato = (short)((registros[OP1] >> 8) & 0xFFFF);

        if (mod == 1)
            pref = 'b';
        else if (mod == 2)
            pref = 'w';
        else
            pref = 'l';

        snprintf(OP_A, sizeof(OP_A), "%c[%s%+d]", pref, NOMBRES[registro], inmediato);
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
            registro = registros[OP1] & 0x1F;
            sector = (registros[OP1] >> 5) & 0x3;

            if (registro >= EAX && registro <= EFX && sector != 0)
            {
                char letra = 'A' + (registro - EAX); // A,B,C,D,E,F
                if (sector == 1)
                    suf = 'L';
                else if (sector == 2)
                    suf = 'H';
                else
                    suf = 'X';
                snprintf(OP_A, sizeof(OP_A), "%c%c", letra, suf);
            }
            else
                strcpy(OP_A, NOMBRES[registro]);
        }
    }
    if (top2 == 3)
    {
        registro = registros[OP2] & 0x1F;
        mod = (registros[OP2] >> 5) & 0x7;
        inmediato = (short)((registros[OP2] >> 8) & 0xFFFF);

        if (mod == 1)
            pref = 'b';
        else if (mod == 2)
            pref = 'w';
        else
            pref = 'l';

        snprintf(OP_B, sizeof(OP_B), "%c[%s%+d]", pref, NOMBRES[registro], inmediato);
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
            registro = registros[OP2] & 0x1F;
            sector = (registros[OP2] >> 5) & 0x3;

            if (registro >= EAX && registro <= EFX && sector != 0)
            {
                char letra = 'A' + (registro - EAX); // A,B,C,D,E,F
                if (sector == 1)
                    suf = 'L';
                else if (sector == 2)
                    suf = 'H';
                else
                    suf = 'X';
                snprintf(OP_B, sizeof(OP_B), "%c%c", letra, suf);
            }
            else
                strcpy(OP_B, NOMBRES[registro]);
        }
    }
    int longitud = top1 + top2 + 1;
    for (int b = 0; b < longitud; b++)
    {
        char byteStr[4];
        sprintf(byteStr, "%02X ", (unsigned char)MP[direfis + b]);
        strcat(Cad_Instru, byteStr);
    }
    if ((registros[IP] & 0xFFFF) == entrypoint)
        marca = '>';

    if (strcmp(MNEM, "STOP") != 0)
        printf("\n %c [%04X] %s  | %s  %s  %s", marca, direfis, Cad_Instru, MNEM, OP_A, OP_B);
    else
        printf("\n %c [%04X] %s  | %s", marca, direfis, Cad_Instru, MNEM);
}
void ejecuta_instruccion(char MP[], int registros[], int tabla_segmentos[], Tmnemonicos VMnemonicos[], char flag_dissasembler, int entrypoint)
{
    char primer_byte, MNEM[5];
    int top1, top2, opc, indice_Mnemonico, Memoria_fisica_IP = Conversor_Memoria_Fisica(tabla_segmentos, registros[IP], 1);
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
                disassembler(top1, top2, registros, tabla_segmentos, MNEM, MP, entrypoint);
            }
            registros[IP] += top1 + top2 + 1;
            VMnemonicos[indice_Mnemonico].ejecuta(top1, top2, registros, MP, tabla_segmentos); // ejecuta la operacion correspondiente al codigo de operacion leido
        }
        else if (opc == 0x0F)
        { // instruccion STOP,IMPLICITA
            if (flag_dissasembler == 'd')
            {
                strcpy(MNEM, "STOP");
                disassembler(top1, top2, registros, tabla_segmentos, MNEM, MP, entrypoint);
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
        // Fin de ejecucion: IP apunto fuera del Code Segment (lectura normal de fin de codigo)
        registros[IP] = -1;
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
void inicializa_pila(int registros[], int tabla_segmentos[], unsigned char MP[], int argc, int vpunteros_parametros[], int16_t entrypoint)
{
    int i = 0, offset_SP;
    offset_SP = (tabla_segmentos[registros[SS] & 0x0FFFF]);
    registros[BP] = registros[SP] = registros[SS] + offset_SP; // inicio del stack segment + el tamaño del segmento (posicionar BP Y SP al final del segmento)    }
    // subrutina principal
    // argv
    registros[OP2] = 0x0 & vpunteros_parametros[0]; // direccion logica del primer parametro
    PUSH(0, 3, registros, MP, tabla_segmentos);     // pushea argc
    // argc
    registros[OP2] = argc;
    PUSH(0, 3, registros, MP, tabla_segmentos); // pushea *argv
    // IP inicialmente vale -1 y en call se lo reserva en la pila,Cuando el usuario termina la subrutina debe poner el RET
    registros[OP2] = entrypoint;
    CALL(0, 1, registros, MP, tabla_segmentos); // llama a la subrutina principal
}
void inicializar_tabla(int tabla_segmentos[], int registros[], char *parametros[], int cantparam, int tamanioCS, int tamanioDS, int tamanioES, int tamanioSS, int tamanioKS, char flag_param, int offsetEP, int memoria_total)
{
    int tamanios[6] = {calculartamanioPS(parametros, cantparam, flag_param), tamanioKS, tamanioCS, tamanioDS, tamanioES, tamanioSS}; // PS, KS, CS, DS, ES, SS (en ese orden)
    // PS=31, KS=30, CS=26, DS=27, ES=2 8, SS=29
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
        registros[IP] = -1;
    }
}

void cargarparamenmem(unsigned char MP[MAX_MEMORIA], char *parametros[], int cantparam, char nombreArch[], char flag_param, int offsets[])
{
    int i, j, posmem = 0;

    offsets[0] = 0;
    for (i = 0; nombreArch[i] != '\0'; i++)
        MP[posmem++] = nombreArch[i];
    MP[posmem++] = 0x00; // FIX: faltaba el '\0'

    for (i = 0; i < cantparam; i++)
    {
        offsets[i + 1] = posmem;

        for (j = 0; parametros[i][j] != '\0'; j++)
            MP[posmem++] = parametros[i][j];
        MP[posmem++] = 0x00; // FIX: faltaba el '\0'
    }

    for (i = 0; i < cantparam + 1; i++)
    {
        MP[posmem++] = 0x00;
        MP[posmem++] = 0x00;
        MP[posmem++] = (offsets[i] >> 8) & 0xFF;
        MP[posmem++] = offsets[i] & 0xFF;
    }
}

void leer_codigo(int registros[], int tabla_segmentos[], unsigned char MP[MAX_MEMORIA], char nombreArch[], char flag_p, char *parametros[], int cantparam, Tmnemonicos VMnemonicos[], char flag_d, int16_t *entrypoint, int memoria_total, int offsets[])
{
    short int i = 0, j = 0, tamanioCS = 0, tamanioDS = 0, tamanioES = 0, tamanioSS = 0, tamanioKS = 0, offsetEP = 0, campos[6], baseCS, baseKS;
    unsigned char x, identificador[6], version, tam[2];
    FILE *Arch;
    Arch = fopen(nombreArch, "rb");
    if (Arch != NULL)
    {
        fread(identificador, sizeof(char), 5, Arch);
        identificador[5] = '\0';

        if (strcmp(identificador, "VMX26") == 0)
        {
            fread(&version, sizeof(char), 1, Arch);

            if (version == 1)
            {
                fread(tam, sizeof(unsigned char), 2, Arch);
                tamanioCS = (tam[0] << 8) | tam[1];

                if (tamanioCS <= MAX_MEMORIA)
                {
                    // Inicalizo la tabla de segmentos
                    tamanioDS = MAX_MEMORIA - tamanioCS;
                    tabla_segmentos[0] = 0x00000000 | tamanioCS;
                    tabla_segmentos[1] = tamanioCS << 16 | tamanioDS;
                    for (i = 2; i < 8; i++)
                    {
                        tabla_segmentos[i] = 0xFFFFFFFF;
                    }

                    // Configuro los registros utilizados
                    registros[CS] = 0x00000000;
                    registros[DS] = 0x00010000;

                    // Cargo la memoria con el codigo
                    fread(MP, sizeof(unsigned char), tamanioCS, Arch);

                    // Indico donde inicia el programa
                    registros[IP] = registros[CS];
                }
                else
                {
                    printf("Memoria insuficiente");
                    registros[IP] = -1;
                }
            }
            else if (version == 2)
            {
                // Leo todos los campos
                for (i = 0; i < 6; i++)
                {
                    fread(tam, sizeof(unsigned char), 2, Arch);
                    campos[i] = (tam[0] << 8) | tam[1];
                }

                // Asigno los valores
                tamanioCS = campos[0];
                tamanioDS = campos[1];
                tamanioES = campos[2];
                tamanioSS = campos[3];
                tamanioKS = campos[4];
                offsetEP = campos[5];
                *entrypoint = offsetEP;
                // Inicializo la tabla de segmentos
                inicializar_tabla(tabla_segmentos, registros, parametros, cantparam, tamanioCS, tamanioDS, tamanioES, tamanioSS, tamanioKS, flag_p, offsetEP, memoria_total);

                // Cargo los parametros si es que hay
                if (flag_p == 'p')
                {
                    cargarparamenmem(MP, parametros, cantparam, nombreArch, flag_p, offsets);
                }
                // Paso a memoria
                baseCS = Conversor_Memoria_Fisica(tabla_segmentos, registros[CS], 4);
                fread(&MP[baseCS], sizeof(unsigned char), tamanioCS, Arch);
                if (tamanioKS > 0)
                {
                    baseKS = Conversor_Memoria_Fisica(tabla_segmentos, registros[KS], 4);
                    fread(&MP[baseKS], sizeof(unsigned char), tamanioKS, Arch);
                }
                if (flag_d == 'd')
                    disassembler_constantes(MP, tabla_segmentos, registros, tamanioKS);

                // Inicializo la pila
                inicializa_pila(registros, tabla_segmentos, MP, cantparam, offsets, offsetEP); // subrutina principal
            }
            else
            {
                printf("La version del archivo vmx no es compatible");
                registros[IP] = -1;
            }
        }
        else
        {
            printf("El archivo vmx no tiene la cabecera correcta");
            registros[IP] = -1;
        }
        fclose(Arch);
    }
    else
    {
        printf("No se pudo leer el archivo vmx");
        registros[IP] = -1;
    }
}

int main(int argc, char *argv[])
{
    char nombre_vmx[256] = "\0", flag_d = '\0', flag_p = '\0', nombre_vmi[256] = "\0";
    int16_t entrypoint;
    int tabla_segmentos[TAM_TABLA];
    int registros[CANT_REGS], offsets[50];
    Tmnemonicos VMnemonicos[CANT_MNE];
    int cantparam = 0, i, memoria_kib = 16;
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
            strcpy(nombre_vmx, argv[i]);
        }
        else if (strstr(argv[i], ".vmi"))
        {
            strcpy(nombre_vmi, argv[i]);
        }
    }
    unsigned char MP[memoria_kib * 1024]; // rompo con la programacion estructurada
    cargar_mnemonicos(VMnemonicos);

    if (nombre_vmx[0] == '\0' && nombre_vmi[0] == '\0')
    {
        printf("No se cargo el archivo .vmx ni .vmi\n");
        return 1;
    }
    else if (nombre_vmx[0] != '\0')
    {
        leer_codigo(registros, tabla_segmentos, MP, nombre_vmx, flag_p, parametros, cantparam, VMnemonicos, flag_d, &entrypoint, memoria_kib * 1024, offsets);
    }
    else
    {
        cargar_imagen(&memoria_kib, nombre_vmi, registros, tabla_segmentos, MP);
    }

    configurar_debug(nombre_vmi, memoria_kib);

    // ciclo de lectura de MP hasta  SEGMENTATION FAULT (IP=-1)
    while (registros[IP] != -1)
    {
        ejecuta_instruccion(MP, registros, tabla_segmentos, VMnemonicos, flag_d, entrypoint);
        verificar_paso_a_paso(registros, tabla_segmentos, MP);
    }
    return 0;
}