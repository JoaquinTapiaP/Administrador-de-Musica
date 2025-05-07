//#include "extra.h"
#include "map.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[100];
    char *artists[300];
    char album_name[100];   
    char track_name[100];
    float tempo;
    List *track_genre;
} Song;
  


List* split_string(char* str, char delimiter) {
    List* lista = list_create();
    if (lista == NULL || str == NULL) return NULL;

    char* token = strtok(str, &delimiter);
    while (token != NULL) {
        list_pushBack(lista, strdup(token)); 
        token = strtok(NULL, &delimiter);
    }

    return lista;
}

char **leer_linea_csv(FILE *archivo, char delimitador) {
    char linea[2048];
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        return NULL; 
    }

    linea[strcspn(linea, "\n")] = '\0';

    char **campos = malloc(sizeof(char*) * (2048 / 2)); 

    int i = 0;
    char *campo = strtok(linea, &delimitador); 

    while (campo != NULL) {
        campos[i] = malloc(strlen(campo) + 1);  
        strcpy(campos[i], campo);  
        i++;
        campo = strtok(NULL, &delimitador); 
    }

    campos[i] = NULL; 

    return campos;
}

/**
 * Carga canciones desde un archivo CSV
 */
List *leer_canciones() {
    List *ListaCanciones = malloc();
    // Intenta abrir el archivo CSV que contiene datos de películas
    FILE *archivo = fopen("data/song_dataset_.csv", "r");
    if (archivo == NULL) {
        perror(
            "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        return;
    }

    char **campos;
    // Leer y parsear una línea del archivo CSV. La función devuelve un array de
    // strings, donde cada elemento representa un campo de la línea CSV procesada.
    campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

    int k = 0;
    // Lee cada línea del archivo CSV hasta el final
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        k++;
        if (k > 10) break; //carga primeras 1000 canciones


        Song *c = malloc(sizeof(Song));
        if (!c) continue;

        c->id = atoi(campos[0]);
        c->track_name = strcpty(campos[4]);
        c->album_name = strdup(campos[3]);
        c->track_genre = strdup(campos[20]);
        c->tempo = atof(campos[18]);

        // Divide artistas por ;
        c->artists = split_string(campos[2], ';');

        list_pushBack(ListaCanciones, c);

        
        /* printf("ID: %d\n", atoi(campos[0]));
        printf("Titulo cancion: %s\n", campos[4]);

        List* artistas = split_string(campos[2], ',');

        printf("Artistas: ");
        for(char *artista = list_first(artistas); artista != NULL; artista = list_next(artistas)) {
            printf("%s\n", artista);
        }   
      
        printf("Album: %s\n", campos[3]);
        printf("Genero: %s\n", campos[20]);
        printf("Tempo: %.2f\n", atof(campos[18]));
        printf(" -------------------------------\n"); */
    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
    return ListaCanciones;
}



int main() {
    List *canciones = leer_canciones();
    

    return 0;
}
