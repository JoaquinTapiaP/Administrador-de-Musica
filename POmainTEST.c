#include "extra.h"
#include "map.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char id[100];
    List *artists;
    char album_name[100];   
    char track_name[100];
    float tempo;
    List *track_genre;
} Song;
  
int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga canciones desde un archivo CSV
 */
List *Cargar_Canciones(Map *MapArtist, Map *MapGenre) {
    List *ListaCanciones = list_create();
    // Intenta abrir el archivo CSV que contiene datos de películas
    FILE *archivo = fopen("data/song_dataset_.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        return NULL;
    }

    char **campos;
    // Leer y parsear una línea del archivo CSV. La función devuelve un array de
    // strings, donde cada elemento representa un campo de la línea CSV procesada.
    campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

    int k = 0;
    // Lee cada línea del archivo CSV hasta el final
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        //k++;
        //if (k > 10) break; //carga primeras 1000 canciones


        Song *Cancion = (Song *) malloc(sizeof(Song));

        strcpy(Cancion->id, campos[0]);

        //char **artistas = split_string(campos[2], ';');
        Cancion->artists = split_string(campos[2], ";");
        strcpy(Cancion->album_name, campos[3]);
        strcpy(Cancion->track_name, campos[4]);
        Cancion->tempo = atof(campos[18]);
        Cancion->track_genre = list_create();
        
        list_pushBack(Cancion->track_genre, strdup(campos[20]));

    
        // Divide artistas por ;
        list_pushBack(ListaCanciones, Cancion);

        //map_insert(pelis_byid, peli->id, peli);
        char *genre = list_first(Cancion->track_genre);

        while (genre != NULL) {
            // Busca el género en el mapa pelis_bygenres
            MapPair *genre_pair = map_search(MapGenre, genre);
    
            // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
            if (genre_pair == NULL) {
                List *new_list = list_create();
                list_pushBack(new_list, Cancion);
                map_insert(MapGenre, genre, new_list);
            } else {
                // Si el género ya existe en el mapa, obtén la lista y agrega la película
                List *genre_list = (List *)genre_pair->value;
                list_pushBack(genre_list, Cancion);
            }
    
            // Avanza al siguiente género en la lista
            genre = list_next(Cancion->track_genre);
        }


    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
    return ListaCanciones;
}

void SearchByGenre(Map *MapGenre) {
    char genero[100];
  
    // Solicita al usuario el ID de la película
    printf("Ingrese el genero que busca: ");
    scanf("%s", genero); // Lee el ID del teclado
  
    MapPair *pair = map_search(MapGenre, genero);
    
    if (pair != NULL) {
        List* pelis = pair->value;
        Song *Cancion = list_first(pelis);
        
        while (Cancion != NULL) {
            printf("-----------------------------\n");
            printf("ID: %s\n",Cancion->id);
            printf("Genero: %s\n", Cancion->track_genre);
            printf("Artista: %s\n",Cancion->artists);
            printf("Album: %s\n", Cancion->album_name);
            printf("Titulo: %s\n",Cancion->track_name);
            printf("Tempo: %d\n",Cancion->tempo);
            Cancion = list_next(pelis);
        }
        printf("-----------------------------\n");
    }
    //printf("Funciona\n");
}

void mostrarMenuPrincipal() {
    puts("========================================");
    puts("Base de canciones");
    puts("========================================");
  
    puts("1) Cargar canciones");
    puts("2) Buscar por genero");
    puts("3) Buscar por artista");
    puts("4) Buscar por tempo");
    puts("5) Crear lista de reproduccion");
    puts("6) Agregar cancion a la lista");
    puts("7) Mostrar canciones de una lista");
    puts("8) Salir");
}

int main() {
    //setlocale(LC_ALL, "");
    Map *MapArtist = map_create(is_equal_str);
    Map *MapGenre = map_create(is_equal_str);
    //List *canciones = leer_canciones(Music_Artist, Music_Genre);
    List *canciones;
    

    int option;
    do {
        mostrarMenuPrincipal();
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1: // CARGAR CANCIONES
                canciones = Cargar_Canciones(MapArtist, MapGenre);
                limpiarPantalla();
                printf("\nLas canciones han sido cargadas");
                break;
            case 2: //BUSCAR POR GENERO
                SearchByGenre(MapGenre);
                break;
            case 3: //BUSCAR POR ARTISTA
                //SearchByArtist();
                break;
            case 4: // BUSCAR POR TEMPO
                //SearchByTempo();
                break;
            case 5: // CREAR LISTA DE REPRODUCCION
                //CreateRepList();
                break;
            case 6: // AGREGAR CANCION A LA LISTA
                //AddSong();
                break;
            case 7: //MOSTRAR CANCIONES DE UNA LISTA
                //ShowSongs();
                break;
            case 8: //SALIR
                printf("CERRANDO");
                break;
            default:
                printf("Invalid option, please try again\n");
        }

        if (option != 8) {
            printf("\n");
        }

    } while (option != 8);

    return 0;
}
