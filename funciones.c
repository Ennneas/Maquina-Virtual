#include "funciones.h"
int busca_registro(int cod_reg,int registros[]){
    int i=0;
    while (cod_reg!=registros[i])
        i++;
    return i; //no se valida
}
void lee_memoria (int registros[],int tabla_segmentos[],char MP[]){
    int Direccion_Fisica,valor,cant_bytes,i;
    registros[LAR] = registros[OP2] & 0x00FFFFFF; // LAR
    registros[MAR] = 4 << 16;                   // MAR parte alta

    Direccion_Fisica = Conversor_Memoria_Fisica(tabla_segmentos, registros[3]);
    registros[MAR] |= Direccion_Fisica; // MAR parte baja
    cant_bytes=registros[MAR] & 0xFFFF0000;
    for (i = 0; i < cant_bytes; i++)
    {
        valor += MP[Direccion_Fisica + i] << (8 * (4 - 1 - i));
    }
    registros[MBR] = valor; // MBR = dato leído
}   
int valida_instruccion(int opc, Tmnemonicos VMnemonicos[], int *indice_Mnmenonico)
{
    int i = 0;
    while (VMnemonicos[i].codigo != opc && i < CANT_MNE)
        i++;
    if (i < CANT_MNE)
        *indice_Mnmenonico = i;
    return i;
}
void carga_operandos(int t1, int t2, int registros[], char MP[], int DireccionF)
{
    int i, j;
    registros[OP1]=registros[OP2]=0;
    for (i = 0; i < t2; i++){
        registros[OP2]= registros[OP2] << 8;
        registros[OP2] += MP[DireccionF + i];
    }
    registros[OP2] |= t2 << 24;
    for (j = i; j < t1 + t2; j++){
        registros[OP1]= registros[OP1] << 8;
        registros[OP1] += MP[DireccionF + j];
    }
    registros[OP1]|= t1 << 24;
    
}
int Conversor_Memoria_Fisica(int tabla_segmentos[], int registro){
    
    int offset = registro & 0x0000FFFF;
    int pos = (registro & 0xFFFF0000) >> 16;
    int direccionbase = (tabla_segmentos[pos] & 0xFFFF0000) >> 16;

    int direccionfisica = direccionbase + offset;

    if(direccionfisica <= direccionbase + direccionbase + tabla_segmentos[pos] & 0x0000FFFF)
        return direccionfisica;
    else
        return -1;
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
void MOV(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[]){ //MOV MODULARIZADO
    int i, direcF, valor_leido,registroescribir,pose,posl,registroleer,cant_bytes,resu_cc;
    if (tipo1 == 3){//escribir en memoria 
        if (tipo2==3){
            lee_memoria(registros,tabla_segmentos,MP);
            valor_leido=registros[MBR];
        }else
            if (tipo2 == 2)//inmediato
                valor_leido =registros[OP2] & 0x00FFFFFF;
            else{//registro
                registroleer = registros[OP2] & 0b00000000000000000000000000011111;
                posl = busca_registro(registroleer,registros); 
                valor_leido = registros[posl];
            }           
        // --- escritura en OP1 (destino) ---
        registros[LAR] = registros[OP1] & 0x00FFFFFF; // LAR
        registros[MAR] = 4 << 16;                   //reset MAR

        direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[2]);
        registros[MAR] |= direcF; // MAR parte baja
        cant_bytes=registros[MAR] & 0xFFFF0000;
        // MBR queda igual  
        for (i = 0; i < cant_bytes; i++)
        {
            MP[direcF + i] = (valor_leido >> (8 * (4 - 1 - i)));//va escribiendo en memoria,big endian o little??
        }
    }else{//escribir en un registro
        if (tipo2==3){
            lee_memoria(registros,tabla_segmentos,MP);
            valor_leido=registros[MBR];
        }else
            if (tipo2 == 2)//inmediato
                valor_leido =registros[OP2] & 0x00FFFFFF;
            else{//registro
                registroleer = registros[OP2] & 0b00000000000000000000000000011111;
                posl = busca_registro(registroleer,registros); 
                valor_leido = registros[posl];
            } 
        registroescribir = registros[OP1] & 0b00000000000000000000000000011111;
        pose = busca_registro(registroescribir,registros);
        registros[pose] = valor_leido; 
    }
}
/*
void MOV(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[]){ //afecta al registro CC
    int i, direcF, valor_leido,registroescribir,pose,posl,registroleer,cant_bytes;
    if (tipo1 == 3 || tipo2 == 3)
    {
        if (tipo1 == 3 && tipo2 == 3)
        {   

            registros[LAR] = registros[OP2] & 0x00FFFFFF; // LAR
            registros[MAR] = 4 << 16;                   // MAR parte alta

            direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[3]);
            registros[MAR] |= direcF; // MAR parte baja
            valor_leido = 0;
            cant_bytes=registros[MAR] & 0xFFFF0000;
            for (i = 0; i < cant_bytes; i++)
            {
                valor_leido += MP[direcF + i] << (8 * (4 - 1 - i));
            }
            registros[MBR] = valor_leido; // MBR = dato leído
        }
        else{
            if(tipo1 == 3){
                if (tipo2 == 2)//inmediato
                    valor_leido =registros[OP2];
                else{//registro
                    registroleer = registros[OP2] & 0b00000000000000000000000000011111;
                    posl = busca_registro(registroleer); 
                    valor_leido = registros[posl];
                }
                registros[LAR] = registros[OP1] & 0x00FFFFFF;//LAR
                registros[MAR] = 4 << 16;//MAR ALTA
                direcF = cambiodelogafis(tabla_segmentos, registros[2]);
                registros[MAR]|= direcF;

                cant_bytes=registros[MAR] & 0xFFFF0000;

                registros[MBR] = valor_leido;//MBR
                for (i = 0; i < cant_bytes; i++)
                {
                    MP[direcF + i] = (valor_leido >> (8 * (4 - 1 - i))); // va escribiendo en memoria
                }
            }
            else{
                
                registros[LAR] = registros[3] & 0x00FFFFFF; // LAR
                registros[MAR] = 4 << 16;                   // MAR ALTA
                direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[OP2]);
                registros[MAR] |= direcF;
                valor_leido = 0;
                for (i = 0; i < 4; i++)
                {
                    valor_leido += MP[direcF + i] << (8 * (4 - 1 - i));
                }
                registros[MBR] = valor_leido;

            }
        }
    }
}
*/
void ADD(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
    int i, direcF, valor_leido,registroescribir,pose,posl,registroleer,cant_bytes,valor_base_memoria,resu_cc;
    if (tipo1 == 3){//escribir en memoria 
        if (tipo2==3){
            lee_memoria(registros,tabla_segmentos,MP);
            valor_leido=registros[MBR];
        }else
            if (tipo2 == 2)//inmediato
                valor_leido =registros[OP2] & 0x00FFFFFF;
            else{//registro
                registroleer = registros[OP2] & 0b00000000000000000000000000011111;
                posl = busca_registro(registroleer,registros); 
                valor_leido = registros[posl];
            }           
        // --- escritura en OP1 (destino) ---
        registros[LAR] = registros[OP1] & 0x00FFFFFF; // LAR
        registros[MAR] = 4 << 16;                   //reset MAR,PARTE ALTA

        direcF = Conversor_Memoria_Fisica(tabla_segmentos, registros[OP1]);
        registros[MAR] |= direcF; // MAR parte baja
        cant_bytes=registros[MAR] & 0xFFFF0000;
        // MBR queda igual
        valor_base_memoria=0;
        for (i = 0; i < cant_bytes; i++){
            valor_base_memoria += MP[direcF + i] << (8 * (4 - 1 - i));
        }
            valor_base_memoria+=valor_leido;
            resu_cc=valor_base_memoria;
        for (i = 0; i < cant_bytes; i++)
        {
            MP[direcF + i] = (valor_base_memoria >> (8 * (4 - 1 - i)));//va escribiendo en memoria,big endian o little??
        }
    }else{//escribir en un registro
        if (tipo2==3){
            lee_memoria(registros,tabla_segmentos,MP);
            valor_leido=registros[MBR];
        }else
            if (tipo2 == 2)//inmediato
                valor_leido =registros[OP2] & 0x00FFFFFF;
            else{//registro
                registroleer = registros[OP2] & 0b00000000000000000000000000011111;
                posl = busca_registro(registroleer,registros); 
                valor_leido = registros[posl];
            } 
        registroescribir = registros[OP1] & 0b00000000000000000000000000011111;
        pose = busca_registro(registroescribir,registros);
        resu_cc=registros[pose] = registros[pose]+valor_leido; 
    }
    //modificaCC(resucc,registros);
}
void SUB(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC

}
void MUL(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}
void DIV(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}
void CMP (int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void AND(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void OR(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void XOR(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void SWAP(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void SHL(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void SHR(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void SAR(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}

void LDL(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void LDH(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void RND(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

// ===== Un operando =====
void SYS(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JMP(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JP(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JN(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JZ(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JC(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JV(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JNP(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JNN(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void JNZ(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){
}

void NOT(int tipo1, int tipo2, int registros[],char MP[],int tabla_segmentos[] ){//afecta al registro CC
}