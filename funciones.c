#include "funciones.h"
void actualiza_CC(int registros[], long long int resu_ope, int valor)
{
    int Z = 0, N = 0, C = 0, aux, V = 0;
    if ((resu_ope >> 31) & 1)
    {
        N = 1;
    }
    if ((resu_ope | 0x00) == 0)
    {
        Z = 1;
    }
    if ((resu_ope >> 32) == 1)
        C = 1;
    aux = resu_ope && 0xFFFFFFFF;
    if (((resu_ope >> 32) == 1) && (resu_ope != valor)) // PREGUNTAR BIEN COMO ES EL DESBORDAMIENTO Y COMO IMPLEMENTAR LA SOLUCION
        V = 1;
}
int busca_registro(int cod_reg, int registros[])
{ // esta mal, no se usa
    int i = 0;
    while (cod_reg != registros[i] && i < 32)
        i++;
    return i; // no se valida
}
void lee_memoria(int OP, int registros[], int tabla_segmentos[], char MP[])
{
    int Direccion_Fisica, valor, cant_bytes, i, offset, base;
    offset = registros[OP] & 0x00FFFF00;
    base = registros[registros[OP1] & 0b00000000000000000000000000011111]; // no entendi
    base += offset;
    registros[LAR] = base;    // LAR
    registros[MAR] = 4 << 16; // MAR parte alta

    Direccion_Fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR]);
    registros[MAR] |= Direccion_Fisica; // MAR parte baja
    cant_bytes = registros[MAR] & 0xFFFF0000;
    registros[MBR] = 0;
    for (i = 0; i < 4; i++)
    {
        registros[MBR] += MP[Direccion_Fisica + i] << (8 * (4 - 1 - i));
    }
    printf("REGISTROS MBR:%d", registros[MBR]);
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
void carga_operandos(int t1, int t2, int registros[], char MP[], int DireccionF)
{
    int i, j;
    registros[OP1] = registros[OP2] = 0;
    for (i = 0; i < t2; i++)
    {
        registros[OP2] = registros[OP2] << 8;
        registros[OP2] += MP[DireccionF + i];
    }
    registros[OP2] |= t2 << 24;
    for (j = i; j < t1 + t2; j++)
    {
        registros[OP1] = registros[OP1] << 8;
        registros[OP1] += MP[DireccionF + j];
    }
    registros[OP1] |= t1 << 24;
}
int Conversor_Memoria_Fisica(int tabla_segmentos[], int registro)
{

    int offset = registro & 0x0000FFFF;
    int pos = (registro & 0xFFFF0000) >> 16;
    int direccionbase = (tabla_segmentos[pos] & 0xFFFF0000) >> 16;

    int direccionfisica = direccionbase + offset;
    // if (direccionfisica >= direccionbase && direccionfisica <= tabla_segmentos[pos] & 0x0000FFFF)// creo que este es correcto
    if (direccionfisica <= direccionbase + direccionbase + tabla_segmentos[pos] & 0x0000FFFF) // ??
        return direccionfisica;
    else
        return -1;
}
int lectura(int tipo, int OP, int registros[], int tabla_segmentos[], char MP[])
{
    int posl, registroleer;
    if (tipo == 3)
    {
        lee_memoria(OP, registros, tabla_segmentos, MP);
        return registros[MBR];
    }
    else if (tipo == 2) // inmediato
        return registros[OP] & 0x00FFFFFF;
    else
    { // registro
        registroleer = registros[OP] & 0b00000000000000000000000000011111;
        posl = busca_registro(registroleer, registros);
        return registros[posl];
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
}
// ===== Dos operandos =====
void escritura(int tipo, int registros[], int tabla_segmentos[], char MP[], int valor_leido)
{
    int direcF, cant_bytes, cod_registro, i, offset, base;
    if (tipo == 3)
    {
        offset = registros[OP1] & 0x00FFFF00;
        registros[LAR] = registros[registros[OP1] & 0b00000000000000000000000000011111] + offset; // a la direccion logica del registro del OPERANDO le agrego el offset
        registros[MAR] = 4 << 16;                                                                 // reset MAR

        direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR]);
        registros[MAR] |= direcF; // MAR parte baja
        cant_bytes = registros[MAR] & 0xFFFF0000;
        // MBR queda igual
        for (i = 0; i < 4; i++)
        {
            MP[direcF + i] = (registros[MBR] >> (8 * (4 - 1 - i))); // va escribiendo en memoria,big endian o little??
        }
    }
    else
    {
        cod_registro = registros[OP1] & 0b00000000000000000000000000011111;
        // pose = busca_registro(cod_registro,registros); CAMBIAR POR VALIDAION??? QUIZAS, POR SI TE METEN UN EGX
        registros[cod_registro] = valor_leido;
    }
}
void MOV(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // MOV MODULARIZADO
    int valor_leido;
    long int resu_ope; // no veo que se use

    valor_leido = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    escritura(tipo1, registros, tabla_segmentos, MP, valor_leido);
    // actualiza_CC(registros,valor_leido,valor_leido);
}
void ADD(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, suma;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 + (long long int)valor2;
    suma = (int)resu_ope;
    escritura(tipo1, registros, tabla_segmentos, MP, suma);
    actualiza_CC(registros, resu_ope, suma);
    // modificaCC(resucc,registros);
}
void SUB(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resta;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 - (long long int)valor2;
    resta = (int)resu_ope;
    escritura(tipo1, registros, tabla_segmentos, MP, resta);
    actualiza_CC(registros, resu_ope, resta);
}

void MUL(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, multi;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 * (long long int)valor2;
    multi = (int)resu_ope;
    escritura(tipo1, registros, tabla_segmentos, MP, multi);
    actualiza_CC(registros, resu_ope, multi);
}

void DIV(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, div, resto;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    if (valor2 != 0)
    {
        resu_ope = (long long int)valor1 / (long long int)valor2;
        resto = valor1 % valor2;
        registros[AC] = resto;
        div = (int)resu_ope;
        escritura(tipo1, registros, tabla_segmentos, MP, div);
        actualiza_CC(registros, resu_ope, div);
    }
    else
    {
        printf("error division  0");
        registros[IP] = -1;
    }
}
void CMP(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resta;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 - (long long int)valor2;
    resta = (int)resu_ope;

    actualiza_CC(registros, resu_ope, resta);
}

void AND(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, resAnd;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resAnd = valor1 & valor2;
    escritura(tipo1, registros, tabla_segmentos, MP, resAnd);
    actualiza_CC(registros, resAnd, resAnd); // sin "resu_ope" de 64 bits, no aplica acá
}

void OR(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, resor;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resor = valor1 | valor2;
    escritura(tipo1, registros, tabla_segmentos, MP, resor);
    actualiza_CC(registros, resor, resor);
}

void XOR(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resxor;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resxor = valor1 ^ valor2;
    escritura(tipo1, registros, tabla_segmentos, MP, rexsor);
    actualiza_CC(registros, resxor, resxor);
}

void SWAP(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
}

void SHL(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
}

void SHR(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
}

void SAR(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
}

void LDL(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void LDH(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void RND(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

// ===== Un operando =====
void SYS(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JMP(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JP(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JN(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JZ(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JC(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JV(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JNP(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JNN(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void JNZ(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{
}

void NOT(int tipo1, int tipo2, int registros[], char MP[], int tabla_segmentos[])
{ // afecta al registro CC
}