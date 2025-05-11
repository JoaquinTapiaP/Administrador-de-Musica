## Administrador de Musica (A.M.)


## Descripción
Este programa permite a los usuarios buscar canciones de la base de datos y crear playlists a la cual se le pueden agregar canciones ya existentes.
Los usuarios pueden cargar la base de datos de las canciones, buscar canciones por genero, por artista o por tempo.
Esta herramiente esta hecha con el proposito de ser un sistema cómodo para administrar las canciones a peticion del usuario.


## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal `main.c`.
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal Git Bash, compila el programa con el siguiente comando: 
    ```
    gcc  main.c list.c map.c extra.c -Wno-unused-result -o Administrador-de-Musica
    ```
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
    ```
    ./Administrador-de-Musica
    ```

### Funcionalidades

### Funcionando correctamente:

- Cargar la base de datos de las canciones desde un archivo .csv (En vez de mostrar las 10000 canciones, se muestran las primeras 100 para que no colapse la terminal).
- Buscar y mostrar todas las canciones del genero que se busque.
- Buscar y mostrar todas las canciones del artista que se busque.
- Buscar y mostrar todas las canciones de cierto tempo en base a Lento siendo menor a 80 BPM, Moderado siendo de 80 BPM a 120 BPM y Rápidas siendo mayor que 120 BPM.
- Crear una lista de reproducción vacia con nombre customizable.
- Agregar una cancion a una lista de reproduccion en base al ID de la cancion.
- Mostrar todas las canciones de una lista de reproduccion

### Problemas conocidos:

- Si el archivo procesa muchas canciones, la terminal puede colapsar y no mostrar las canciones desde el inicio al final, y cortar las primeras canciones mostradas.
- No se pueden eliminar canciones de las listas de reproduccion.
- No se puede cambiar el nombre de una lista de reproduccion.
- No se puede cambiar el orden de las canciones de una lista de reproduccion.

### A mejorar:


- Permitir la muestra de canciones con mas de un artista en base al nombre de uno de los artistas involucrados.
- Permitir la busqueda de artistas independientemente de si su nombre incluye mayusculas, tildes o cáracteres especiales.

## Ejemplo de uso

**Paso 1: Registrar un Nuevo Ticket**

El usuario abre el programa por primera vez.

```
Opción seleccionada: 1) Cargar Canciones
```

El sistema procesa el archivo .csv de canciones y las añade para poder ser utilizadas.

**Paso 2: Buscar por genero**
Despues de que las cancione sean procecadas, el usuario busca canciones de un genero en especifico
```
Opción seleccionada: 2) Buscar por genero
Ingrese el genero que busca: j-pop
```
Si hay canciones de ese genero, el programa mostrara todas las canciones de ese genero.
Si no hay canciones de ese genero, el programa avisara que no hay y devolverá al usuario al menu principal

**Paso 3: Buscar por Artista**
El usuario ahora quiere encontrar todas las canciones de un artista en especifico.
```
Opción seleccionada: 3) Buscar por artista
Ingrese el artista que busca: YOASOBI
```
Si el artista existe y tiene canciones, mostrara todas las canciones que esten de forma individual de ese artista (Colaboraciones no se mostrarán).
Si el artista no tiene canciones, indicara que no hay canciones de ese artista y devolvera al usuario al menu principal

**Paso 4: Buscar por tempo**
El usuario ahora quiere buscar canciones que tengan una rapidez general
```
Opción seleccionada: 4) Buscar por tempo
Inserte la velocidad de las canciones deseadas: Lentas, Moderadas o Rapidas:
- Rapidas
```
El programa mostrara todas las canciones de la velocidad solicitada, si el usuario inserta un tipo de velocidad invalida, indicara que esa velocidad no existe y devolverá al usuario al menu principal.

**Paso 5: Crear Lista de reproduccion**
El usuario quiere crear una lista de reproduccion con sus canciones favoritas.
```
Opción seleccionada: 5) Crear Lista de reproduccion
Ingrese el nombre de la nueva playlist: Favs
```
El programa creara la playlist con el nombre indicado por el usuario.

**Paso 6: Agregar cancion a la lista**
El usuario quiere añadir canciones a su lista de reproudccion.
```
Opción seleccionada: 6) Agregar cancion a la lista
Ingrese el ID de la cancion: 62330
Ingrese el nombre de la Playlist a la que se quiere añadir la cancion: Favs
```
El programa mostrará los datos de la cancion si es encontrada, sino devolvara al usuario al menu principal.
Luego el programa mostrara todas las listas de reproduccion existentes.
El usuario escribe el nombre de la lista y la cancion será agregada, si la lista no existe, el usuario se le devolverá al menu principal.

**Paso 7: Mostrar canciones de una lista**
El usuario quiere ver las canciones de su lista de reproduccion.
```
Opción seleccionada: 7) Mostrar canciones de una lista
Ingrese el nombre de la Playlist: Favs
```
El programa mostrara las listas existentes.
El usuario indica el nombre de la lista solicitada.
El programa mostrará los datos de todas las canciones de la lista de reproducción.

**Paso 8: Salir**
El usuario desea salir del programa.
```
Opción Seleccionada: 6) Salir
```
El programa se termina y elimina la informacion.

## Contribuciones

### Joaquin Tapia:

- Desarrolló las funciones generales, cargar canciones, buscar por genero, buscar por tempo, Agregar cancion a lista,Mostrar Canciones de una lista, Salir.
- Diseño y redacción del README.
- Correccion de funciones.

### Joaquin Guarda:

- Desarrolló las funciones Buscar por Artista, Crear Lista de reproducción, Agregar cancion a lista, Salir,
- Documentación y comentarios en el código.
- Correción del README.