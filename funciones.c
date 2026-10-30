#include "funciones.h"
#include "imagen.h"
#include <string.h>

static char nombre_vmi[256] = "";
static int tamanio_MP = 16;
static int paso_a_paso = 0;

void configurar_debug(char nombre[], int tamanio)
{
    if (nombre[0] != '\0')
        strcpy(nombre_vmi, nombre);
    else
        nombre_vmi[0] = '\0';
    tamanio_MP = tamanio;
}

void actualiza_CC(int registros[], long long int resu_s, unsigned long long int resu_u, int valor)
{

    int Z = 0, N = 0, C = 0, V = 0;
    if ((valor >> 31) & 1)
        N = 1;
    if (valor == 0)
        Z = 1;
    if ((resu_u >> 32) != 0)
        C = 1;
    if (resu_s != valor)
        V = 1;
    registros[CC] = (N << 31) | (Z << 30) | (C << 29) | (V << 28);
}
void lee_memoria(int OP, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int Direccion_Fisica, valor, i, offset, Dire_logica, cant;
    offset = (registros[OP] & 0x00FFFF00) >> 8;
    Dire_logica = registros[registros[OP] & 0b00000000000000000000000000011111];
    Dire_logica += offset;

    registros[LAR] = Dire_logica;

    cant = (registros[OP] >> 5) & 0x7;
    registros[MAR] = cant << 16; // MAR parte alta
    if (cant != 1 && cant != 2)
        cant = 4;
    Direccion_Fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR], cant);
    if (Direccion_Fisica != -1)
    {
        registros[MAR] |= Direccion_Fisica; // MAR parte baja

        registros[MBR] = 0;
        for (i = 0; i < cant; i++)
        {
            registros[MBR] += MP[Direccion_Fisica + i] << (8 * (cant - 1 - i));
        }
    }
    else
    {
        registros[IP] = -1; // en el caso que no haya un stop
        printf("\n SEGMENTATION FAULT");
    }
}
int valida_instruccion(int opc, Tmnemonicos VMnemonicos[], int *indice_Mnmenonico)
{
    int i = 0;
    while (VMnemonicos[i].codigo != opc && i < CANT_MNE)
        i++;
    if (i < CANT_MNE)
        *indice_Mnmenonico = i;
    return (i < CANT_MNE);
}
void carga_operandos(int t1, int t2, int registros[], unsigned char MP[], int DireccionF)
{
    int i, j;
    registros[OP1] = registros[OP2] = 0;
    for (i = 0; i < t2; i++)
    {
        registros[OP2] = registros[OP2] << 8;
        registros[OP2] += MP[DireccionF + i];
    }

    registros[OP2] |= (t2 << 24);
    for (j = i; j < t1 + t2; j++)
    {
        registros[OP1] = registros[OP1] << 8;
        registros[OP1] += MP[DireccionF + j];
    }
    registros[OP1] |= t1 << 24;
}
int Conversor_Memoria_Fisica(int tabla_segmentos[], int registro, int cant)
{

    int offset = registro & 0x0000FFFF;
    int pos = ((unsigned int)registro & 0xFFFF0000) >> 16;
    if (pos < 0 || pos >= TAM_TABLA || tabla_segmentos[pos] == -1)
        return -1;
    int direccionbase = ((unsigned int)tabla_segmentos[pos] & 0xFFFF0000) >> 16;
    int limite = tabla_segmentos[pos] & 0x0000FFFF; // Tamaño del segmento
    int direccionfisica = direccionbase + offset;

    if (offset + cant < limite)
        return direccionfisica;
    else
        return -1;
}
int lectura(int tipo, int OP, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int posl, registroleer, sector, cant, valor;
    if (tipo == 0b11)
    {
        cant = (registros[OP] >> 5) & 0x7;
        if (cant != 1 && cant != 2)
            cant = 4;
        lee_memoria(OP, registros, tabla_segmentos, MP);

        if (cant == 1)
            return (int)(signed char)(registros[MBR] & 0xFF); // byte: extiendo signo
        else if (cant == 2)
            return (int)(short int)(registros[MBR] & 0xFFFF); // word: extiendo signo
        else
            return registros[MBR];
    }
    else if (tipo == 0b10)
    { // inmediato
        return (int)(short int)(registros[OP] & 0x0000FFFF);
    }
    else
    { // registro
        registroleer = registros[OP] & 0b00000000000000000000000000011111;
        sector = (registros[OP] >> 5) & 0x3;
        valor = registros[registroleer];

        if (sector == 0)
            return valor;
        else if (sector == 1)
            return (int)(signed char)(valor & 0xFF);
        else if (sector == 2)
            return (int)(signed char)((valor >> 8) & 0xFF);
        else
            return (int)(short int)(valor & 0xFFFF);
    }
}
void cargar_mnemonicos(Tmnemonicos vectormnemonico[])
{
    // Instrucciones de dos operandos
    strcpy(vectormnemonico[0].mnemonico, "MOV");
    vectormnemonico[0].codigo = 0x10;
    vectormnemonico[0].ejecuta = MOV;

    strcpy(vectormnemonico[1].mnemonico, "ADD");
    vectormnemonico[1].codigo = 0x11;
    vectormnemonico[1].ejecuta = ADD;

    strcpy(vectormnemonico[2].mnemonico, "SUB");
    vectormnemonico[2].codigo = 0x12;
    vectormnemonico[2].ejecuta = SUB;

    strcpy(vectormnemonico[3].mnemonico, "MUL");
    vectormnemonico[3].codigo = 0x13;
    vectormnemonico[3].ejecuta = MUL;

    strcpy(vectormnemonico[4].mnemonico, "DIV");
    vectormnemonico[4].codigo = 0x14;
    vectormnemonico[4].ejecuta = DIV;

    strcpy(vectormnemonico[5].mnemonico, "CMP");
    vectormnemonico[5].codigo = 0x15;
    vectormnemonico[5].ejecuta = CMP;

    strcpy(vectormnemonico[6].mnemonico, "AND");
    vectormnemonico[6].codigo = 0x16;
    vectormnemonico[6].ejecuta = AND;

    strcpy(vectormnemonico[7].mnemonico, "OR");
    vectormnemonico[7].codigo = 0x17;
    vectormnemonico[7].ejecuta = OR;

    strcpy(vectormnemonico[8].mnemonico, "XOR");
    vectormnemonico[8].codigo = 0x18;
    vectormnemonico[8].ejecuta = XOR;

    strcpy(vectormnemonico[9].mnemonico, "SWAP");
    vectormnemonico[9].codigo = 0x19;
    vectormnemonico[9].ejecuta = SWAP;

    strcpy(vectormnemonico[10].mnemonico, "SHL");
    vectormnemonico[10].codigo = 0x1A;
    vectormnemonico[10].ejecuta = SHL;

    strcpy(vectormnemonico[11].mnemonico, "SHR");
    vectormnemonico[11].codigo = 0x1B;
    vectormnemonico[11].ejecuta = SHR;

    strcpy(vectormnemonico[12].mnemonico, "SAR");
    vectormnemonico[12].codigo = 0x1C;
    vectormnemonico[12].ejecuta = SAR;

    strcpy(vectormnemonico[13].mnemonico, "LDL");
    vectormnemonico[13].codigo = 0x1D;
    vectormnemonico[13].ejecuta = LDL;

    strcpy(vectormnemonico[14].mnemonico, "LDH");
    vectormnemonico[14].codigo = 0x1E;
    vectormnemonico[14].ejecuta = LDH;

    strcpy(vectormnemonico[15].mnemonico, "RND");
    vectormnemonico[15].codigo = 0x1F;
    vectormnemonico[15].ejecuta = RND;

    // Instrucciones de un operando
    strcpy(vectormnemonico[16].mnemonico, "SYS");
    vectormnemonico[16].codigo = 0x00;
    vectormnemonico[16].ejecuta = SYS;

    strcpy(vectormnemonico[17].mnemonico, "JMP");
    vectormnemonico[17].codigo = 0x01;
    vectormnemonico[17].ejecuta = JMP;

    strcpy(vectormnemonico[18].mnemonico, "JP");
    vectormnemonico[18].codigo = 0x02;
    vectormnemonico[18].ejecuta = JP;

    strcpy(vectormnemonico[19].mnemonico, "JN");
    vectormnemonico[19].codigo = 0x03;
    vectormnemonico[19].ejecuta = JN;

    strcpy(vectormnemonico[20].mnemonico, "JZ");
    vectormnemonico[20].codigo = 0x04;
    vectormnemonico[20].ejecuta = JZ;

    strcpy(vectormnemonico[21].mnemonico, "JC");
    vectormnemonico[21].codigo = 0x05;
    vectormnemonico[21].ejecuta = JC;

    strcpy(vectormnemonico[22].mnemonico, "JV");
    vectormnemonico[22].codigo = 0x06;
    vectormnemonico[22].ejecuta = JV;

    strcpy(vectormnemonico[23].mnemonico, "JNP");
    vectormnemonico[23].codigo = 0x07;
    vectormnemonico[23].ejecuta = JNP;

    strcpy(vectormnemonico[24].mnemonico, "JNN");
    vectormnemonico[24].codigo = 0x08;
    vectormnemonico[24].ejecuta = JNN;

    strcpy(vectormnemonico[25].mnemonico, "JNZ");
    vectormnemonico[25].codigo = 0x09;
    vectormnemonico[25].ejecuta = JNZ;

    strcpy(vectormnemonico[26].mnemonico, "NOT");
    vectormnemonico[26].codigo = 0x0A;
    vectormnemonico[26].ejecuta = NOT;

    strcpy(vectormnemonico[27].mnemonico, "PUSH");
    vectormnemonico[27].codigo = 0x0B;
    vectormnemonico[27].ejecuta = PUSH;

    strcpy(vectormnemonico[28].mnemonico, "POP");
    vectormnemonico[28].codigo = 0x0C;
    vectormnemonico[28].ejecuta = POP;

    strcpy(vectormnemonico[29].mnemonico, "CALL");
    vectormnemonico[29].codigo = 0x0D;
    vectormnemonico[29].ejecuta = CALL;

    strcpy(vectormnemonico[30].mnemonico, "RET");
    vectormnemonico[30].codigo = 0x0E;
    vectormnemonico[30].ejecuta = RET;
}
// ===== Dos operandos =====
void escritura(int tipo, int OP, int registros[], int tabla_segmentos[], unsigned char MP[], int valor_leido)
{
    int direcF, cod_registro, i, offset, base, cant, sector;
    if (tipo == 3)
    {
        offset = (registros[OP] & 0x00FFFF00);

        cant = (registros[OP] >> 5) & 0x7; // estos tres bits te dicen la cantidad de bytes
        if (cant != 1 && cant != 2)
            cant = 4;

        offset = offset >> 8;
        registros[LAR] = registros[registros[OP] & 0x01F] + offset; // a la direccion logica del registro del OPERANDO le agrego el offset
        registros[MAR] = cant << 16;                                // reset MAR
        direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR],4);
        if (direcF != -1)
        {
            registros[MAR] |= direcF; // MAR parte baja
            for (i = 0; i < (registros[MAR] & 0xFFFF0000) >> 16; i++)
            {
                MP[direcF + i] = (valor_leido >> (8 * (4 - 1 - i))); // va escribiendo en memoria,big endian o little??
            }
        }
        else
        {
            registros[IP] = -1; // en el caso que no haya un stop
            printf("\n SEGMENTATION FAULT");
        }
    }
    else
    {
        cod_registro = registros[OP] & 0x1F;
        sector = (registros[OP] >> 5) & 0x3;

        if (sector == 0)
            registros[cod_registro] = valor_leido;
        else if (sector == 1)
            registros[cod_registro] = (registros[cod_registro] & 0xFFFFFF00) | (valor_leido & 0xFF);
        else if (sector == 2)
            registros[cod_registro] = (registros[cod_registro] & 0xFFFF00FF) | ((valor_leido & 0xFF) << 8);
        else
            registros[cod_registro] = (registros[cod_registro] & 0xFFFF0000) | (valor_leido & 0xFFFF);
    }
}
void MOV(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // MOV MODULARIZADO
    int valor_leido;
    valor_leido = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, valor_leido);
    actualiza_CC(registros, (long long int)valor_leido, (unsigned long long int)(unsigned int)valor_leido, valor_leido);
}
void ADD(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, suma;
    long long int resu_s;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_s = (long long int)valor1 + (long long int)valor2;
    suma = (int)resu_s;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, suma);

    resu_u = (unsigned long long int)(unsigned int)valor1 + (unsigned long long int)(unsigned int)valor2;
    actualiza_CC(registros, resu_s, resu_u, suma);
    // modificaCC(resucc,registros);
}
void SUB(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    uint16_t valor1, valor2;
    int resta;
    long long int resu_s;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    if (valor1 > 32768) // como los inmediatos son de  16 bits  y van de -32768 a 32768, si es mayor a 32768, entonces es negativo
        valor1 = valor1 - 65536;
    if (valor2 > 32768)          // si quiero restar 2 direcciones de memoria????
        valor2 = valor2 - 65536; // solo si el negativo "viene" de un inmediato

    resu_s = (long long int)valor1 - (long long int)valor2;
    resta = (int)resu_s;

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resta);

    resu_u = (unsigned long long int)(unsigned int)valor1 - (unsigned long long int)(unsigned int)valor2;

    actualiza_CC(registros, resu_s, resu_u, resta);
}
void MUL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, multi;
    long long int resu_s;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_s = (long long int)valor1 * (long long int)valor2;
    multi = (int)resu_s;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, multi);

    resu_u = (unsigned long long int)(unsigned int)valor1 * (unsigned long long int)(unsigned int)valor2;
    actualiza_CC(registros, resu_s, resu_u, multi);
}

