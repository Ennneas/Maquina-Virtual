#include "imagen.h"
#include <string.h>
#include <stdio.h>

//Inicializar nombre de la imagen en el main
//El tamanio va a estar en Kib??
void guardar_imagen(int tamamio_MP, char nombre[], int registros[], int tabla_segmentos[], unsigned char MP[]){
    int i, reg, seg;
    unsigned char tam[2], aux[4], version;
    FILE* arch;
    if (nombre[0] != '\0'){ //No voy a usar nombre como puntero
        arch = fopen(nombre, "wb");
        if (arch != NULL){
            fwrite("VMI26", sizeof(char), 5, arch);
            version = 1;
            fwrite(&version, sizeof(unsigned char), 1, arch); //La version de la imagen
            tam[0] = (tamamio_MP >> 8) & 0xFF;
            tam[1] = tamamio_MP & 0xFF;
            fwrite(tam, sizeof(unsigned char), 2, arch);
            for (i = 0; i < CANT_REGS; i++){
                reg = registros[i];
                aux[0] = (reg >> 24) & 0xFF;
                aux[1] = (reg >> 16) & 0xFF;
                aux[2] = (reg >> 8) & 0xFF;
                aux[3] = reg & 0xFF;
                fwrite(aux, sizeof(unsigned char), 4, arch);
            }
            for (i = 0; i < TAM_TABLA; i++){
                seg = tabla_segmentos[i];
                aux[0] = (seg >> 24) & 0xFF;
                aux[1] = (seg >> 16) & 0xFF;
                aux[2] = (seg >> 8) & 0xFF;
                aux[3] = seg & 0xFF;
                fwrite(aux, sizeof(unsigned char), 4, arch);
            }
            fwrite(MP, sizeof(unsigned char), tamamio_MP * 1024, arch);
            fclose(arch);
        }
    }
}