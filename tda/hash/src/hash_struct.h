#ifndef __HASH_STRUCT_H__
#define __HASH_STRUCT_H__

#include "hash.h"
#include "lista.h"

struct hash {
  size_t cantidad_elementos;
  lista_t** tabla;
  size_t tamanio_tabla;
  hash_destruir_dato_t destructor_de_elemento;
};

#endif /* __HASH_H__ */