void DIV(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, div, resto;
    long long int resu_s;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    if (valor2 != 0)
    {
        resu_s = (long long int)valor1 / (long long int)valor2;
        resto = valor1 % valor2;
        registros[AC] = resto;
        div = (int)resu_s;
        escritura(tipo1, OP1, registros, tabla_segmentos, MP, div);
        // DIV nunca agranda el numero por lo que no puede haber carry
        actualiza_CC(registros, resu_s, (unsigned long long int)(unsigned int)div, div);
    }
    else
    {
        printf("error division  0");
        registros[IP] = -1;
    }
}
void CMP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resta;
    long long int resu_s;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_s = (long long int)valor1 - (long long int)valor2;
    resta = (int)resu_s;

    resu_u = (unsigned long long int)(unsigned int)valor1 - (unsigned long long int)(unsigned int)valor2;
    actualiza_CC(registros, resu_s, resu_u, resta);
}

void AND(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, res_and;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    res_and = valor1 & valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, res_and);
    // AND no puede tener overflow ni carry
    actualiza_CC(registros, (long long int)res_and, (unsigned long long int)(unsigned int)res_and, res_and);
}

void OR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, res_or;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    res_or = valor1 | valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, res_or);

    // OR no puede tener overflow ni carry
    actualiza_CC(registros, (long long int)res_or, (unsigned long long int)(unsigned int)res_or, res_or);
}

