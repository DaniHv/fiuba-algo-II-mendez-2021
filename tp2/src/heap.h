#ifndef __HEAP__H__
#define __HEAP__H__

#include <stdbool.h>
#include <stdlib.h>

typedef int (*heap_comparador_elementos)(void*, void*);

typedef struct heap {
  void** contenido;
  size_t tamanio;
  size_t capacidad;
  heap_comparador_elementos comparador;
} heap_t;

heap_t* heap_crear(size_t capacidad, heap_comparador_elementos comparador);

void* heap_extraer_raiz(heap_t* heap);

bool heap_insertar(heap_t* heap, void* elemento);

size_t heap_tamanio(heap_t* heap);

void heap_destruir(heap_t *heap);

#endif /* __HEAP__H__ */
