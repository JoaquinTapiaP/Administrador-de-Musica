#include "extra.h"
#include "map.h"
#include "list.h"


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct {
    char id[100];
    char artists[100];
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

void normalize(char *str) {
    int i = 0, j = 0;
    char result[100]; // asegúrate de que tenga suficiente espacio

    while (str[i]) {
        unsigned char c = (unsigned char)str[i];

        // Detectar caracteres multibyte (UTF-8)
        if ((c & 0xE0) == 0xC3) {  // caracteres latinos acentuados comunes
            unsigned char next = (unsigned char)str[i + 1];
            switch (next) {
                case 0xA1: result[j++] = 'a'; break; // á
                case 0xA9: result[j++] = 'e'; break; // é
                case 0xAD: result[j++] = 'i'; break; // í
                case 0xB3: result[j++] = 'o'; break; // ó
                case 0xBA: result[j++] = 'u'; break; // ú
                default:
                    // si no es una tilde conocida, ignora ambos bytes
                    break;
            }
            i += 2; // saltar dos bytes
        } else {
            result[j++] = tolower(c);
            i++;
        }
    }
    result[j] = '\0';
    strcpy(str, result);
}

/**
 * Carga canciones desde un archivo CSV
 */

List *Cargar_Canciones(Map *MapArtist, Map *MapGenre, List *SlowBPM, List *MidBPM, List *FastBPM) {
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
        Song *Cancion = (Song *) malloc(sizeof(Song));

        //k++;
        //if (k > 10) break;
        strcpy(Cancion->id, campos[0]);
        strcpy(Cancion->artists, campos[2]);
        strcpy(Cancion->album_name, campos[3]);
        strcpy(Cancion->track_name, campos[4]);
        Cancion->tempo = atof(campos[18]);
        Cancion->track_genre = list_create();

        /*char *genero_limpio = quitar_tildes(campos[20]);
        list_pushBack(Cancion->track_genre, strdup(genero_limpio));*/

        list_pushBack(Cancion->track_genre, strdup(campos[20]));
        list_pushBack(ListaCanciones, Cancion);

        char *genre = list_first(Cancion->track_genre);

        while (genre != NULL) {
            MapPair *genre_pair = map_search(MapGenre, genre);

            if (genre_pair == NULL) {
                List *new_list = list_create();
                list_pushBack(new_list, Cancion);
                map_insert(MapGenre, genre, new_list);
            } else {
                List *genre_list = (List *)genre_pair->value;
                list_pushBack(genre_list, Cancion);
            }

            genre = list_next(Cancion->track_genre);
        }

        MapPair *artist_pair = map_search(MapArtist, Cancion->artists);
        if (artist_pair == NULL) {
            List *new_list_artist = list_create();
            list_pushBack(new_list_artist, Cancion);
            map_insert(MapArtist, strdup(Cancion->artists), new_list_artist);
        }
        else { 
            List *artist_list = (List *)artist_pair->value;
            list_pushBack(artist_list, Cancion);
        }

        if (Cancion->tempo < 80) {
            list_pushBack(SlowBPM, Cancion);
        }
        else if (Cancion->tempo >= 80 && Cancion->tempo <= 120) {
            list_pushBack(MidBPM, Cancion);
        }
        else
            list_pushBack(FastBPM, Cancion);
        
        
    }

    fclose(archivo); // Cierra el archivo después de leer todas las líneas
    return ListaCanciones;
}

void printCancion(Song* Cancion) {
    // Verificar que los campos esenciales no sean NULL
    printf("ID: %s\n", Cancion->id);

    // Verificar si track_genre existe
    if (Cancion->track_genre != NULL) {
        char *genre = list_first(Cancion->track_genre);
        if (genre != NULL) {
            printf("Genero: %s\n", genre);
        }
    }

    printf("Artista: %s\n", Cancion->artists);
    printf("Album: %s\n", Cancion->album_name);
    printf("Titulo: %s\n", Cancion->track_name);
    printf("Tempo: %.2f BPM\n", Cancion->tempo);
}

void showList(List *Lista) {
    if (Lista == NULL || list_first(Lista) == NULL) {
        printf("La lista está vacía.\n");
        return;
    }

    Song *cancion = list_first(Lista);
    while (cancion != NULL) {
        printf("-----------------------------\n");
        printCancion(cancion);
        cancion = list_next(Lista);
    }
    printf("-----------------------------\n");
}

