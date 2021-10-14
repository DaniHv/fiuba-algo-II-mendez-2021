#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "split.h"

#define ERROR -1
#define INVALIDO 0

typedef struct entrenador {
    size_t id;
    char* nombre;
} entrenador_t;

struct _hospital_pkm_t {
    size_t cantidad_entrenadores;
    entrenador_t* vector_entrenadores;
    size_t cantidad_pokemon;
    pokemon_t* vector_pokemones;
};

struct _pkm_t {
    size_t entrenador_id;
    char* nombre;
    size_t nivel;
};

/*
 * Pre: -
 * Post: Crea un hospital con datos iniciales válidos (Cantidades en 0, y vectores NULL).
 *       En caso de error al inicializar, devuelve NULL.
 */
hospital_t* hospital_crear() {
    hospital_t* hospital = (hospital_t*) malloc(sizeof(hospital_t));

    if (!hospital) return NULL;

    hospital->cantidad_entrenadores = 0;
    hospital->vector_entrenadores = NULL;
    hospital->cantidad_pokemon = 0;
    hospital->vector_pokemones = NULL;

    return hospital;
}


/*
 * Pre: El array de datos debe tener los datos de pokemones empezando en la posición 3 (i = 2),
 *      y con sus datos ordenados en par 1 -> nombre y 2 -> nivel.
 * Post: Crea y guarda pokemones en base a procesar los datos en el hospital recibido.
 *       En caso de recibir un hospital o datos inválido, o algún error al crear algún pokemon,
 *       devuelve FALSE, caso contrario TRUE.
 */
