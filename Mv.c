#include "Mv.h"
#include "funciones.h"
int valida_instruccion (int opc,Tmnemonicos VMnemonicos[],int *indice_Mnmenonico){
    int i=0;
    while (VMnemonicos[i].codigo!=opc && i < CANT_MNE)
        i++;
    if (i < CANT_MNE)
        *indice_Mnmenonico=i;
    return (i < CANT_MNE);
}
void carga_operandos (int t1, int t2,int registros[],char MP[],int DireccionF){
        int i,j;
        for (i=0;i<t2;i++)
            registros[OP2]+=MP[DireccionF+i] << 8*(t2-1-i); //Big-endian los bytes mas significativos se cargan primero
        for (j=i;j<t1+t2;j++)//obra de tomy 
            registros[OP1]+=MP[DireccionF+j] << 8*(t2-1-i);
}  
void ejecuta_instruccion(char MP[],int registros[],int tabla_segmentos[],Tmnemonicos VMnemonicos[]){
    char primer_byte;
    int top1,top2,opc,indice_Mnemonico;
    if (registros[IP] <= registros[DS] + (tabla_segmentos[0] & 0x00FF)) {//suponiendo que el CS siempre esta en el 0 de la tabla de segmentos 
        primer_byte=MP[registros[IP]];
        top2=(primer_byte & 0b11000000);//tipo de operando B
        top1=(primer_byte & 0b00110000);//tipo de operando A
        opc=(primer_byte & 0b00011111);//Codigo de Operacion
        if (valida_instruccion(opc,VMnemonicos,&indice_Mnemonico)){ //rompo con la programacion estructurada ajkajaj
            carga_operandos(top1,top2,registros,MP,registros[IP]+1);
            //copiar la instruccion completa para el disassembler
            registros[IP]+=top1+top2+1;
            VMnemonicos[indice_Mnemonico].ejecuta(top1,top2,registros,MP,tabla_segmentos); //ejecuta la operacion correspondiente al codigo de operacion leido 
        }else
            if (opc==0x0F){//instruccion STOP
                registros[IP]=-1;
            }else //INSTRUCCION INVALIDA
                printf("instruccion :%x invalida",opc);
    }else{
         registros[IP]=-1; //en el caso que no haya un stop
         printf("\n SEGMENTATION FAULT");
         //SEGMENTATION FAULT
        }
}
void inicializa_registros(int tabla_segmentos[], int registros[])
{
    int i, j;
    registros[CS]=(tabla_segmentos[0])>>16;
    registros[DS]=(tabla_segmentos[1])>>16;
    registros[IP]=(tabla_segmentos[0])>>16;
    registros[OP1]=registros[OP2]=0;
}
void inicializar_tabla(int tabla_segmentos[], int tamanioCS)
{
    int i = 2;
    while (i < 7)
    {
        tabla_segmentos[i] = -1;
        i++;
    }
    tabla_segmentos[0]=tamanioCS; //siempre que el cs este en la pos 0 de la tabla
    tabla_segmentos[1]=tamanioCS<<16 | ((MAX_MEMORIA - tamanioCS)); //Asigno los 2 bytes mas significativos del tamaño a la base del DS y en los 2 menos significativos su tamaño
    printf("\n TABLA0:%x  TABLA1:%x",tabla_segmentos[0],tabla_segmentos[1]);
}
void leer_codigo(char MP[MAX_MEMORIA], int tabla_segmentos[], char nombreArch[])
{
    int i = 0, version, tamanioCS=0, j = 0;
    char x, identificador[6];
    FILE *Ar;
    Ar = fopen(nombreArch, "rb");
    if (Ar!=NULL)
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
                    if (i==6)
                        tamanioCS=x;
                    else{
                        tamanioCS= tamanioCS<<8;
                        tamanioCS+=x;
                    }
                        
            }else
                if (strcmp(identificador, "VMX26") != 0)
                    break;
                else
                    MP[i] = x;
            i++;
        }
    else
        printf("\n Archivo inexsistente ");
    if (i>=8){
        inicializar_tabla(tabla_segmentos, tamanioCS);
    }
    fclose(Ar);
}
int main(char argc, char *argv[])
{
    char nombreArch[256];
    int tabla_segmentos[TAM_TABLA];
    int registros[CANT_REGS];
    char MP[MAX_MEMORIA];
    Tmnemonicos VMnemonicos[CANT_MNE];
    strcpy(nombreArch, argv[1]);
    leer_codigo(MP,tabla_segmentos,nombreArch);
    inicializa_registros(tabla_segmentos,registros);    
    //ciclo de lectura de MP hasta  SEGMENTATION FAULT (IP=-1)???
    while (registros[IP]!=-1)
        ejecuta_instruccion(MP,registros,tabla_segmentos,VMnemonicos);
    return 0;
}