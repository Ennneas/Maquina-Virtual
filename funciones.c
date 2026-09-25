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
    if ((resu_ope >> 32) == 1) // cambiar por lo que dijo el profe
        C = 1;
    aux = resu_ope && 0xFFFFFFFF;
    if (((resu_ope >> 32) == 1) && (resu_ope != valor)) // CAMBIAR POR LO QUE DIJO EL PROFE
        V = 1;
}
void lee_memoria(int OP, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int Direccion_Fisica, valor, i, offset, Dire_logica;
    offset = (registros[OP] & 0x00FFFF00) >> 8;
    Dire_logica = registros[registros[OP] & 0b00000000000000000000000000011111];
    Dire_logica += offset;

    registros[LAR] = Dire_logica;
    registros[MAR] = 4 << 16; // MAR parte alta

    Direccion_Fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR]);
    registros[MAR] |= Direccion_Fisica; // MAR parte baja

    registros[MBR] = 0;
    for (i = 0; i < (registros[MAR] & 0xFFFF0000) >> 16; i++)
    {
        registros[MBR] += MP[Direccion_Fisica + i] << (8 * (4 - 1 - i));
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
void carga_operandos(int t1, int t2, int registros[], unsigned char MP[], int DireccionF, long long int *instrucomp)
{
    int i, j;
    registros[OP1] = registros[OP2] = 0;
    for (i = 0; i < t2; i++)
    {
        registros[OP2] = registros[OP2] << 8;
        registros[OP2] += MP[DireccionF + i];
    }
    (*instrucomp) += registros[OP2] & 0x00FFFFFF;
    (*instrucomp) = (*instrucomp) << 16;
    registros[OP2] += (t2 << 24);
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
    int pos = ((unsigned int)registro & 0xFFFF0000) >> 16;
    if (pos < 0 || pos >= TAM_TABLA || tabla_segmentos[pos] == -1)
        return -1;

    int direccionbase = ((unsigned int)tabla_segmentos[pos] & 0xFFFF0000) >> 16;
    int limite = tabla_segmentos[pos] & 0x0000FFFF; // Tamaño del segmento
    int direccionfisica = direccionbase + offset;

    if (offset < limite)
        return direccionfisica;
    else
        return -1;
}
int lectura(int tipo, int OP, int registros[], int tabla_segmentos[], unsigned char MP[])
{
    int posl, registroleer;
    if (tipo == 0b11)
    {
        lee_memoria(OP, registros, tabla_segmentos, MP);
        return registros[MBR];
    }
    else if (tipo == 0b10)
    { // inmediato
        return registros[OP] & 0x00FFFFFF;
    }
    else
    { // registro
        registroleer = registros[OP] & 0b00000000000000000000000000011111;
        printf("\nregistros[registroleer]:%x",registros[registroleer]);
        return registros[registroleer];
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
void escritura(int tipo, int OP, int registros[], int tabla_segmentos[], unsigned char MP[], int valor_leido)
{
    int direcF, cod_registro, i, offset, base;
    if (tipo == 3)
    {
        offset = (registros[OP] & 0x00FFFF00);
        offset = offset >> 8;
        registros[LAR] = registros[registros[OP] & 0b00000000000000000000000000011111] + offset; // a la direccion logica del registro del OPERANDO le agrego el offset
        registros[MAR] = 4 << 16;                                                                // reset MAR
        direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR]);
        registros[MAR] |= direcF; // MAR parte baja
        for (i = 0; i < (registros[MAR] & 0xFFFF0000) >> 16; i++)
        {
            MP[direcF + i] = (valor_leido >> (8 * (4 - 1 - i))); // va escribiendo en memoria,big endian o little??
        }
        for (i = 0; i < (registros[MAR] & 0xFFFF0000) >> 16; i++)
        {
            printf("\nMP[%d]:%x",i,MP[direcF+i]);
        }
    }
    else
    {
        cod_registro = registros[OP] & 0b00000000000000000000000000011111;
        registros[cod_registro] = valor_leido;
    }
}
void MOV(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // MOV MODULARIZADO
    int valor_leido;
    valor_leido = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, valor_leido);
    actualiza_CC(registros, valor_leido, valor_leido);
}
void ADD(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, suma;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 + (long long int)valor2;
    suma = (int)resu_ope;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, suma);
    actualiza_CC(registros, resu_ope, suma);
    // modificaCC(resucc,registros);
}
void SUB(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    uint16_t valor1, valor2;
    int resta;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    if (valor1 > 32768) //como los inmediatos son de  16 bits  y van de -32768 a 32768, si es mayor a 32768, entonces es negativo 
        valor1=valor1-65536;
    if (valor2 > 32768) //si quiero restar 2 direcciones de memoria????
        valor2=valor2-65536;//solo si el negativo "viene" de un inmediato
    printf("\n valor2:%x",valor2);
    resta=valor1-valor2;
    resu_ope = (long long int)valor1 - (long long int)valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resta);
    actualiza_CC(registros, resu_ope, resta);
}