void SearchByGenre(Map *MapGenre) {
    char genero[100];
  
    // Solicita al usuario el ID de la película
    printf("Ingrese el genero que busca: ");
    scanf(" %[^\n]s", genero); // Lee el ID del teclado
    getchar();
  
    MapPair *pair = map_search(MapGenre, genero);
    
    if (pair != NULL) {
        List* pelis = pair->value;
        Song *Cancion = list_first(pelis);

        while (Cancion != NULL) {
            printf("-----------------------------\n");
            
            printCancion(Cancion);
            Cancion = list_next(pelis);
        }
        printf("-----------------------------\n");
    }
    else {
        printf("No hay canciones de ese genero para mostrar.\n");
        return;
    }
    //printf("Funciona\n");
}

void SearchByArtist(Map *MapArtist) {
    char artista[100];
  
    // Solicita al usuario el artista de la canción
    printf("Ingrese el artista que busca: ");

    // Limpia el '\n' si vienes de un scanf anterior
    scanf(" %[^\n]s", artista); // Elimina el salto de línea al final
    
  
    MapPair *pair = map_search(MapArtist, artista);
    
    if (pair != NULL) {
        printf("Mostrando todas las canciones encontradas de %s\n", artista);
        List* music = pair->value;
        Song *Cancion = list_first(music);
    

        while (Cancion != NULL) {
            printf("-----------------------------\n");
            
            printCancion(Cancion);
            Cancion = list_next(music);
        }
        printf("-----------------------------\n");
    }
    else {
        printf("No hay canciones de ese artista para mostrar.\n");
        return;
    }
    //printf("Funciona\n");
}

void SearchByTempo(List *SlowBPM, List *MidBPM, List *FastBPM) {
    printf("Inserte la veloidad de las canciones deseadas: \n");
    puts("Lentas, Moderadas o Rapidas");
    char inputBPM[100];
    scanf(" %[^\n]s", inputBPM);

    normalize(inputBPM); // <- aquí forzamos el formato

    if (strcmp(inputBPM, "lentas") == 0) {
        showList(SlowBPM);
    }
    else if (strcmp(inputBPM, "moderadas") == 0) {
        showList(MidBPM);
    }
    else if (strcmp(inputBPM, "rapidas") == 0) {
        showList(FastBPM);
    }
    else {
        printf("Tipo de tempo no reconocido.\n");
    }

}

void CreatePlayList(Map *AllPlaylists) {
    char nombrePlaylist[100];
    printf("Ingrese el nombre de la nueva playlist: \n");
    scanf(" %[^\n]", nombrePlaylist);
    getchar();
    normalize(nombrePlaylist);

    // Verifica si ya existe la playlist
    if (map_search(AllPlaylists, nombrePlaylist) != NULL) {
        printf("Ya existe una playlist con ese nombre.\n");
        return;
    }

    // Crear una lista vacía para la nueva playlist
    List *nuevaLista = list_create();
    map_insert(AllPlaylists, strdup(nombrePlaylist), nuevaLista);

    limpiarPantalla();
    printf("La Playlist '%s' ha sido creada exitosamente.\n", nombrePlaylist);
}

void ShowAllPlaylists(Map *AllPlaylists) {
    printf("\n--- Listas de Reproducción Disponibles ---\n");
    
    // Obtener el primer par del mapa
    MapPair *pair = map_first(AllPlaylists);
    
    if (pair == NULL) {
        printf("No hay listas de reproducción creadas.\n");
        return;
    }
    
    // Recorrer todas las entradas del mapa
    while (pair != NULL) {
        printf("• %s\n", (char*)pair->key);  // Imprimir el nombre de la playlist
        pair = map_next(AllPlaylists);       // Avanzar al siguiente elemento
    }
    printf("----------------------------------------\n");
}