void XOR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, res_xor;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    res_xor = valor1 ^ valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, res_xor);

    // XOR no puede tener overflow ni carry
    actualiza_CC(registros, (long long int)res_xor, (unsigned long long int)(unsigned int)res_xor, res_xor);
}

void SWAP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int valorA, valorB, res_xor;

    valorA = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    valorB = lectura(tipo2, OP2, registros, tabla_segmentos, MP);

    res_xor = valorA ^ valorB; // XOR A, B  -> A
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, res_xor);
    valorA = res_xor;

    res_xor = valorB ^ valorA; // XOR B, A  -> B
    escritura(tipo2, OP2, registros, tabla_segmentos, MP, res_xor);
    valorB = res_xor;

    res_xor = valorA ^ valorB; // XOR A, B  -> A
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, res_xor);

    // XOR no puede tener overflow ni carry
    // CC según el último XOR
    actualiza_CC(registros, (long long int)res_xor, (unsigned long long int)(unsigned int)res_xor, res_xor);
}

void SHL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[]) // PREGUNTAR como es cc con los shift
{                                                                                          // afecta al registro CC
    int valor1, valor2, shleft;
    long long int resu_s;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    if (valor2 < 0)
        valor2 = 0;

    resu_s = (long long int)valor1 << valor2;
    shleft = (int)resu_s;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, shleft);

    resu_u = (unsigned long long int)(unsigned int)valor1 << valor2;
    actualiza_CC(registros, resu_s, resu_u, shleft);
}

