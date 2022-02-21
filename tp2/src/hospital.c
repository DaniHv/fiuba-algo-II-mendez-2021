#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "hospital_structs.h"
#include "split.h"
#include "lista.h"
#include "abb.h"

#define ERROR -1
#define INVALIDO 0

typedef struct recorrido_pokemon_aux {
    bool (*funcion)(pokemon_t* p);
} recorrido_pokemon_aux_t;

int comparador_nombres_pokemon(void* dato1, void* dato2) {
    pokemon_t* pokemon1 = (pokemon_t*) dato1;
    pokemon_t* pokemon2 = (pokemon_t*) dato2;

    return strcmp(pokemon1->nombre, pokemon2->nombre);
}

/*
 * Pre: -
 * Post: Crea un hospital con datos iniciales válidos (Cantidades en 0, y vectores NULL).
 *       En caso de error al inicializar, devuelve NULL.
 */
hospital_t* hospital_crear() {
    hospital_t* hospital = (hospital_t*) malloc(sizeof(hospital_t));

    if (!hospital) return NULL;

    hospital->entrenadores = lista_crear();
    hospital->pokemones_alfabeticamente = abb_crear(&comparador_nombres_pokemon);

    if (!hospital->entrenadores || !hospital->pokemones_alfabeticamente) {
        lista_destruir(hospital->entrenadores);
        abb_destruir(hospital->pokemones_alfabeticamente);
        free(hospital);
        return NULL;
    }

    return hospital;
}

pokemon_t* crear_pokemon(entrenador_t* entrenador, char* nombre, size_t nivel) {
    if (!nombre) return NULL;

    pokemon_t* pokemon = (pokemon_t*) malloc(sizeof(pokemon_t));
    if (!pokemon) {
        return NULL;
    }

    pokemon->nombre = (char*) malloc((strlen(nombre) + 1) * sizeof(char));
    if (!pokemon->nombre) {
        free(pokemon);
        return false;
    }

    strcpy(pokemon->nombre, nombre);
    pokemon->entrenador = entrenador;
    pokemon->nivel = nivel;

    return pokemon;
}

void destruir_pokemon(pokemon_t* pokemon) {
    if (!pokemon) return;

    free(pokemon->nombre);
    free(pokemon);
}

/*
 * Pre: -
 * Post:
 */
void destruir_entrenador(entrenador_t* entrenador) {
    if (!entrenador) return;

    lista_destruir(entrenador->pokemones);

    free(entrenador->nombre);
    free(entrenador);
}

/*
 * Pre: El array de datos debe tener los datos de pokemones empezando en la posición 3 (i = 2),
 *      y con sus datos ordenados en par 1 -> nombre y 2 -> nivel.
 * Post: Crea y guarda pokemones en base a procesar los datos en el hospital recibido.
 *       En caso de recibir un hospital o datos inválido, o algún error al crear algún pokemon,
 *       devuelve FALSE, caso contrario TRUE.
 */
bool guardar_pokemones(entrenador_t* entrenador, abb_t* pokemones_alfabeticamente, char** datos) {
    if (!entrenador || !pokemones_alfabeticamente) return false;

    size_t i = 0;
    while (datos[i] && datos[i + 1]) {
        pokemon_t* pokemon = crear_pokemon(entrenador, datos[i], (size_t) atoi(datos[i + 1]));
        if (!pokemon) {
            return false;
        }

        if(!lista_insertar(entrenador->pokemones, (void*) pokemon) || !abb_insertar(pokemones_alfabeticamente, (void*) pokemon)) {
            destruir_pokemon(pokemon);
            return false;
        }

        i += 2;
    }

    return true;
}

/*
 * Pre: -
 * Post: Destruirá (liberará la memoria) del array de strings recibido,
 *       incluyendo cada uno de los substrings contenidos en él.
 */
void destruir_split(char** split) {
    if (!split) return;

    size_t i = 0;
    while (split[i]) {
        free(split[i]);
        i++;
    }

    free(split);
}

entrenador_t* crear_entrenador(size_t id, char* nombre) {
    if (!nombre) return NULL;

    entrenador_t* entrenador = malloc(sizeof(entrenador_t));

    if (!entrenador) {
        return NULL;
    }

    entrenador->id = id;

    entrenador->nombre = (char*) malloc((strlen(nombre) + 1) * sizeof(char));
    if (!entrenador->nombre) {
        free(entrenador);
        return NULL;
    }
    strcpy(entrenador->nombre, nombre);

    entrenador->pokemones = lista_crear();
    if (!entrenador->pokemones) {
        free(entrenador->nombre);
        free(entrenador);
        return NULL;
    }

    return entrenador;
}

