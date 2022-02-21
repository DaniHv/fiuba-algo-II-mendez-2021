
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"


heap_t* heap_crear(size_t capacidad, heap_comparador_elementos comparador){
    if (!comparador) return NULL;

	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->contenido = calloc(capacidad, sizeof(void*));
    if (!heap->contenido) {
        free(heap);
        return NULL;
    }

	heap->capacidad = capacidad;
	heap->tamanio = 0;
	heap->comparador = comparador;

	return heap;
}

void swap(void** contenido, size_t pos1, size_t pos2) {
    void *temp = contenido[pos1];
    contenido[pos1] = contenido[pos2];
    contenido[pos2] = temp;
}

void sift_up(heap_t* heap, size_t pos_actual){
    if (!pos_actual) return;

    size_t pos_superior = (pos_actual - 1) / 2;
    if (heap->comparador(heap->contenido[pos_superior], heap->contenido[pos_actual])){
        swap(heap->contenido, pos_actual, pos_superior);
        sift_up(heap, pos_superior);
    }
}

void sift_down(heap_t* heap, size_t pos_actual) {
    if (!heap) return;

    size_t pos_inferior = 2 * pos_actual + 1;    
	if (pos_inferior >= heap->tamanio) return;

	if (pos_inferior + 1 < heap->tamanio && heap->comparador(heap->contenido[pos_inferior], heap->contenido[pos_inferior + 1])) {
		pos_inferior++;
    }
	
	if (heap->comparador(heap->contenido[pos_actual], heap->contenido[pos_inferior])){
		swap(heap->contenido, pos_inferior, pos_actual);
		sift_down(heap, pos_inferior);
	}
}

void* heap_extraer_raiz(heap_t *heap){
    if (!heap->tamanio) return NULL;

    void* raiz = heap->contenido[0];
    heap->contenido[0] = heap->contenido[heap->tamanio - 1];
    sift_down(heap, 0);
    heap->tamanio--;

    return raiz;
}


bool heap_insertar(heap_t *heap, void *elemento) {
	if (heap->tamanio == heap->capacidad) return false;

    heap->contenido[heap->tamanio] = elemento;
    sift_up(heap, heap->tamanio);
    heap->tamanio++;

    return true;
}

size_t heap_tamanio(heap_t* heap) {
    if (!heap) return 0;

    return heap->tamanio;
}

void heap_destruir(heap_t* heap) {
	free(heap->contenido);
	free(heap);
}