void SHR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, shright;
    unsigned long long int resu_u;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    if (valor2 < 0)
        valor2 = 0;

    resu_u = (unsigned long long int)(unsigned int)valor1 >> valor2;
    shright = (int)resu_u;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, shright);

    actualiza_CC(registros, (long long int)shright, resu_u, shright);
}

void SAR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, saright;
    long long int resu_s;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    if (valor2 < 0)
        valor2 = 0;

    resu_s = (long long int)valor1 >> valor2;
    saright = (int)resu_s;

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, saright);
    actualiza_CC(registros, resu_s, (unsigned long long int)(unsigned int)saright, saright);
}

void LDL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int valor1, valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resultado = (valor1 & 0xFFFF0000) | (valor2 & 0x0000FFFF);
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
}

void LDH(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int valor1, valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resultado = (valor2 << 16) | (valor1 & 0x0000FFFF);
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
}

void RND(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);

    if (valor2 >= 0)
        resultado = rand() % (valor2 + 1); // número aleatorio entre 0 y valor2 inclusive
    else
        resultado = 0; // por las dudas, si viene un valor negativo mal formado

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
}
// ===== Un operando =====

int traduce_y_setea_MAR(int dir_logica, int tam, int registros[], int tabla_segmentos[]) // Traduce dir_logica a fisica, seteando LAR/MAR como el resto de las instrucciones,
{                                                                                        // y devuelve la direccion fisica (o -1 si hay fallo de segmento)
    int direccion_fisica;
    registros[LAR] = dir_logica;
    registros[MAR] = tam << 16; // parte alta = cantidad de bytes
    direccion_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR], tam);
    if (direccion_fisica != -1)
        registros[MAR] |= direccion_fisica; // parte baja
    return direccion_fisica;
}

