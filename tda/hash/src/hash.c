#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "hash_struct.h"
#include "lista.h"

#define EXITO 0
#define ERROR -1
#define CAPACIDAD_INICIAL_MINIMA 3
#define MAX_FACTOR_DE_CARGA_REHASH 0.75

typedef struct dato {
    char* clave;
    void* elemento;
} dato_t;

/*
 * Struct como auxiliar que contiene la información necesaria para ser usada en transformador_de_recorrido,
 * con la intención de no reescribir una funcionalidad de recorrido de las listas de la tabla, aprovechando
 * la primitiva de iteración nativa de lista.
 */
typedef struct transformador_de_recorrido_aux {
    hash_t* hash;
    bool (*funcion_recorrido_hash)(hash_t* hash, const char* clave, void* aux);
    void* aux;
    bool continuar;
} transformador_de_recorrido_aux_t;

/*
 * Struct Auxiliar para ser usado en rehash_elemento,
 * conteniendo la información necesaria para él.
 * Es implementado con el objetivo de aprovechar el iterador
 * interno en el recorrido de las listas.
 */
typedef struct rehash_elemento_aux {
    lista_t** tabla;
    size_t tamanio;
} rehash_elemento_aux_t;

/*
 * Pre: Debe recibir una clave válida.
 * Post: Devuelve el valor hash correspondiente a la clave recibida,
 *       haciendo uso del cálculo en size_t del valor en ASCII de la primera
 *       letra + última letra, entre el largo total de la clave.
 */
size_t hashear_clave(const char* clave) {
    return clave[0] + clave[strlen(clave) - 1] / strlen(clave);
}

/*
 * Pre: Debe recibir una tabla de hash válida, el tamaño correspondiente a la misma,
 *      y una clave previamente hasheada.
 * Post: Devuelve la lista_t* correspondiente a la posición calculada por el hash recibido,
 *       en caso de no existir aún una lista en dicha posición, se creará y devolverá.
 */
lista_t* obtener_lista_de_hash(lista_t** tabla, size_t tamanio, size_t clave_hasheada) {
    size_t posicion = clave_hasheada % tamanio;

    if (!tabla[posicion]) {
        tabla[posicion] = lista_crear();
    }

    return tabla[posicion];
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial) {
    hash_t* hash = (hash_t*) malloc(sizeof(hash_t));
    if(!hash) return NULL;

    size_t capacidad = capacidad_inicial >= CAPACIDAD_INICIAL_MINIMA ? capacidad_inicial : CAPACIDAD_INICIAL_MINIMA;

    hash->tabla = (lista_t**) calloc(capacidad, sizeof(lista_t*));
    if(!hash->tabla) {
        free(hash);
        return NULL;
    }

    hash->destructor_de_elemento = destruir_elemento;
    hash->tamanio_tabla = capacidad;
    hash->cantidad_elementos = 0;

    return hash;
}

/*
 * Pre: El elemento recibido debe ser de tipo dato_t*, y el auxiliar de tipo hash_t*.
 * Post: Destruye el dato recibido, usando (de existir) el destructor del hash sobre el elemento.
 *       Es una implementación genérica con firma haciendo uso de void* para permitirle ser 
 *       usa en conjunto al iterador interno de las listas.
 *       En caso de no recibir elemento o aux, devuelve false, caso contrario true.
 */
bool destruir_dato(void* elemento, void* aux) {
    if (!elemento || !aux) return false;
    dato_t* dato = (dato_t*) elemento;
    hash_t* hash = (hash_t*) aux;

    if (hash->destructor_de_elemento) {
        hash->destructor_de_elemento(dato->elemento);
    }

    free(dato->clave);
    free(dato);

    return true;
}


/*
 * Pre: Los elementos dato1 y dato2, deben ser de tipo dato_t*.
 * Post: Compara la clave de ambos datos y devuelve true en caso de ser igual, false en caso contrario.
 *       Es una implementación genérica con firma haciendo uso de void* para permitirle ser 
 *       usa en conjunto al iterador interno de las listas.
 */
