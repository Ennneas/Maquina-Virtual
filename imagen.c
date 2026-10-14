#include "imagen.h"
#include <string.h>
#include <stdio.h>

//Inicializar nombre de la imagen en el main
//El tamanio va a estar en Kib??
void guardar_imagen(int tamamio_MP, char nombre[], int registros[], int tabla_segmentos[], unsigned char MP[]){
    int i, reg, seg;
    unsigned char tam[2], aux[4], version;
    FILE* arch;
    if (nombre[0] != '\0'){ //No voy a usar nombre como puntero asi que no valido que no sea nulo
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
        } else {
            printf("Fallo al crear la imagen");
        }
    }
}

void cargar_imagen(int *tamanio_MP, char nombre[], int registros[], int tabla_segmentos[], unsigned char MP[]){
    int i, reg, seg;
    unsigned char tam[2], aux[4], cabecera[6], version;
    FILE* arch;
    if (nombre[0] != '\0'){
        arch = fopen(nombre, "rb");
        if (arch != NULL){
            fread(cabecera, sizeof(unsigned char), 5, arch);
            cabecera[5] = '\0';
            if (strcmp(cabecera, "VMI26") == 0){
                fread(&version, sizeof(unsigned char), 1, arch);
                if (version == 1){
                    fread(tam, sizeof(unsigned char), 2, arch);
                    *tamanio_MP = (tam[0] << 8) | tam[1];
                    for (i = 0; i < CANT_REGS; i++){
                        fread(aux, sizeof(unsigned char), 4, arch);
                        reg = (aux[0] << 24) | (aux[1] << 16) | (aux[2] << 8) | aux[3];
                        registros[i] = reg;
                    }
                    for (i = 0; i < TAM_TABLA; i++){
                        fread(aux, sizeof(unsigned char), 4, arch);
                        seg = (aux[0] << 24) | (aux[1] << 16) | (aux[2] << 8) | aux[3];
                        tabla_segmentos[i] = seg;
                    }
                    fread(MP, sizeof(unsigned char), (*tamanio_MP) * 1024, arch);
                }
            }
            fclose(arch);
        } else {
            printf("Error al abrir el archivo");
        }
    }
}