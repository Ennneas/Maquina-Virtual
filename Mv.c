#include <stdio.h>
#include <stdint.h>
#include <string.h>
void inicializa_registros (char tabla_segmentos [8][4],char registros [32][4]){
    int i,j;
    while (i<8)
        while (j<4){
            
        }
    
}
void inicializar_tabla(char tabla_segmentos[8][4],char tamañoCS[]){
    int i=0,j=0;
    /*
    tabla_segmentos[0][0]=0x0;
    tabla_segmentos[0][1]=0x0;
    tabla_segmentos[0][2]=tamañoCS[0];
    tabla_segmentos[0][3]=tamañoCS[1];
    
    tabla_segmentos[1][0]=tamañoCS[0];
    tabla_segmentos[2][1]=tamañoCS[1];
    tabla_segmentos[3][2]= como calcular el tamaño del DS 16834(entero )-TamañoCS(2bytes);
    tabla_segmentos[4][3]=; comprobar si hay alguna forma de hacerlo general(osea CS!=0) */ 
    while (i<tamañoCS[0]*16+tamañoCS[1])
        while (j<tamañoCS[0]*16+tamañoCS[1])
            tabla_segmentos[i][j]=0xFF;
    
}
void leer_codigo (char MP[16834],char tabla_segmentos[8][4]){
    int i=0,version,tamañoCS[2],j=0;
    char x,identificador[6];
    FILE *Ar;
    Ar=fopen("CodigoAsembler.asm","rb");
    while (fread(&x,sizeof(char),1,Ar)==1){
        if (i<=7){
            if (i<5)
                identificador[i]=x;
            else
                if (i==5){
                    identificador [i]='\0';
                    version=x;
                }else{
                    tamañoCS[j]=x;
                    j++;
                }
                    
        }
        if (strcmp(identificador,"VMX25")!=0)
            break;
        else{
            MP[i]=x;
        }
        i++;
    }
    inicializar_tabla(tabla_segmentos,tamañoCS);
}
void main (){
    char tabla_segmentos[8][4];
    char registros [32] [4];
    char MP[16384];
    leer_codigo(MP,tabla_segmentos);
    inicializa_registros(tabla_segmentos,registros);
}