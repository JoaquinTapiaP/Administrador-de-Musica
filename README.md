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
    gcc  TSMS.c list.c -Wno-unused-result -o Tech-Support-Management-System
    ```
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
    ```
    ./Tech-Support-Management-System
    ```

### Funcionalidades

### Funcionando correctamente:

- Registrar tickets con su ID, descripcion, hora y una prioridad inicial en orden de llegada.
- Modificar la prioridad de los tickets.
- Ver la lista de espera de tickets, ordenada por prioridad y hora de registro.
- Atender al siguiente usuario, respetando el orden de prioridad (de alta a baja).
- Buscar un usuario según su ID.

### Problemas conocidos:

- Una vez la prioridad de un ticket en la lista de prioridad baja es cambiada, no se puede cambiar al estar en otra lista de prioridad

### A mejorar:

- Permitir la edición de la descripcion de los tickets.
- Permitir el cambio de prioridades cuando no se encuentran en prioridad baja.


## Ejemplo de uso

**Paso 1: Registrar un Nuevo Ticket**

Se comienza registrando un nuevo ticket para la persona que acaba de llegar al servicio tecnica.

```
Opción seleccionada: 1) Registrar ticket
Ingrese el ID del usuario: 6
Ingrese la descripcion del problema del usuario: Pantalla Rota
Ingrese hora de llegada: 12:34
```

El sistema registra a un usuario con una ID, con una prioridad inicial "Bajo" y guarda la hora actual de registro en formato 24 horas (ejemplo: 12:34, 15:30, 23:32) La prioridad inicial puede ser ajustada más adelante.

**Paso 2: Reasignar Prioridad a un Usuario**

Tras una evaluación inicial, el tecnico determina que el usuario requiere atención prioritaria.

```
Opción seleccionada: 2) Asignar nueva prioridad a usuario
Ingrese el ID del usuario: 6
Seleccione el nuevo nivel de prioridad (Alto = 1, Medio = 2, Bajo = 3): 1
```

El sistema cambiara la prioridad del usuario a alto, y lo movera a la lista de alta prioridad.

**Paso 3: Mostrar la lista de espera**
El usuario desea ver la lista de usuarios del dia.

```
Opción Seleccionada: 3) Mostrar lista de espera
```

Mostrara la lista de espera, desde el primer usuario en la lista de pruioriad alta, hasta el ultimo usuario en la lista de prioridad baja.

**Paso 4: Mostrar la lista de espera**
El primer usuario es llamado para ser atendido.

```
Opción Seleccionada: 4) Procesar siguiente usuario
```
El sistema muestra la informacion del usuario, y lo elimina de la lista de espera.

**Paso 5: Buscar por un usuario**
El usuario desea buscar la informacion de un usuario en especifico
```
Opción Seleccionada: 5) Buscar un usuario
Ingrese el ID del usuario: 6
```

El sistema muestra la informacion del usuario.

**Paso 6: Buscar por un usuario**
Al final del dia, el técnico desea terminar el programa y vaciar toda la lista de espera
```
Opción Seleccionada: 6) Abandonar programa
```
El programa se termina y elimina la informacion restante.