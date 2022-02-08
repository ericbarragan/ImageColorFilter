/* Autor: Eric Barragán
 * Fecha: 2016-01-06
 * Condiciones: Este código es publicado con el fin de compartir
 * lo que a aguien más le pueda ser útil.
 * Se ofrece sin garantía alguna.
 * 
 * Aportacion:
 * - Tiene un manejo básico de imagenes en formato BMP
 *   sin requerir de más librerias que las base en GCC.
 * - Filtra rangos configurables de color en mapas RGB
 *   en base a la formula de la elipse por pares de color
 *   R con G, G con B y G con R, formando 3 filtros
 *   que se complementan.
 *   Por lo que se configura con los tres pares de
 *   cordenadas con sus respectivas distancias focales
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static float F1x=40, F1y=35, F2x=200, F2y=195, RA=228;
static float F3x=45, F3z=35, F4x=205, F4z=195, RB=229;
static float F5z=20, F5y=35, F6z=180, F6y=195, RC=228;

typedef struct {
    char *Nombre;
    unsigned char *Archivo;
    int tamanio;
    int inicio;
    int ancho;
    int alto;
    short bits;
    int imagen;
    int avance;
    int anchot;
    int extras;
    int anchob;
} BMP;

typedef struct {
    char *Nombre;
    int *Imagen;
    int ancho;
    int anchob;
    int alto;
    int imagen;
} Imagen;


BMP MesaG;
Imagen MesaC, MesaF, MesaPC;

static Imagen cargaImagen(char *Nombre){
    unsigned char *Pre_Arc;
    int *Pre_Img;
    Imagen Img;
    FILE *Archivo;
    long Tamanio;
    size_t Resultado;
    int x, y;
    int alto;
    int ancho;
    int anchob;
    int anchot;
    int inicio;

    // Abre el Archivo para Lectura Binaria
    Archivo = fopen(Nombre,"rb");
    if (Archivo == NULL){printf("\nError de apertura del Archivo: %s. \n\n", Nombre); exit(1);}

    // Optiene tamaño de Archivo
    fseek(Archivo , 0 , SEEK_END);
    Tamanio = ftell(Archivo);
    rewind(Archivo);

    // Asigna Memoria para Precarga
    Pre_Arc = (unsigned char*)malloc(sizeof(char)*Tamanio);
    if (Pre_Arc == NULL){printf("\nError de Memoria a Pre. \n\n"); exit(2);}

    // Copia el Archivo a la Memoria
    Resultado = fread(Pre_Arc, 1, Tamanio, Archivo);
    if (Resultado != Tamanio) {printf("\nError de Lectura de Archivo. \n\n"); exit(3);}
    fclose(Archivo);

    // Datos del Encabezado
    Img.Nombre = Nombre;
    Img.ancho = *(int*)&Pre_Arc[18];
    Img.alto = *(int*)&Pre_Arc[22];
    Img.imagen =  *(int*)&Pre_Arc[34];
    Img.anchob = Img.ancho * 3;
    alto = Img.alto;
    ancho = Img.ancho;
    anchot = Img.imagen/Img.alto;
    //~ extras = anchot - (Img.ancho*3);
    anchob = Img.anchob;
    inicio = *(int*)&Pre_Arc[10];

    // Asigna Memoria para Imagen
    Pre_Img = (int*)malloc(sizeof(int)*Img.imagen*3);
    if (Pre_Img == NULL){printf("\nError de Memoria a Imagen. \n\n"); exit(2);}

    // Copia Solo la Imagen
    y = alto; do { y--;
        x = ancho; do { x--;
            Pre_Img[y*anchob+x*3+0] = Pre_Arc[inicio + y*anchot + x*3 + 0];
            Pre_Img[y*anchob+x*3+1] = Pre_Arc[inicio + y*anchot + x*3 + 1];
            Pre_Img[y*anchob+x*3+2] = Pre_Arc[inicio + y*anchot + x*3 + 2];
        } while (x);
    } while (y);
    free(Pre_Arc);
    Img.Imagen = Pre_Img;

    return Img;
}


static BMP cargaModelo(char *Nombre){
    unsigned char *Pre_Arc;
    BMP bmp;
    FILE *Archivo;
    long Tamanio;
    size_t Resultado;

    // Abre el Archivo para Lectura Binaria
    Archivo = fopen(Nombre,"rb");
    if (Archivo == NULL){printf("\nError de apertura del Archivo: %s. \n\n", Nombre); exit(1);}

    // Optiene tamaño de Archivo
    fseek(Archivo , 0 , SEEK_END);
    Tamanio = ftell(Archivo);
    rewind(Archivo);

    // Asigna Memoria para Precarga
    Pre_Arc = (unsigned char*)malloc(sizeof(char)*Tamanio);
    if (Pre_Arc == NULL){printf("\nError de Memoria a Pre: %s. \n\n", Nombre); exit(2);}

    // Copia el Archivo a la Memoria
    Resultado = fread(Pre_Arc, 1, Tamanio, Archivo);
    if (Resultado != Tamanio) {printf("\nError de Copia de Archivo: %s. \n\n", Nombre); exit(3);}
    fclose(Archivo);

    // Datos del Encabezado
    bmp.Archivo = Pre_Arc;
    bmp.Nombre = Nombre;
    bmp.tamanio = *(int*)&Pre_Arc[2];
    bmp.inicio = *(int*)&Pre_Arc[10];
    bmp.ancho = *(int*)&Pre_Arc[18];
    bmp.alto = *(int*)&Pre_Arc[22];
    bmp.bits =  *(short*)&Pre_Arc[28];
    bmp.imagen =  *(int*)&Pre_Arc[34];
    bmp.avance = bmp.imagen/(bmp.alto*bmp.ancho);
    bmp.anchot = bmp.imagen/bmp.alto;
    bmp.extras = bmp.anchot - (bmp.ancho*3);
    bmp.anchob = bmp.ancho * 3;

    return bmp;
}


static Imagen creaImagen(Imagen Img_Org, char *Nombre){
    int *Pre_Img;
    Imagen Img;

    // Datos del Encabezado
    Img.Nombre = Nombre;
    Img.ancho = Img_Org.ancho;
    Img.anchob = Img_Org.anchob;
    Img.alto = Img_Org.alto;
    Img.imagen =  Img_Org.imagen;

    // Asigna Memoria para Imagen
    Pre_Img = (int*)malloc(sizeof(int)*Img.imagen*3);
    if (Pre_Img == NULL){printf("\nError de Memoria al Crear Imagen: %s. \n\n", Nombre); exit(2);}
    Img.Imagen = Pre_Img;

    return Img;
}


static void FiltraColor(Imagen Img_Org, Imagen Img_Dst){
    int x, y, X, Y, Z;
    y = Img_Org.alto; do{ y--;
    x = Img_Org.ancho; do { x--;
            X = Img_Org.Imagen[y*Img_Org.anchob+x*3+0];
            Y = Img_Org.Imagen[y*Img_Org.anchob+x*3+1];
            Z = Img_Org.Imagen[y*Img_Org.anchob+x*3+2];
            if (( sqrt((X-F1x)*(X-F1x) + (Y-F1y)*(Y-F1y)) + sqrt((X-F2x)*(X-F2x) + (Y-F2y)*(Y-F2y)) ) > RA){
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+0] = X;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+1] = Y;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+2] = Z;
            }else if (( sqrt((X-F3x)*(X-F3x) + (Z-F3z)*(Z-F3z)) + sqrt((X-F4x)*(X-F4x) + (Z-F4z)*(Z-F4z)) ) > RB){
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+0] = X;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+1] = Y;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+2] = Z;
            }else if (( sqrt((Z-F5z)*(Z-F5z) + (Y-F5y)*(Y-F5y)) + sqrt((Z-F6z)*(Z-F6z) + (Y-F6y)*(Y-F6y)) ) > RC){
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+0] = X;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+1] = Y;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+2] = Z;
            }else{
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+0] = 128;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+1] = 255;
                Img_Dst.Imagen[y*Img_Dst.anchob+x*3+2] = 64;
			}
        } while(x);
    }while(y);
}


static void PaletaColores(Imagen Img_Org, Imagen Img_Dst){
    int x, y, X, Y, Z;
    y = Img_Dst.alto; do{ y--;
        x = Img_Dst.ancho; do { x--;
            Img_Dst.Imagen[y*Img_Dst.anchob+x*3+0] = 0;
            Img_Dst.Imagen[y*Img_Dst.anchob+x*3+1] = 0;
            Img_Dst.Imagen[y*Img_Dst.anchob+x*3+2] = 0;
        } while(x);
    }while(y);
    y = Img_Org.alto; do{ y--;
        x = Img_Org.ancho; do { x--;
            X = Img_Org.Imagen[y*Img_Org.anchob+x*3+0];
            Y = Img_Org.Imagen[y*Img_Org.anchob+x*3+1];
            Z = Img_Org.Imagen[y*Img_Org.anchob+x*3+2];
            if (( sqrt((X-F1x)*(X-F1x) + (Y-F1y)*(Y-F1y)) + sqrt((X-F2x)*(X-F2x) + (Y-F2y)*(Y-F2y)) ) > RA){
                Img_Dst.Imagen[Y*Img_Dst.anchob+X*3+0] = X;
                Img_Dst.Imagen[Y*Img_Dst.anchob+X*3+1] = Y;
                Img_Dst.Imagen[Y*Img_Dst.anchob+X*3+2] = Z;
            }else{
                Img_Dst.Imagen[Y*Img_Dst.anchob+X*3+1] = 255;
            }
        } while(x);
    }while(y);
    y = Img_Org.alto; do{ y--;
        x = Img_Org.ancho; do { x--;
            X = Img_Org.Imagen[y*Img_Org.anchob+x*3+0];
            Y = Img_Org.Imagen[y*Img_Org.anchob+x*3+1];
            Z = Img_Org.Imagen[y*Img_Org.anchob+x*3+2];
            if (( sqrt((X-F3x)*(X-F3x) + (Z-F3z)*(Z-F3z)) + sqrt((X-F4x)*(X-F4x) + (Z-F4z)*(Z-F4z)) ) > RB){
                Img_Dst.Imagen[Z*Img_Dst.anchob+X*3+0+768] = X;
                Img_Dst.Imagen[Z*Img_Dst.anchob+X*3+1+768] = Y;
                Img_Dst.Imagen[Z*Img_Dst.anchob+X*3+2+768] = Z;
            }else{
                Img_Dst.Imagen[Z*Img_Dst.anchob+X*3+1+768] = 255;
            }
        } while(x);
    }while(y);
    y = Img_Org.alto; do{ y--;
        x = Img_Org.ancho; do { x--;
            X = Img_Org.Imagen[y*Img_Org.anchob+x*3+0];
            Y = Img_Org.Imagen[y*Img_Org.anchob+x*3+1];
            Z = Img_Org.Imagen[y*Img_Org.anchob+x*3+2];
            if (( sqrt((Z-F5z)*(Z-F5z) + (Y-F5y)*(Y-F5y)) + sqrt((Z-F6z)*(Z-F6z) + (Y-F6y)*(Y-F6y)) ) > RC){
                Img_Dst.Imagen[Z*Img_Dst.anchob+Y*3+0+1536] = X;
                Img_Dst.Imagen[Z*Img_Dst.anchob+Y*3+1+1536] = Y;
                Img_Dst.Imagen[Z*Img_Dst.anchob+Y*3+2+1536] = Z;
            }else{
                Img_Dst.Imagen[Z*Img_Dst.anchob+Y*3+1+1536] = 255;
            }
        } while(x);
    }while(y);
}


static void guardaBMP(BMP bmp, Imagen Img){
    FILE *Archivo;
    int x, y;
    int inicio = bmp.inicio;
    int anchot = bmp.anchot;
    int ancho = Img.ancho;
    int anchob = Img.anchob;

    // Copia Solo la Imagen
    y = Img.alto; do{ y--;
        x = ancho; do{ x--;
            bmp.Archivo[inicio+y*anchot+x*3+0] = (unsigned char)Img.Imagen[y*anchob+x*3+0];
            bmp.Archivo[inicio+y*anchot+x*3+1] = (unsigned char)Img.Imagen[y*anchob+x*3+1];
            bmp.Archivo[inicio+y*anchot+x*3+2] = (unsigned char)Img.Imagen[y*anchob+x*3+2];
        } while (x);
    }while(y);

    Archivo = fopen (Img.Nombre, "wb");
    fwrite(bmp.Archivo, sizeof(char), bmp.tamanio, Archivo);
    fclose (Archivo);
}


void insertaCadena(char *Buffer, char *Cadena, char Caracter, char *Inserto){
    int max = 128;
    char largo = strlen(Cadena);
    char Ext[] = ".bmp";
    if(largo > 96) largo = 96;

    memccpy( memccpy( memccpy( Buffer, Cadena, Caracter, largo )-1, Inserto, '\0', max)-1, Ext, '\0', max);
}


int main(int argc, char *argv[]){
    if(argc < 2) return 0;
    char *Original = argv[1];
    char *Filtrado = (char *)malloc(128);
    char *PaletaCol = (char *)malloc(128);;
    
    insertaCadena(Filtrado, Original, '.', "_F");
    insertaCadena(PaletaCol, Original, '.', "_PC");
    

    // Carga de Modelos
    MesaG = cargaModelo(Original);          // Modelo Original para Guardar
    MesaC = cargaImagen(Original);          // Imagen Original para Copiar
    MesaF = creaImagen(MesaC, Filtrado);    // Imagen Filtrada
    MesaPC = creaImagen(MesaC, PaletaCol);  // Rango de Colores Filtrado

    // Trabajo de Imagen
    FiltraColor(MesaC, MesaF);
    PaletaColores(MesaC, MesaPC);
    guardaBMP(MesaG, MesaF);
    guardaBMP(MesaG, MesaPC);

    return 0;
}