void MUL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, multi;
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 * (long long int)valor2;
    multi = (int)resu_ope;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, multi);
    actualiza_CC(registros, resu_ope, multi);
}

void DIV(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
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
        escritura(tipo1, OP1, registros, tabla_segmentos, MP, div);
        actualiza_CC(registros, resu_ope, div);
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
    long long int resu_ope;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resu_ope = (long long int)valor1 - (long long int)valor2;
    resta = (int)resu_ope;

    actualiza_CC(registros, resu_ope, resta);
}

void AND(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, resAnd;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resAnd = valor1 & valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resAnd);
    actualiza_CC(registros, resAnd, resAnd); // sin "resu_ope" de 64 bits, no aplica acá
}

void OR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC

    int valor1, valor2, resor;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resor = valor1 | valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resor);
    actualiza_CC(registros, resor, resor);
}

void XOR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resxor;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    resxor = valor1 ^ valor2;
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resxor);
    actualiza_CC(registros, resxor, resxor);
}

void SWAP(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int valorA, valorB, resxor;

    valorA = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    valorB = lectura(tipo2, OP2, registros, tabla_segmentos, MP);

    resxor = valorA ^ valorB; // XOR A, B  -> A
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resxor);
    valorA = resxor;

    resxor = valorB ^ valorA; // XOR B, A  -> B
    escritura(tipo2, OP2, registros, tabla_segmentos, MP, resxor);
    valorB = resxor;

    resxor = valorA ^ valorB; // XOR A, B  -> A
    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resxor);

    actualiza_CC(registros, resxor, resxor); // CC según el último XOR
}

void SHL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[]) // PREGUNTAR como es cc con los shift
{                                                                                          // afecta al registro CC
    int valor1, valor2, shleft;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    shleft = valor1 << valor2;

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, shleft);
    actualiza_CC(registros, shleft, shleft);
}

void SHR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, shright;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    shright = (int)((unsigned int)valor1 >> valor2);

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, shright);
    actualiza_CC(registros, shright, shright);
}

void SAR(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, saright;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    saright = valor1 >> valor2;

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, saright);
    actualiza_CC(registros, saright, saright);
}

void LDL(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    resultado = (valor1 & 0x0000FFFF) | (valor2 & 0x0000FFFF);

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
    actualiza_CC(registros, resultado, resultado);
}

void LDH(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor1, valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);
    valor1 = lectura(tipo1, OP1, registros, tabla_segmentos, MP);

    resultado = (valor1 & 0xFFFF0000) | ((valor2 & 0xFFFF0000) << 16);

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
    actualiza_CC(registros, resultado, resultado);
}

void RND(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{ // afecta al registro CC
    int valor2, resultado;
    valor2 = lectura(tipo2, OP2, registros, tabla_segmentos, MP);

    if (valor2 >= 0)
        resultado = rand() % (valor2 + 1); // número aleatorio entre 0 y valor2 inclusive
    else
        resultado = 0; // por las dudas, si viene un valor negativo mal formado

    escritura(tipo1, OP1, registros, tabla_segmentos, MP, resultado);
    actualiza_CC(registros, resultado, resultado);
}
// ===== Un operando =====

int traduce_y_setea_MAR(int dir_logica, int tam, int registros[], int tabla_segmentos[]) // Traduce dir_logica a fisica, seteando LAR/MAR como el resto de las instrucciones,
{                                                                                        // y devuelve la direccion fisica (o -1 si hay fallo de segmento)
    int direccion_fisica;
    registros[LAR] = dir_logica;
    registros[MAR] = tam << 16; // parte alta = cantidad de bytes
    direccion_fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[LAR]);
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
            valor = (valor << 8) | MP[direccion_fisica + j];
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
            MP[direccion_fisica + j] = (valor >> (8 * (tam - 1 - j))) & 0xFF;
    }
}

void SYS(int tipo1, int tipo2, int registros[], unsigned char MP[], int tabla_segmentos[])
{
    int llamada = lectura(tipo1, OP1, registros, tabla_segmentos, MP);
    int modo = registros[EAX];
    int cant = registros[ECX] & 0x0000FFFF;        // cantidad de celdas
    int tam = (registros[ECX] >> 16) & 0x0000FFFF; // tamaño de c/celda
    int dir_log = registros[EDX];                  // puntero inicial

    if (llamada == 1)
        sys_read(dir_log, cant, tam, modo, registros, tabla_segmentos, MP);
    else if (llamada == 2)
        sys_write(dir_log, cant, tam, modo, registros, tabla_segmentos, MP);
    else
    {
        printf("\nLlamada al sistema invalida");
        registros[IP] = -1;
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
    actualiza_CC(registros, valor, valor);
}