/*
 * Pre: El array de datos debe tener en su primera posición (i = 0) el id del entrenador a guardar,
 *      y en la segunda posición (i = 1) su nombre, y los datos de pokemones empezando en la
 *      posición 3 (i = 2), y con sus datos ordenados en par 1 -> nombre y 2 -> nivel.
 * Post: Añadirá los datos al hospital recibido (entrenador y pokemones del entrenador).
 *       Si se recibe un hospital o datos inválido, u ocurre un error en el guardado de
 *       los nuevos datos, devuelve FALSE, caso contrario TRUE.
 */
bool actualizar_hospital(hospital_t* hospital, char** datos) {
    if (!hospital) return false;
    if (!datos) return false;

    entrenador_t* entrenador = crear_entrenador((size_t) atoi(datos[0]), datos[1]);
    if(!entrenador) {
        return false;
    }

    if(!lista_insertar(hospital->entrenadores, entrenador)) {
        destruir_entrenador(entrenador);
        return false;
    }

    if (!guardar_pokemones(entrenador, hospital->pokemones_alfabeticamente, datos + 2)) return false;

    return true;
}

/*
 * Pre: Para un correcto funcionamiento de lectura completa de línea, se debe recibir el char* vacío o NULL.
 * Post: Leerá una línea completa del archivo recibido y la guardará en el vector dinámico recibido,
 *       aumentando su tamaño según sea necesario.
 *       En caso de algún problema al redimensionar el vector o leer el archivo, devuelve ERROR,
 *       caso contrario devuelve la cantidad de caracteres procesados.
 */
int leer_linea_archivo(char** linea, FILE* archivo) {
    int caracter = fgetc(archivo);
    size_t tope = 0;

    while (caracter != EOF && caracter != '\n') {
        char* linea_aux = (*linea);
        (*linea) = realloc((*linea), sizeof(char) * (tope + 2));

        if (!(*linea)) {
            free(linea_aux);
            return ERROR;
        };

        (*linea)[tope] = (char) caracter;
        tope++;

        caracter = fgetc(archivo);
    }

    if (tope != 0) {
        (*linea)[tope] = '\0';
    };

    return (int) tope;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo) {
    if (!hospital) return false;

    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return false;

    size_t entrenadores_iniciales = lista_tamanio(hospital->entrenadores);
    bool hubo_error = false;
    char* linea = NULL;
    int leidos = leer_linea_archivo(&linea, archivo);

    while (leidos && leidos != ERROR && !hubo_error) {
        char** datos = split(linea, ';');

        if (!datos || !actualizar_hospital(hospital, datos)) {
            hubo_error = true;
        };

        destruir_split(datos);

        free(linea);
        linea = NULL;
        leidos = leer_linea_archivo(&linea, archivo);
    }

    fclose(archivo);

    if ((leidos == ERROR || hubo_error) && lista_tamanio(hospital->entrenadores) == entrenadores_iniciales) {
        return false;
    }

    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital) {
    if (!hospital) return INVALIDO;

    return abb_tamanio(hospital->pokemones_alfabeticamente);
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital) {
    if (!hospital) return INVALIDO;

    return lista_tamanio(hospital->entrenadores);
}

bool recorrido_abb_pokemon(void* elemento, void* aux) {
    if (!elemento || !aux) return false;

    pokemon_t* pokemon = (pokemon_t*) elemento;
    recorrido_pokemon_aux_t* recorrido_aux = (recorrido_pokemon_aux_t*) aux;

    return recorrido_aux->funcion(pokemon);
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)) {
    if (!hospital || !funcion) return INVALIDO;

    recorrido_pokemon_aux_t aux = {
        .funcion = funcion,
    };

    return abb_con_cada_elemento(hospital->pokemones_alfabeticamente, INORDEN, &recorrido_abb_pokemon, &aux);
}

bool lista_iterador_destruir_entrenador(void* elemento, void* aux) {
    destruir_entrenador((entrenador_t*) elemento);

    return true;
}

void abb_iterador_destruir_pokemon(void* elemento) {
    destruir_pokemon((pokemon_t*) elemento);
}

void hospital_destruir(hospital_t* hospital) {
    if (!hospital) return;

    lista_con_cada_elemento(hospital->entrenadores, &lista_iterador_destruir_entrenador, NULL);
    lista_destruir(hospital->entrenadores);
    abb_destruir_todo(hospital->pokemones_alfabeticamente, &abb_iterador_destruir_pokemon);

    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon) {
    if (!pokemon) return INVALIDO;

    return pokemon->nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon) return NULL;

    return pokemon->nombre;
}