int mascara(int tam)
{
    if (tam == 1)
        return 0xFF;
    if (tam == 2)
        return 0xFFFF;
    if (tam == 3)
        return 0xFFFFFF;
    return 0xFFFFFFFF;
}

void escribe_binario(int valor, int tam)
{
    int i;
    printf("0b");
    for (i = tam * 8 - 1; i >= 0; i--)
        printf("%d", (valor >> i) & 1);
    printf(" ");
}

void escribe_caracteres(int valor, int tam)
{
    int i;
    unsigned char c;
    for (i = tam - 1; i >= 0; i--)
    {
        c = (valor >> (8 * i)) & 0xFF;
        printf("%c", (c >= 32 && c < 127) ? c : '.');
    }
    printf(" ");
}

void sys_write(int dir_log, int cant, int tam, int modo, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int i, j, direccion_fisica, valor;

    for (i = 0; i < cant; i++)
    {
        direccion_fisica = traduce_y_setea_MAR(dir_log + i * tam, tam, registros, tabla_segmentos);
        if (direccion_fisica == -1)
        {
            printf("\nError: fallo de segmento");
            registros[IP] = -1;
            return;
        }
        valor = 0;
        for (j = 0; j < tam; j++)
            valor = (valor << 8) | (unsigned char)MP[direccion_fisica + j];
        registros[MBR] = valor;
        printf("\n[%04X]: ", direccion_fisica);
        if (modo & 0x10)
            escribe_binario(valor, tam);
        if (modo & 0x08)
            printf("0x%0*X ", tam * 2, valor & mascara(tam));
        if (modo & 0x04)
            printf("0o%o ", valor & mascara(tam));
        if (modo & 0x02)
            escribe_caracteres(valor, tam);
        if (modo & 0x01)
            printf("%d ", valor);
    }
}

