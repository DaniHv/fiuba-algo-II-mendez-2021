#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

lista_t* lista_crear() {
    lista_t* lista = malloc(sizeof(lista_t));

    if (!lista) return NULL;

    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = 0;

    return lista;
}

lista_t* lista_insertar(lista_t* lista, void* elemento) {
    if (!lista) return NULL;

    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->elemento = elemento;
    nodo->siguiente = NULL;

    if (lista->cantidad == 0) {
        lista->nodo_inicio = lista->nodo_fin = nodo;
    } else {
        lista->nodo_fin->siguiente = nodo;
        lista->nodo_fin = nodo;
    }

    lista->cantidad++;

    return lista;
}

/*
 * Pre: Recibe la posición a buscar, donde 0 es la posición inicial.
 * Post: Busca el nodo de la posición solicitada.
 *       Si recibe una lista inválida, o vacía, o no tiene dicha posición,
 *       devuelve NULL, caso contrario el nodo.
 */
nodo_t* obtener_nodo_en_posicion(lista_t* lista, size_t posicion) {
    if (!lista) return NULL;
    if (posicion >= lista->cantidad) return NULL;

    nodo_t* nodo_actual = lista->nodo_inicio;
    if (!nodo_actual) return NULL;

    for (size_t i = 1; i < posicion + 1; i++) {
        nodo_actual = nodo_actual->siguiente;
    }

    return nodo_actual;
}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {
    if (!lista) return NULL;

    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo) return NULL;

    nuevo_nodo->elemento = elemento;

    if (lista->cantidad == 0) {
        nuevo_nodo->siguiente = NULL;
        lista->nodo_fin = lista->nodo_inicio = nuevo_nodo;
    } else if (posicion == 0) {
        nuevo_nodo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo_nodo;
    } else if (posicion >= lista->cantidad) {
        nuevo_nodo->siguiente = NULL;
        lista->nodo_fin->siguiente = nuevo_nodo;
        lista->nodo_fin = nuevo_nodo;
    } else {
        nodo_t* nodo_anterior = obtener_nodo_en_posicion(lista, posicion - 1);
        nodo_t* nodo_a_mover = nodo_anterior->siguiente;

        nodo_anterior->siguiente = nuevo_nodo;
        nuevo_nodo->siguiente = nodo_a_mover;
    }

    lista->cantidad++;

    return lista;
}

void* lista_quitar(lista_t* lista) {
    if (!lista) return NULL;
    if (lista->cantidad == 0) return NULL;

    void* elemento = lista->nodo_fin->elemento;
    nodo_t* nodo_actual = lista->nodo_inicio;

    if (lista->cantidad == 1) {
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
        free(nodo_actual);
    } else {
        for (size_t i = 1; i < lista->cantidad - 1; i++) {
            nodo_actual = nodo_actual->siguiente;
        }

        free(lista->nodo_fin);
        lista->nodo_fin = nodo_actual;
        nodo_actual->siguiente = NULL;
    }

    lista->cantidad--;

    return elemento;
}

void* lista_quitar_de_posicion(lista_t* lista, size_t posicion) {
    if (!lista) return NULL;
    if (lista->cantidad == 0) return NULL;

    nodo_t* nodo_a_quitar = NULL;

    if (posicion == 0 || lista->cantidad == 1) {
        nodo_a_quitar = lista->nodo_inicio;

        lista->nodo_inicio = nodo_a_quitar->siguiente;

        if (lista->cantidad == 1) {
            lista->nodo_fin = NULL;
        }
    } else {
        nodo_t* nodo_anterior = obtener_nodo_en_posicion(lista, (posicion >= lista->cantidad ? lista->cantidad - 2 : posicion - 1));

        nodo_a_quitar = nodo_anterior->siguiente;

        nodo_anterior->siguiente = nodo_a_quitar->siguiente;

        if (posicion >= lista->cantidad) {
            lista->nodo_fin = nodo_anterior;
        }
    }

    void* elemento = nodo_a_quitar->elemento;
    free(nodo_a_quitar);
    lista->cantidad--;

    return elemento;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {
    if (!lista || lista->cantidad < posicion) return NULL;

    if (posicion >= lista->cantidad) return lista->nodo_fin->elemento;

    nodo_t* nodo = obtener_nodo_en_posicion(lista, posicion);
    
    return nodo->elemento;
}

void* lista_primero(lista_t* lista) {
    if (!lista) return NULL;
    if (!lista->nodo_inicio) return NULL;

    return lista->nodo_inicio->elemento;
}

void* lista_ultimo(lista_t* lista) {
    if (!lista) return NULL;
    if (!lista->nodo_fin) return NULL;

    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista) {
    if (!lista) return true;

    return (lista->cantidad == 0);
}

size_t lista_tamanio(lista_t* lista) {
    if (!lista) return 0;

    return lista->cantidad;
}

void lista_destruir(lista_t* lista) {
    if (!lista) return;

    nodo_t* nodo_actual = lista->nodo_inicio;

    while (nodo_actual) {
        nodo_t* nodo_aux = nodo_actual;
        nodo_actual = nodo_actual->siguiente;
        free(nodo_aux);
    }

    free(lista);
    lista = NULL;
}

lista_iterador_t* lista_iterador_crear(lista_t* lista) {
    if (!lista) return NULL;

    lista_iterador_t* lista_iterador = malloc(sizeof(lista_iterador_t));

    if (!lista_iterador) return NULL;

    lista_iterador->lista = lista;
    lista_iterador->corriente = lista->nodo_inicio;

    return lista_iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
    if (!iterador) return false;

    return (iterador->corriente != NULL);
}

bool lista_iterador_avanzar(lista_iterador_t* iterador) {
    if (!iterador) return false;
    if (!iterador->corriente) return false;

    iterador->corriente = iterador->corriente->siguiente;
    return (iterador->corriente != NULL);
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
    if (!iterador) return NULL;
    if (!iterador->corriente) return NULL;

    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador) {
    if (!iterador) return;

    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto) {
    if (!lista) return 0;
    if (lista->cantidad == 0) return 0;
    if (!funcion) return 0;

    size_t i = 0;
    bool continuar = true;
    nodo_t* nodo_actual = lista->nodo_inicio;

    while (continuar && i < lista->cantidad) {
        continuar = funcion(nodo_actual->elemento, contexto);
        nodo_actual = nodo_actual->siguiente;
        i++;
    }

    return i;
}