bool comparar_clave_datos(void* dato1, void* dato2) {
    if (!dato1 || !dato2) return false;

    return (strcmp(((dato_t*) dato1)->clave, ((dato_t*) dato2)->clave) == 0);
}

/*
 * Pre: El elemento recibido debe ser de tipo dato_t* y el aux de tipo rehash_elemento_aux*
 * Post: Tomará el dato recibido y lo insertará en la tabla recibida en el auxiliar.
 *       Devuelve false en caso de error, o true en caso contrario.
 */
bool rehash_elemento(void* elemento, void* aux) {
    if (!elemento || !aux) return false;

    dato_t* dato = (dato_t*) elemento;
    rehash_elemento_aux_t* rehash_aux = (rehash_elemento_aux_t*) aux;

    lista_t* lista = obtener_lista_de_hash(rehash_aux->tabla, rehash_aux->tamanio, hashear_clave(dato->clave));
    if (!lista) return false;

    lista_insertar(lista, elemento);

    return true;
}

/*
 * Pre: -
 * Post: Crea una nueva tabla de hash y mueve todos los elementos del hash actual
 *       a la nueva tabla, para posteriormente reemplazarla en el hash eliminando la anterior.
 */
int rehashear_tabla(hash_t* hash) {
    if (!hash) return ERROR;

    size_t nuevo_tamanio = hash->tamanio_tabla * 2;
    lista_t** nueva_tabla = calloc(nuevo_tamanio, sizeof(lista_t*));

    if (!nueva_tabla) return ERROR;

    rehash_elemento_aux_t aux = {
        .tabla = nueva_tabla,
        .tamanio = nuevo_tamanio,
    };

    size_t cantidad = 0;
    size_t i = 0;
    while (i < hash->tamanio_tabla) {
        if (hash->tabla[i]) {
            cantidad += lista_con_cada_elemento(hash->tabla[i], &rehash_elemento, (void*) &aux);
            lista_destruir(hash->tabla[i]);
        }

        i++;
    }

    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->tamanio_tabla = nuevo_tamanio;

    return EXITO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento) {
    if(!hash || !clave) return ERROR;

    lista_t* lista = obtener_lista_de_hash(hash->tabla, hash->tamanio_tabla, hashear_clave(clave));
    if (!lista) return ERROR;

    dato_t* dato = malloc(sizeof(dato_t));
    if (!dato) return ERROR;

    dato->clave = (char*) malloc(sizeof(char) * (strlen(clave) + 1));
    if (!dato->clave) {
        free(dato);
        return ERROR;
    }
    strcpy(dato->clave, clave);
    dato->elemento = elemento;

    void* dato_anterior = NULL;

    if(!lista_insertar_o_actualizar(lista, dato, &comparar_clave_datos, &dato_anterior)) {
        destruir_dato((void*) dato, (void*) hash);
        return ERROR;
    };

    if (dato_anterior) {
        destruir_dato((void*) dato_anterior, (void*) hash);
    } else {
        hash->cantidad_elementos++;
    }

    if((hash->cantidad_elementos / hash->tamanio_tabla) >= MAX_FACTOR_DE_CARGA_REHASH) rehashear_tabla(hash);

    return EXITO;
}

/*
 * Pre: elemento debe ser de tipo dato_t*, y aux debe ser de tipo char*.
 * Post: Comparará la clave contenida en el dato recibido, y la clave recibida en aux,
 *       Devuelve true en caso de ser iguales, o false en caso contrario o en caso de error.
 */
bool comparar_dato_clave(void* elemento, void* aux) {
    if (!elemento || !aux) return false;

    dato_t* dato = (dato_t*) elemento;
    char* clave = (char*) aux;

    return (strcmp(dato->clave, clave) == 0);
}