void sys_read(int dir_log, int cant, int tam, int modo, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int i, j, direccion_fisica, valor;
    for (i = 0; i < cant; i++)
    {
        direccion_fisica = traduce_y_setea_MAR(dir_log + i * tam, tam, registros, tabla_segmentos);
        printf("\ndireccion_fisica:%d", direccion_fisica);
        if (direccion_fisica == -1)
        {
            printf("\nError: fallo de segmento");
            registros[IP] = -1;
            return;
        }

        printf("\n[%04X]: ", direccion_fisica);
        if (modo & 0x10)
            scanf("%i", &valor);
        else if (modo & 0x08)
            scanf("%x", &valor);
        else if (modo & 0x04)
            scanf("%o", &valor);
        else if (modo & 0x02)
        {
            char c;
            scanf(" %c", &c);
            valor = c;
        }
        else
            scanf("%d", &valor);

        registros[MBR] = valor;
        for (j = 0; j < tam; j++)
        {
            MP[direccion_fisica + j] = (valor >> (8 * (tam - 1 - j))) & 0xFF;
        }
    }
}

void ejecutar_breakpoint(int registros[], int tabla_segmentos[], unsigned char MP[])
{
    char indicador_debug[10]; // Sobra espacio por las dudas
    if (nombre_vmi[0] != '\0')
    {
        guardar_imagen(tamanio_MP, nombre_vmi, registros, tabla_segmentos, MP);
        printf("\n[DEBUG] Enter: paso a paso | g: continuar | q: salir > ");
        do
        {
            // Uso fgets en lugar de scanf para leer el enter y para mas seguridad
            fgets(indicador_debug, sizeof(indicador_debug), stdin);
            if (indicador_debug[0] == '\n')
                paso_a_paso = 1;
            else if (indicador_debug[0] == 'g')
                paso_a_paso = 0;
            else if (indicador_debug[0] == 'q')
                registros[IP] = -1;
            else
                printf("Comando invalido (Enter: paso a paso | g: continuar | q: salir) > ");
        } while (indicador_debug[0] != '\n' && indicador_debug[0] != 'g' && indicador_debug[0] != 'q');
    }
}

void verificar_paso_a_paso(int registros[], int tabla_segmentos[], unsigned char MP[])
{
    if (paso_a_paso == 1 && registros[IP] != -1)
        ejecutar_breakpoint(registros, tabla_segmentos, MP);
}

void string_read(int registros[], int tabla_segmentos[], unsigned char MP[])
{
    char c;
    int i = 0, max_tam, dir, dir_fisica;
    max_tam = registros[ECX];
    dir = registros[EDX];
    c = getchar(); // Leo caracter a caracter para no tener un tamaño fijo
    while (registros[IP] != -1 && c != '\n' && (max_tam == -1 || i < max_tam))
    {
        dir_fisica = traduce_y_setea_MAR(dir + i, 1, registros, tabla_segmentos);
        if (dir_fisica != -1)
        {
            registros[MBR] = c;
            MP[dir_fisica] = registros[MBR];
            i++;
            c = getchar();
        }
        else
        {
            printf("Error al escribir cadena: Segmentation Fault \n");
            registros[IP] = -1;
        }
    }
    if (registros[IP] != -1)
    {
        while (c != '\n')
        {
            c = getchar(); // Limpio el buffer
        }
        dir_fisica = traduce_y_setea_MAR(dir + i, 1, registros, tabla_segmentos);
        if (dir_fisica != -1)
        {
            registros[MBR] = '\0';
            MP[dir_fisica] = registros[MBR];
        }
        else
        {
            printf("Error al escribir cadena: Segmentation Fault \n");
            registros[IP] = -1;
        }
    }
}