bool hospital_guardar_pokemones(hospital_t* hospital, char** datos, size_t entrenador_id) {
    if (!hospital) return false;
    if (!datos) return false;

    size_t i = 2;

    while (datos[i] && datos[i + 1]) {
        pokemon_t* vector_pokemon_aux = hospital->vector_pokemones;
        hospital->vector_pokemones = realloc(hospital->vector_pokemones, sizeof(pokemon_t) * (hospital->cantidad_pokemon + 1));

        if (!hospital->vector_pokemones) {
            free(vector_pokemon_aux);
            return false;
        }

        hospital->vector_pokemones[hospital->cantidad_pokemon].nombre = (char*) calloc(strlen(datos[i]) + 1, sizeof(char));

        if (!hospital->vector_pokemones[hospital->cantidad_pokemon].nombre) {
            return false;
        }

        strcpy(hospital->vector_pokemones[hospital->cantidad_pokemon].nombre, datos[i]);

        hospital->vector_pokemones[hospital->cantidad_pokemon].nivel = (size_t) atoi(datos[i + 1]);
        hospital->vector_pokemones[hospital->cantidad_pokemon].entrenador_id = entrenador_id;

        hospital->cantidad_pokemon++;

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

    entrenador_t* vector_entrenadores_aux = hospital->vector_entrenadores;
    hospital->vector_entrenadores = realloc(hospital->vector_entrenadores, sizeof(entrenador_t) * (hospital->cantidad_entrenadores + 1));

    if (!hospital->vector_entrenadores) {
        free(vector_entrenadores_aux);
        return false;
    }

    hospital->vector_entrenadores[hospital->cantidad_entrenadores].id = (size_t) atoi(datos[0]);
    hospital->vector_entrenadores[hospital->cantidad_entrenadores].nombre = (char*) calloc(strlen(datos[1]) + 1, sizeof(char));

    if (!hospital->vector_entrenadores[hospital->cantidad_entrenadores].nombre) {
        return false;
    }

    strcpy(hospital->vector_entrenadores[hospital->cantidad_entrenadores].nombre, datos[1]);
    
    hospital->cantidad_entrenadores++;

    if (!hospital_guardar_pokemones(hospital, datos, (size_t) atoi(datos[0]))) return false;

    return true;
}

/*
 * Pre: -
 * Post: Duplicará exactamente el hospital recibido, devolviendo un nuevo puntero.
 */
hospital_t* duplicar_hospital(hospital_t* hospital) {
    hospital_t* nuevo_hospital = hospital_crear();

    if (!nuevo_hospital) return NULL;

    nuevo_hospital->cantidad_pokemon = hospital->cantidad_pokemon;
    for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
        nuevo_hospital->vector_pokemones = realloc(nuevo_hospital->vector_pokemones, sizeof(pokemon_t) * (i + 1));

        if (!nuevo_hospital->vector_pokemones) {
            return NULL;
        }

        nuevo_hospital->vector_pokemones[i].nombre = (char*) calloc(strlen(hospital->vector_pokemones[i].nombre) + 1, sizeof(char));

        if (!nuevo_hospital->vector_pokemones[i].nombre) {
            free(nuevo_hospital->vector_pokemones);
            return NULL;
        }

        strcpy(nuevo_hospital->vector_pokemones[i].nombre, hospital->vector_pokemones[i].nombre);
        nuevo_hospital->vector_pokemones[i].nivel = hospital->vector_pokemones[i].nivel;
        nuevo_hospital->vector_pokemones[i].entrenador_id = hospital->vector_pokemones[i].entrenador_id;
    }
    

    nuevo_hospital->cantidad_entrenadores = hospital->cantidad_entrenadores;
    for (size_t i = 0; i < hospital->cantidad_entrenadores; i++) {
        nuevo_hospital->vector_entrenadores = realloc(nuevo_hospital->vector_entrenadores, sizeof(pokemon_t) * (i + 1));

        if (!nuevo_hospital->vector_entrenadores) {
            free(nuevo_hospital->vector_pokemones);
            return NULL;
        }

        nuevo_hospital->vector_entrenadores[i].id = hospital->vector_entrenadores[i].id;
        nuevo_hospital->vector_entrenadores[i].nombre = (char*) calloc(strlen(hospital->vector_entrenadores[i].nombre) + 1, sizeof(char));

        if (!nuevo_hospital->vector_entrenadores[i].nombre) {
            free(nuevo_hospital->vector_pokemones);
            free(nuevo_hospital->vector_entrenadores);
            return NULL;
        }

        strcpy(nuevo_hospital->vector_entrenadores[i].nombre, hospital->vector_entrenadores[i].nombre);
    }

    return nuevo_hospital;
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

/*
 * Pre: -
 * Post: Leerá el archivo recibido, procesará la información contenida en él para crear
 *       y guardar los datos de entrenadores y pokemones dentro del hospital recibido.
 *       Si hospital es inválido, falla la apertura del archivo, el procesamiento del contenido
 *       del archivo o la creación de memoria en las creaciones de entrenadores y/o pokemones,
 *       devolverá FALSE y hospital permanecerá igual, en caso contrario TRUE.
 */
bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo) {
    if (!hospital) return false;

    FILE* archivo = fopen(nombre_archivo, "r");

    if (!archivo) return false;

    hospital_t* hospital_aux = duplicar_hospital(hospital);

    if (!hospital_aux) {
        fclose(archivo);
        return false;
    }

    char* linea = NULL;
    int leidos = leer_linea_archivo(&linea, archivo);

    while (leidos && leidos != ERROR) {
        char** datos = split(linea, ';');

        free(linea);
        linea = NULL;

        if (!datos || !actualizar_hospital(hospital, datos)) {
            fclose(archivo);
            destruir_split(datos);
            hospital_destruir(hospital);
            hospital = hospital_aux;
            return false;
        };

        destruir_split(datos);
        leidos = leer_linea_archivo(&linea, archivo);
    }

    fclose(archivo);

    if (leidos == ERROR) {
        hospital_destruir(hospital);
        hospital = hospital_aux;
        return false;
    }

    hospital_destruir(hospital_aux);

    return true;
}

