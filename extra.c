#include "extra.h"


#define MAX_LINE_LENGTH 4096
#define MAX_FIELDS      128

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH];
    static char *campos[MAX_FIELDS];
    int idx = 0;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL)
        return NULL;  // fin de fichero

    // quitar salto de línea
    linea[strcspn(linea, "\r\n")] = '\0';

    char *ptr = linea;
    while (*ptr && idx < MAX_FIELDS - 1) {
        char *start;

        if (*ptr == '\"') {
            // campo entrecomillado
            ptr++;              // saltar la comilla inicial
            start = ptr;

            // compactar contenido: convertir "" → " y copiar el resto
            char *dest = ptr;
            while (*ptr) {
                if (*ptr == '\"' && *(ptr + 1) == '\"') {
                    *dest++ = '\"';  // una comilla literal
                    ptr += 2;        // saltar ambas
                }
                else if (*ptr == '\"') {
                    ptr++;           // fin del campo
                    break;
                }
                else {
                    *dest++ = *ptr++;
                }
            }
            *dest = '\0';        // terminar cadena

            // ahora ptr apunta justo después de la comilla de cierre
            if (*ptr == separador) ptr++;
        }
        else {
            // campo sin comillas
            start = ptr;
            while (*ptr && *ptr != separador)
                ptr++;
            if (*ptr == separador) {
                *ptr = '\0';
                ptr++;
            }
        }

        campos[idx++] = start;
    }

    campos[idx] = NULL;
    return campos;
}


List *split_string(const char *str, const char *delim) {
    List *result = list_create();
    if (result == NULL) return NULL;  // Verificar si la lista se crea correctamente.
  
    // Crear una copia de la cadena para evitar modificar la original
    char *str_copy = strdup(str);
    if (str_copy == NULL) return NULL;  // Verificar si la copia se creó correctamente.
  
    char *token = strtok(str_copy, delim);
  
    while (token != NULL) {
      // Eliminar espacios en blanco al inicio del token
      while (*token == ' ') {
        token++;
      }
  
      // Eliminar espacios en blanco al final del token
      char *end = token + strlen(token) - 1;
      while (*end == ' ' && end > token) {
        *end = '\0';
        end--;
      }
  
      // Comprobar si el token no está vacío
      if (strlen(token) > 0) {
        // Copiar el token en un nuevo string
        char *new_token = strdup(token);
        if (new_token != NULL) {
          // Agregar el nuevo string a la lista
          list_pushBack(result, new_token);
        }
      }
  
      // Obtener el siguiente token
      token = strtok(NULL, delim);
    }
  
    free(str_copy);  // Liberar la memoria de la copia de la cadena.
    return result;
  }

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}