void string_write(int registros[], int tabla_segmentos[], unsigned char MP[])
{
    char c;
    int i = 0, dir = registros[EDX], dir_fisica;
    dir_fisica = traduce_y_setea_MAR(dir, 1, registros, tabla_segmentos);
    if (dir_fisica != -1)
    {
        registros[MBR] = MP[dir_fisica];
        c = registros[MBR];
    }
    else
    {
        printf("Error al acceder a cadena: Segmentation Fault \n");
        registros[IP] = -1;
    }
    while (registros[IP] != -1 && c != '\0')
    {
        putchar(c);
        i++;
        dir_fisica = traduce_y_setea_MAR(dir + i, 1, registros, tabla_segmentos);
        if (dir_fisica != -1)
        {
            registros[MBR] = MP[dir_fisica];
            c = registros[MBR];
        }
        else
        {
            printf("Error al acceder a cadena: Segmentation Fault \n");
            registros[IP] = -1;
        }
    }
}

void SYS(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int llamada = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    int modo = registros[EAX];
    int cant = registros[ECX] & 0x0000FFFF; // cantidad de celdas
    int tam = (registros[ECX] >> 16);       // tamaño de c/celda
    int dir_log = registros[EDX];           // puntero inicial
    if (llamada == 0)
    {                           // Limpiar pantalla
        printf("\033[H\033[J"); // Caracter ESC en otcal (27), seguido de [ para iniciar comandos, H mueve el cursor al inicio y J Borra todo lo que este despues del cursor
    }
    else if (llamada == 1)
    {
        sys_read(dir_log, cant, tam, modo, registros, tabla_segmentos, MP);
    }
    else if (llamada == 2)
    {
        sys_write(dir_log, cant, tam, modo, registros, tabla_segmentos, MP);
    }
    else if (llamada == 3)
    {
        string_read(registros, tabla_segmentos, MP);
    }
    else if (llamada == 4)
    {
        string_write(registros, tabla_segmentos, MP);
    }
    else if (llamada == 15)
    {
        if (nombre_vmi[0] != '\0')
            paso_a_paso = 1; // Simula que el usuario puso paso a paso para que el proximo breakpoint se active siempre y ahi realmente decida el usuario
    }
    else
    {
        printf("\nLlamada al sistema invalida");
        // registros[IP] = -1;
    }
}
void JMP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[]) // Lee el operando 2 al ser una funcion de un solo operando
{
    unsigned int direccion_salto;
    direccion_salto = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    registros[IP] = (registros[CS] & 0xFFFF0000) | (direccion_salto & 0x0000FFFF);
}