int hash_quitar(hash_t* hash, const char* clave) {
    if(!hash || !clave) return ERROR;

    lista_t* lista = obtener_lista_de_hash(hash->tabla, hash->tamanio_tabla, hashear_clave(clave));
    if(lista_vacia(lista)) return ERROR;

    dato_t* dato = (dato_t*) lista_quitar_especifico(lista, &comparar_dato_clave, (void*) clave);
    if(!dato) return ERROR;

    destruir_dato((void*) dato, (void*) hash);
    hash->cantidad_elementos--;

    return EXITO;
}

void* hash_obtener(hash_t* hash, const char* clave) {
    if(!hash || !clave) return NULL;

    lista_t* lista = obtener_lista_de_hash(hash->tabla, hash->tamanio_tabla, hashear_clave(clave));
    if(lista_vacia(lista)) return NULL;

    lista_iterador_t* iterador = lista_iterador_crear(lista);
    dato_t* dato_actual = (dato_t*) lista_iterador_elemento_actual(iterador);
    if(!dato_actual) {
        lista_iterador_destruir(iterador);
        return NULL;
    };

    bool hubo_error = false;
    while(lista_iterador_tiene_siguiente(iterador) && strcmp(dato_actual->clave, clave) != 0 && !hubo_error) {
        if(!lista_iterador_avanzar(iterador)) {
            hubo_error = true;
        } else {
            dato_actual = lista_iterador_elemento_actual(iterador);
        }
    }

    lista_iterador_destruir(iterador);

    if(hubo_error) return NULL;

    return (strcmp(dato_actual->clave, clave) == 0) ? dato_actual->elemento : NULL;
}

bool hash_contiene(hash_t* hash, const char* clave) {
    if(!hash || !clave) return false;

    return (!!hash_obtener(hash, clave));
}

size_t hash_cantidad(hash_t* hash) {
    if(!hash) return 0;

    return hash->cantidad_elementos;
}

void hash_destruir(hash_t* hash) {
    if(!hash) return;

    for (size_t i = 0; i < hash->tamanio_tabla; i++) {
        if (hash->tabla[i]) {
            lista_con_cada_elemento(hash->tabla[i], &destruir_dato, hash);
            lista_destruir(hash->tabla[i]);
        }
    }

    free(hash->tabla);
    free(hash);
}

/*
 * Pre: El elemento recibido debe ser de tipo dato_t*, y aux debe ser transformador_de_recorrido_aux_t*
 * Post: Transforma una función de recorrido de lista_con_cada_elemento para recorrer cada clave contenida
 *       dentro de los datos (contenedor con clave, elemento) ejecutando la funcion de recorrido de hash
 *       contenida dentro de aux.
 *       Devuelve el resultado de la ejecución de la funcion de recorrido hash con el hash, clave y aux del recorrido hash.
 */
bool transformador_de_recorrido(void* elemento, void* aux) {
    dato_t* dato = (dato_t*) elemento;
    transformador_de_recorrido_aux_t* aux_recorrido = (transformador_de_recorrido_aux_t*) aux;

    aux_recorrido->continuar = !(aux_recorrido->funcion_recorrido_hash(aux_recorrido->hash, dato->clave, aux_recorrido->aux));

    return aux_recorrido->continuar;
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux) {
    if(!hash || !funcion) return 0;

    size_t cantidad = 0;

    transformador_de_recorrido_aux_t recorrido_aux = {
        .hash = hash,
        .funcion_recorrido_hash = funcion,
        .aux = aux,
        .continuar = true,
    };

    size_t i = 0;
    while (i < hash->tamanio_tabla && recorrido_aux.continuar) {
        if (hash->tabla[i]) {
            cantidad += lista_con_cada_elemento(hash->tabla[i], &transformador_de_recorrido, &recorrido_aux);
        }

        i++;
    }

    return cantidad;
}