void AddSong(List *Canciones, Map *AllPlaylists) {
    char IDBuscada[100];
    printf("Ingrese el ID de la canción: \n");
    scanf(" %99[^\n]", IDBuscada);
    getchar();
    limpiarPantalla();

    Song *current = list_first(Canciones);
    if (current == NULL) {
        printf("No hay canciones disponibles.\n");
        return;
    }

    while (current != NULL) {
        if (strcmp(current->id, IDBuscada) == 0) {
            break;
        }
        current = list_next(Canciones);
    }

    if (current == NULL) {
        printf("No se encontró ninguna canción con ese ID.\n");
        return;
    }

    char PlaylistReq[100];
    printf("Cancion Detectada: \n");
    printf("-----------------------------\n");
    printCancion(current);
    printf("-----------------------------\n");

    ShowAllPlaylists(AllPlaylists);
    printf("Ingrese el nombre de la Playlist a la que se quiere añadir la cancion: \n");
    scanf(" %[^\n]s", PlaylistReq);
    getchar();
    normalize(PlaylistReq);
    limpiarPantalla();

    MapPair *pair = map_search(AllPlaylists, PlaylistReq); // Obtener MapPair*
    if (pair == NULL) {
        printf("No existe una playlist con ese nombre.\n");
        return;
    }

    List *playlist = (List*) pair->value; // Extraer la lista
    list_pushBack(playlist, current);
    printf("La cancion '%s' ha sido añadida a la lista '%s' exitosamente.\n", current->track_name, pair->key);
}

void ShowPlayList(Map *AllPlaylists) {
    char PlaylistReq[100];
    printf("Ingrese el nombre de la Playlist: \n");
    scanf(" %[^\n]", PlaylistReq);
    getchar();
    normalize(PlaylistReq);
    limpiarPantalla();

    // Obtener el MapPair que contiene la lista
    MapPair *pair = map_search(AllPlaylists, PlaylistReq);
    
    if (pair == NULL) {
        printf("No existe una playlist con ese nombre.\n");
        return;
    }

    // Extraer la lista del par
    List *playlist = (List*) pair->value;
    printf("Mostrando canciones de la lista '%s' \n", pair->key);
    showList(playlist);
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
    //puts("tíldés");
}

void salir(Map *MapArtist, Map *MapGenre, List *ListaCanciones) {
    free(ListaCanciones);
    free(MapArtist);
    free(MapGenre);

    printf("Gracias por usas el sistema de musica.\n");
    printf("Terminando el programa...\n");
}

int main() {
    system("chcp 65001 > NUL"); //Permite leer mostrar tildes, letras japones/chino, etc
    //setlocale(LC_ALL, "");

    Map *MapArtist = map_create(is_equal_str);
    Map *MapGenre = map_create(is_equal_str);
    //List *canciones = leer_canciones(Music_Artist, Music_Genre);
    List *canciones;
    bool Proceso = false;
    
    List *SlowBPM = list_create();
    List *MidBPM = list_create();
    List *FastBPM = list_create();
    
    Map *AllPlaylists = map_create(is_equal_str);


    int option;
    do {
        mostrarMenuPrincipal();
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1: // CARGAR CANCIONES
                if (Proceso == false) {
                    Proceso = true;
                    canciones = Cargar_Canciones(MapArtist, MapGenre, SlowBPM, MidBPM, FastBPM);
                    limpiarPantalla();
                    printf("\nLas canciones han sido cargadas");
                }
                else {
                    limpiarPantalla();
                    printf("Las Canciones ya han sido procesadas");
                }
                break;
            case 2: //BUSCAR POR GENERO
                limpiarPantalla();
                SearchByGenre(MapGenre);
                break;
            case 3: //BUSCAR POR ARTISTA
                limpiarPantalla();
                SearchByArtist(MapArtist);
                break;
            case 4: // BUSCAR POR TEMPO
                limpiarPantalla();
                SearchByTempo(SlowBPM, MidBPM, FastBPM);
                break;
            case 5: // CREAR LISTA DE REPRODUCCION
                limpiarPantalla();
                CreatePlayList(AllPlaylists);
                break;
            case 6: // AGREGAR CANCION A LA LISTA
                limpiarPantalla();
                AddSong(canciones, AllPlaylists);
                break;
            case 7: //MOSTRAR CANCIONES DE UNA LISTA
                limpiarPantalla();
                ShowAllPlaylists(AllPlaylists);
                ShowPlayList(AllPlaylists);
                break;
            case 8: //SALIR
                salir(MapArtist, MapGenre, canciones);  
                break;
            default:
                limpiarPantalla(); 
                printf("Opción Invalida, intentelo otra vez\n");
        }

        if (option != 8) {
            printf("\n");
        }

    } while (option != 8);

    return 0;
}


//   gcc  main.c list.c map.c extra.c -Wno-unused-result -o Tarea-2
//   ./Tarea-2