void JP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, N, Z;
    bits_control = registros[CC] >> 28;
    N = ((bits_control) & 0b1000) >> 3;
    Z = ((bits_control) & 0b0100) >> 2;
    if (!N && !Z)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JN(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, N;
    bits_control = registros[CC] >> 28;
    N = ((bits_control) & 0b1000) >> 3;
    if (N)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JZ(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, Z;
    bits_control = registros[CC] >> 28;
    Z = ((bits_control) & 0b0100) >> 2;
    if (Z)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JC(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, C;
    bits_control = registros[CC] >> 28;
    C = ((bits_control) & 0b0010) >> 1;
    if (C)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JV(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, V;
    bits_control = registros[CC] >> 28;
    V = ((bits_control) & 0b0001);
    if (V)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JNP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, N, Z;
    bits_control = registros[CC] >> 28;
    N = ((bits_control) & 0b1000) >> 3;
    Z = ((bits_control) & 0b0100) >> 2;
    if (N || Z)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JNN(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, N;
    bits_control = registros[CC] >> 28;
    N = ((bits_control) & 0b1000) >> 3;
    if (!N)
    {
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
    }
}

void JNZ(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    unsigned int bits_control, Z;
    bits_control = registros[CC] >> 28;
    Z = ((bits_control) & 0b0100) >> 2;
    if (!Z)
        JMP(tipo1, tipo2, registros, MP, tabla_segmentos);
}
void NOT(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor;
    valor = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor = ~valor;
    escritura(tipo2, OP2, registros, tabla_segmentos, MP, valor);
    // NOT no puede tener overflow ni carry
    actualiza_CC(registros, (long long int)valor, (unsigned long long int)(unsigned int)valor, valor);
}
void PUSH(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int32_t valor = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    int offset_sp = registros[SP] & 0x0000FFFF;
    if (offset_sp < 4) // Stack Overflow (llegó al fondo del segmento SS)
    {
        printf("\nSTACK OVERFLOW");
        registros[IP] = -1;
        return;
    }
    registros[SP] -= 4; // Decrementa antes de escribir
    registros[LAR] = registros[SP];
    registros[MAR] = 4 << 16;
    int dir_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR],4);
    if (dir_fisica != -1)
    {
        registros[MAR] |= dir_fisica;
        registros[MBR] = valor;
        for (int i = 0; i < 4; i++)
            MP[dir_fisica + i] = (valor >> (8 * (3 - i))) & 0xFF; // Big Endian
    }
    else
    {
        printf("\nSEGMENTATION FAULT EN PUSH");
        registros[IP] = -1;
    }
}
void POP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int pos_ss = ((unsigned int)registros[SS] & 0xFFFF0000) >> 16;
    int limite_ss = tabla_segmentos[pos_ss] & 0x0000FFFF;
    int offset_sp = registros[SP] & 0x0000FFFF;
    if (offset_sp + 4 > limite_ss) // Stack Underflow (pila vacía)
    {
        printf("\nSTACK UNDERFLOW");
        registros[IP] = -1;
        return;
    }
    registros[LAR] = registros[SP];
    registros[MAR] = 4 << 16;
    int dir_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR],4);
    if (dir_fisica != -1)
    {
        registros[MAR] |= dir_fisica;
        int32_t valor = 0;
        for (int i = 0; i < 4; i++)
            valor = (valor << 8) | (unsigned char)MP[dir_fisica + i];
        registros[MBR] = valor;
        registros[SP] += 4; // Incrementa luego de leer
        escritura(tipo2, OP2, registros, tabla_segmentos, MP, valor);
    }
    else
    {
        printf("\nSEGMENTATION FAULT EN POP");
        registros[IP] = -1;
    }
}
void CALL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int32_t offset_subrutina = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    // 1. Apilar dirección de retorno (IP actual ya apunta a la prox instrucción)
    int offset_sp = registros[SP] & 0x0000FFFF;
    if (offset_sp < 4)
    {
        printf("\nSTACK OVERFLOW EN CALL");
        registros[IP] = -1;
        return;
    }
    registros[SP] -= 4;
    int dir_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[SP],4);
    if (dir_fisica != -1)
    {
        int32_t dir_retorno = registros[IP];
        for (int i = 0; i < 4; i++)
            MP[dir_fisica + i] = (dir_retorno >> (8 * (3 - i))) & 0xFF;
        // 2. Saltar a la subrutina
        registros[IP] = (registros[CS] & 0xFFFF0000) | (offset_subrutina & 0x0000FFFF);
    }
    else
    {
        printf("\nSEGMENTATION FAULT EN CALL");
        registros[IP] = -1;
    }
}
void RET(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int pos_ss = ((unsigned int)registros[SS] & 0xFFFF0000) >> 16;
    int limite_ss = tabla_segmentos[pos_ss] & 0x0000FFFF;
    int offset_sp = registros[SP] & 0x0000FFFF;
    if (offset_sp + 4 > limite_ss)
    {
        printf("\nSTACK UNDERFLOW EN RET");
        registros[IP] = -1;
        return;
    }
    registros[LAR] = registros[SP];
    int dir_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR],4);
    if (dir_fisica != -1)
    {
        int32_t dir_retorno = 0;
        for (int i = 0; i < 4; i++)
            dir_retorno = (dir_retorno << 8) | (unsigned char)MP[dir_fisica + i];
        registros[SP] += 4;
        // Solo actualizamos IP con la dirección recuperada. Ejecutaintrcuccionse encarga de lo demas
        registros[IP] = dir_retorno;
    }
    else
    {
        printf("\nSEGMENTATION FAULT EN RET");
        registros[IP] = -1;
    }
}