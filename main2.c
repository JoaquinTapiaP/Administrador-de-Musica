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
    return strcmp((char *)key1, (char *)key2) == 0; //si dos llaves son iguales, devuelve
}

int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

void normalize(char *str) {
    // Esta funcion convertira tildes, mayusculas o simbolos en minusculas que se puedan leer y procesar de manera normal
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

List *Cargar_Canciones(Map *MapArtist, Map *MapGenre, List *SlowBPM, List *MidBPM, List *FastBPM) {
    // Esta funcion leera las canciones de un archivo de texto y las almacenara
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

        list_pushBack(Cancion->track_genre, strdup(campos[20])); //agrega al final la track_genre de la cancion
        list_pushBack(ListaCanciones, Cancion); // agrega la cancion al final de la lista con todas las canciones

        char *genre = list_first(Cancion->track_genre);

        while (genre != NULL) { //mientras que queden canciones
            MapPair *genre_pair = map_search(MapGenre, genre); //Se crea un dato que reciba el par que encontrara la funcion map_search

            if (genre_pair == NULL) { //Si el par encontrado es nulo
                List *new_list = list_create(); // Crea una lista para poder agregar las canciones
                list_pushBack(new_list, Cancion); // Añade dentro de la nueva lista la cancion 
                map_insert(MapGenre, genre, new_list); // Añade al mapa el genero de la nueva cancion agregada para facilitar la busqueda y orden a futuro
            } else { //si el par encontrado no es nulo (ya esta)
                List *genre_list = (List *)genre_pair->value; // Crea una lista para el genero y actualiza
                list_pushBack(genre_list, Cancion); // añade al final de la lista creada la cancion
            }

            genre = list_next(Cancion->track_genre); // Actualiza la cancion para mostrar los datosa de la siguiente
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
    // Esta funcion mostrara todos los datos de una cancion
    // Verificar que los campos esenciales no sean NULL
    printf("ID: %s\n", Cancion->id);

    // Verificar si track_genre existe
    if (Cancion->track_genre != NULL) {
        char *genre = list_first(Cancion->track_genre);
        if (genre != NULL) {
            printf("Genero: %s\n", genre);
        }
    }
    // Mostrar los datos de la cancion con el orden de artista, album, titulo y tempo
    printf("Artista: %s\n", Cancion->artists);
    printf("Album: %s\n", Cancion->album_name);
    printf("Titulo: %s\n", Cancion->track_name);
    printf("Tempo: %.2f BPM\n", Cancion->tempo);
}

void showList(List *Lista) {
    // Esta funcion mostrara los datos de una lista 
    if (Lista == NULL || list_first(Lista) == NULL) { // Si la lista esta vacia
        printf("La lista está vacía.\n");
        return;
    }

    Song *cancion = list_first(Lista); //crea un dato tipo song que sera igual al primer dato de la lista ingresada
    while (cancion != NULL) { // Si la cancion existe
        printf("-----------------------------\n");
        printCancion(cancion); // Llama a la funcion printCancion y imprime la cancion
        cancion = list_next(Lista); // Actualiza la cancion para que pase a la siguiente
    }
    printf("-----------------------------\n");
}

void SearchByGenre(Map *MapGenre) {
    // Esta funcion buscara entre todas las canciones basandose en el genero que el usuario ingrese (acoustic, anime, electronic, etc)
    char genero[100];
  
    // Solicita al usuario el ID de la película
    printf("Ingrese el genero que busca: ");
    scanf(" %[^\n]s", genero); // Lee el ID del teclado
    getchar();
  
    MapPair *pair = map_search(MapGenre, genero); //se crea un dato llamado par para buscar el par que coincida con el genero
    
    if (pair != NULL) { // si existen canciones de ese genero en la lista
        List* genreList = pair->value; // Se crea una lista que almacenara las canciones de ese genero
        Song *Cancion = list_first(genreList); // Se crea un dato de tipo song que se usara para recorrer las canciones

        while (Cancion != NULL) { // Si la cancion existe
            printf("-----------------------------\n");
            
            printCancion(Cancion); // Llama a la funcion printCancion y imprime los datos de la cancion
            Cancion = list_next(genreList); //actualiza cancion para mostrar los datos de la siguiente cancion
        }
        printf("-----------------------------\n");
    }
    else { // Si no hay canciones que sean de ese genero
        printf("No hay canciones de ese genero para mostrar.\n");
        return;
    }
}

void SearchByArtist(Map *MapArtist) {
    // Esta funcion buscara entre todas las canciones basandose en el artista que el usuario ingrese
    char artista[100];
  
    // Solicita al usuario el artista de la canción
    printf("Ingrese el artista que busca: ");

    // Limpia el '\n' si vienes de un scanf anterior
    scanf(" %[^\n]s", artista); // Elimina el salto de línea al final
    
  
    MapPair *pair = map_search(MapArtist, artista); // Se crea un dato que se usara para recibir los resultados de la busqueda respecto al artista
    
    if (pair != NULL) { // Si existen canciones con ese artista
        printf("Mostrando todas las canciones encontradas de %s\n", artista);
        List* music = pair->value; // Se crea una lista que almacenara canciones de ese artista
        Song *Cancion = list_first(music); // Se crea un dato tipo song que se usara para recorrer 
    

        while (Cancion != NULL) { // Mientras queden canciones
            printf("-----------------------------\n");
            
            printCancion(Cancion); // Muestra los datos de la cancion 
            Cancion = list_next(music); // Actualiza cancion para que muestre los datos de la siguiente cancion
        }
        printf("-----------------------------\n");
    }
    else { // Si no quedan canciones
        printf("No hay canciones de ese artista para mostrar.\n");
        return;
    }
}

void SearchByTempo(List *SlowBPM, List *MidBPM, List *FastBPM) {
    // se pide al usuario que ingrese que tempo desea buscar
    printf("Inserte la veloidad de las canciones deseadas: \n");
    puts("Lentas, Moderadas o Rapidas");
    char inputBPM[100];
    scanf(" %[^\n]s", inputBPM);

    normalize(inputBPM); // <- aquí forzamos el formato

    if (strcmp(inputBPM, "lentas") == 0) { // Si se cumple la condicion para que sea de tempo lento
        showList(SlowBPM); // Se muestran las canciones de esa lista (tempo)
    }
    else if (strcmp(inputBPM, "moderadas") == 0) { // Si se cumple la condicion para que sea de tempo moderado
        showList(MidBPM); // Se muestran las canciones de esa lista (tempo)
    }
    else if (strcmp(inputBPM, "rapidas") == 0) { // Si se cumple la condicion para que sea de tempo rapido
        showList(FastBPM); // Se muestran las canciones de esa lista (tempo)
    }
    else { // Si no encuentra de esos tempos
        printf("Tipo de tempo no reconocido.\n");
    }

}

void CreatePlayList(Map *AllPlaylists) {
    // Se pide al usario que ingrese los el nombre de la playlist que quiera crear
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

    limpiarPantalla(); // Limpia la pantalla para que se vea ordenado
    printf("La Playlist '%s' ha sido creada exitosamente.\n", nombrePlaylist);
}

void ShowAllPlaylists(Map *AllPlaylists) {
    // Esta funcion mostrara todas las playlist creadas actualmente
    printf("\n--- Listas de Reproducción Disponibles ---\n");
    
    // Obtener el primer par del mapa
    MapPair *pair = map_first(AllPlaylists);
    
    if (pair == NULL) { // Si no hay listas de reproduccion (playlists)
        printf("No hay listas de reproducción creadas.\n");
        return;
    }
    
    // Recorrer todas las entradas del mapa
    while (pair != NULL) { // mientras queden playlists
        printf("• %s\n", (char*)pair->key);  // Imprimir el nombre de la playlist
        pair = map_next(AllPlaylists);       // Avanzar al siguiente elemento
    }
    printf("----------------------------------------\n");
}

void AddSong(List *Canciones, Map *AllPlaylists) {
    // Esta funcion añadira una cancion en base a una ID a una playlist
    char IDBuscada[100];
    printf("Ingrese el ID de la canción: \n");
    scanf(" %99[^\n]", IDBuscada);
    getchar();

    limpiarPantalla();

    Song *current = list_first(Canciones); // Se crea un dato tipo song que sera usado para recorrer la lista
    if (current == NULL) { // Si no hay canciones
        printf("No hay canciones disponibles.\n");
        return;
    }

    while (current != NULL) { // Mientras hayan canciones
        if (strcmp(current->id, IDBuscada) == 0) { // Si la ID ingresada coincide con una de las canciones, copiala
            break;
        }
        current = list_next(Canciones); // si no encuentra la ID, pasa a la siguiente cancion
    }

    if (current == NULL) { // Si current sigue siendo null, significa que no encontro la cancion
        printf("No se encontró ninguna canción con ese ID.\n");
        return;
    }

    // Informa que la cancion fue detectada y empieza el proceso de agregarla a la playlist
    char PlaylistReq[100];
    printf("Cancion Detectada: \n");
    printf("-----------------------------\n");
    printCancion(current);
    printf("-----------------------------\n");

    ShowAllPlaylists(AllPlaylists); // Muestra todas las playlists creadas y pidele al usuario que elija a cual quiere agregar
    printf("Ingrese el nombre de la Playlist a la que se quiere añadir la cancion: \n");
    scanf(" %[^\n]s", PlaylistReq);
    getchar();
    normalize(PlaylistReq); // convierte tildes, mayusculas y caracteres extraños para que se pueda trabajar
    limpiarPantalla(); // Limpia la pantalla por temas de orden

    MapPair *pair = map_search(AllPlaylists, PlaylistReq); // Obtener MapPair*
    if (pair == NULL) { // Si la playlist ingresada no es encontrada
        printf("No existe una playlist con ese nombre.\n");
        return;
    }

    List *playlist = (List*) pair->value; // Extraer la lista
    list_pushBack(playlist, current); // Añade la cancion a la playlist
    printf("La cancion '%s' ha sido añadida a la lista '%s' exitosamente.\n", current->track_name, pair->key);
}

void ShowPlayList(Map *AllPlaylists) {
    // Esta funcion mostrara todas las canciones de una playlist
    char PlaylistReq[100];
    printf("Ingrese el nombre de la Playlist: \n");
    scanf(" %[^\n]", PlaylistReq);
    getchar();
    normalize(PlaylistReq); // Convierte tildes, mayusculas y simbolos para que se puedan trabajar
    limpiarPantalla(); // Se limpia la pantalla por temas de orden

    // Obtener el MapPair que contiene la lista
    MapPair *pair = map_search(AllPlaylists, PlaylistReq); // Se crea un par que sera usado para ver si se encuentra
    
    if (pair == NULL) { // Si no encuentra la playlist
        printf("No existe una playlist con ese nombre.\n");
        return;
    }

    // Extraer la lista del par
    List *playlist = (List*) pair->value; // Crea una lista con las canciones de la playlist
    printf("Mostrando canciones de la lista '%s' \n", pair->key);
    showList(playlist); // Muestra todas las canciones de una playlist o lista
}

void mostrarMenuPrincipal() {
    // Esta funcion mostrara un menu para la comodidad y entendimiento del usuario
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

void salir(Map *MapArtist, Map *MapGenre, Map *AllPlaylists, List *ListaCanciones) {
    // Esta funcion liberara todos los datos para apagar el sistema (terminar el codigo)
    free(ListaCanciones);
    free(MapArtist);
    free(MapGenre);
    free(AllPlayLists)

    printf("Gracias por usas el sistema de musica.\n");
    printf("Terminando el programa...\n");
}

int main() {
    system("chcp 65001 > NUL"); //Permite leer mostrar tildes, letras japones/chino, etc
    //setlocale(LC_ALL, "");

    Map *MapArtist = map_create(is_equal_str); //  Crea un mapa para los artistas
    Map *MapGenre = map_create(is_equal_str); // Crea un mapa para los generos
    //List *canciones = leer_canciones(Music_Artist, Music_Genre);
    List *canciones; // Se crea una lista para las canciones
    bool Proceso = false;
    
    List *SlowBPM = list_create(); // Se crea una lista para las canciones de tempo lento
    List *MidBPM = list_create(); // Se crea una lista para las canciones de tempo medio
    List *FastBPM = list_create(); // Se crea una lista para las canciones de tempo rapido
    
    Map *AllPlaylists = map_create(is_equal_str); // Se crea un mapa que almacenara todas las playlist


    int option; // se le pedira al usuario que ingrese un numero relacionado a una opcion por lo que se crea este dato
    do { // haz lo siguiente mientras que se cumpla la condicion de que no ingresen 8
        mostrarMenuPrincipal(); // Se muestra el menu principal
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
                salir(MapArtist, MapGenre, AllPlaylists ,canciones);  
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