/*
 * Pre: -
 * Post: Devuelve la cantidad de pokemones contenidos en el hospital recibido.
 *       En caso reciba un hospital inválido (no creado e inicializado), devuelve INVALIDO.
 */
size_t hospital_cantidad_pokemon(hospital_t* hospital) {
    if (!hospital) return INVALIDO;

    return hospital->cantidad_pokemon;
}

/*
 * Pre: -
 * Post: Devuelve la cantidad de entrenadores contenidos en el hospital recibido.
 *       En caso reciba un hospital inválido (no creado e inicializado), devuelve INVALIDO.
 */
size_t hospital_cantidad_entrenadores(hospital_t* hospital) {
    if (!hospital) return INVALIDO;

    return hospital->cantidad_entrenadores;
}

/*
 * Pre: -
 * Post: Ordenará los pokemones recibidos en orden alfabético, actualizando el resultado en el mismo puntero recibido.
 *       En caso de recibir un vector de pokemones inválido, devuelve FALSE, caso contrario TRUE.
 */
bool ordenar_pokemones(pokemon_t* pokemones, size_t cantidad_pokemones) {
    if (!pokemones) return false;

    for (size_t i = 0; i < cantidad_pokemones - 1; i++) {
        for (size_t j = 0; j < cantidad_pokemones - i - 1; j++) {
            if (strcmp(pokemones[j].nombre, pokemones[j + 1].nombre) > 0) {
                pokemon_t pokemon_aux = pokemones[j];
                pokemones[j] = pokemones[j + 1];
                pokemones[j + 1] = pokemon_aux;
            }
        }
    }

    return true;
}

/*
 * Pre: -
 * Post: Procesará los pokemones dentro del hospital recibido con la función recibida,
 *       ordenados alfabéticamente desde el inicio hasta que la respuesta de la función
 *       sea FALSE o se termine de recorrer el vector (Es decir, si es TRUE seguirá recorriendo).
 *       En caso de recibir un hospital o función inválida u ocurrir un problema al ordenar los pokemones,
 *       devolverá INVALIDO.
 */
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)) {
    if (!hospital) return INVALIDO;
    if (!funcion) return INVALIDO;

    size_t i = 0;
    bool continuar = true;

    if (!ordenar_pokemones(hospital->vector_pokemones, hospital->cantidad_pokemon)) return INVALIDO;

    while(i < hospital->cantidad_pokemon && continuar) {
        continuar = funcion(&hospital->vector_pokemones[i]);
        i++;
    }

    return i;
}

/*
 * Pre: -
 * Post: Destruirá toda el contenido dinámico del hospital recibido:
 *       pokemones, entrenadores, así como también la misma instancia del hospital.
 *       En caso de recibir un hospital inválido, no hace nada.
 */
void hospital_destruir(hospital_t* hospital) {
    if (!hospital) return;

    if (hospital->vector_entrenadores) {
        for (size_t i = 0; i < hospital->cantidad_entrenadores; i++) {
            free(hospital->vector_entrenadores[i].nombre);
        }
        free(hospital->vector_entrenadores);
    }

    if (hospital->vector_pokemones) {
        for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
            free(hospital->vector_pokemones[i].nombre);
        }
        free(hospital->vector_pokemones);
    }

    free(hospital);
}

/*
 * Pre: -
 * Post: Obtiene el nivel del pokemon recibido.
 *       En caso reciba un pokemon inválido, devuelve INVALIDO, caso contrario devuelve el nivel correspondiente.
 */
size_t pokemon_nivel(pokemon_t* pokemon) {
    if (!pokemon) return INVALIDO;

    return pokemon->nivel;
}

/*
 * Pre: -
 * Post: Obtiene el nombre del pokemon recibido.
 *       En caso reciba un pokemon inválido, devuelve NULL, caso contrario devuelve el nombre correspondiente.
 */
const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon) return NULL;

    return pokemon->nombre;
}
