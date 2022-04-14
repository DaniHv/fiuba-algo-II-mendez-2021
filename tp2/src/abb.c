#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct recorrido {
  void** array;
  size_t tamanio_array;
  size_t* contador;
} recorrido_t;

abb_t* abb_crear(abb_comparador comparador) {
  if (!comparador) return NULL;

  abb_t* arbol = malloc(sizeof(abb_t));
  if (!arbol) return NULL;

  arbol->nodo_raiz = NULL;
  arbol->comparador = comparador;
  arbol->tamanio = 0;

  return arbol;
}

static void abb_insertar_aux(nodo_abb_t* nodo_actual, nodo_abb_t* nodo_a_insertar, abb_comparador comparador) {
  if ((comparador(nodo_actual->elemento, nodo_a_insertar->elemento) >= 0) && nodo_actual->izquierda) {
    abb_insertar_aux(nodo_actual->izquierda, nodo_a_insertar, comparador);
    return;
  } else if ((comparador(nodo_actual->elemento, nodo_a_insertar->elemento) < 0) && nodo_actual->derecha) {
    abb_insertar_aux(nodo_actual->derecha, nodo_a_insertar, comparador);
    return;
  }

  if (comparador(nodo_actual->elemento, nodo_a_insertar->elemento) >= 0) {
    nodo_actual->izquierda = nodo_a_insertar;
  } else if (comparador(nodo_actual->elemento, nodo_a_insertar->elemento) < 0) {
    nodo_actual->derecha = nodo_a_insertar;
  }
}

abb_t* abb_insertar(abb_t* arbol, void* elemento) {
  if (!arbol) return NULL;

  nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
  if (!nodo) return NULL;

  nodo->elemento = elemento;
  nodo->izquierda = NULL;
  nodo->derecha = NULL;

  if (!arbol->nodo_raiz) {
    arbol->nodo_raiz = nodo;
  } else {
    abb_insertar_aux(arbol->nodo_raiz, nodo, arbol->comparador);
  }

  arbol->tamanio++;

  return arbol;
}

/*
 * Pre: Recibe el nodo padre, el cual efectivamente debe tener un hijo izquierdo.
 * Post: Se elimina el hizo izquierdo del nodo padre recibido, siendo reemplazado
 *       por el predecesor inorder del mismo (si tiene).
 */
void* abb_quitar_izquierdo(nodo_abb_t* nodo_padre) {
  nodo_abb_t* nodo_a_quitar = nodo_padre->izquierda;
  void* elemento = nodo_a_quitar->elemento;

  if (nodo_a_quitar->izquierda) {
    nodo_abb_t* nodo_a_mover = nodo_a_quitar->izquierda;

    if (nodo_a_mover->derecha) {
      nodo_abb_t* nodo_actual = nodo_a_mover;

      while(nodo_actual->derecha->derecha) {
        nodo_actual = nodo_actual->derecha;
      }

      nodo_a_mover = nodo_actual->derecha;
      nodo_actual->derecha = nodo_a_mover->izquierda;
      nodo_a_mover->izquierda = nodo_a_quitar->izquierda;
    }

    nodo_a_mover->derecha = nodo_a_quitar->derecha;
    nodo_padre->izquierda = nodo_a_mover;
  } else {
    nodo_padre->izquierda = nodo_a_quitar->derecha;
  }

  free(nodo_a_quitar);

  return elemento;
}

/*
 * Pre: Recibe el nodo padre, el cual efectivamente debe tener un hijo derecho.
 * Post: Se elimina el hijo derecho del nodo padre recibido, siendo reemplazado
 *       por el predecesor inorder del mismo (si tiene).
 */
void* abb_quitar_derecho(nodo_abb_t* nodo_padre) {
  nodo_abb_t* nodo_a_quitar = nodo_padre->derecha;
  void* elemento = nodo_a_quitar->elemento;

  if (nodo_a_quitar->izquierda) {
    nodo_abb_t* nodo_a_mover = nodo_a_quitar->izquierda;

    if (nodo_a_mover->derecha) {
      nodo_abb_t* nodo_actual = nodo_a_mover;

      while(nodo_actual->derecha->derecha) {
        nodo_actual = nodo_actual->derecha;
      }

      nodo_a_mover = nodo_actual->derecha;
      nodo_actual->derecha = nodo_a_mover->izquierda;
      nodo_a_mover->izquierda = nodo_a_quitar->izquierda;
    }
    
    nodo_a_mover->derecha = nodo_a_quitar->derecha;
    nodo_padre->derecha = nodo_a_mover;
  } else {
    nodo_padre->derecha = nodo_a_quitar->derecha;
  }

  free(nodo_a_quitar);

  return elemento;
}

/*
 * Pre: Debe recibir un nodo_actual, un elemento y un comparador válido.
 * Post: Buscará el nodo con el elemento solicitado y lo eliminará devolviendo el elemento contenido.
 *       En caso de no encontrar un nodo con dicho elemento, devuelve NULL
 */
void* abb_quitar_aux(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador) {
  if ((comparador(nodo_actual->elemento, elemento) > 0) && nodo_actual->izquierda) {
    if ((comparador(nodo_actual->izquierda->elemento, elemento) == 0)) {
      return abb_quitar_izquierdo(nodo_actual);
    } else {
      return abb_quitar_aux(nodo_actual->izquierda, elemento, comparador);
    }
  } else if ((comparador(nodo_actual->elemento, elemento) < 0) && nodo_actual->derecha) {
    if ((comparador(nodo_actual->derecha->elemento, elemento) == 0)) {
      return abb_quitar_derecho(nodo_actual);
    } else {
      return abb_quitar_aux(nodo_actual->derecha, elemento, comparador);
    }
  }

  return NULL;
}

/*
 * Pre: Debe recibir un arbol válido con al menos un elemento para ser eliminado.
 * Post: Eliminará la raíz actual reemplazandola (si tiene más de un elemento), por el predecesor
 *       inorder en caso de tenerlo, en caso contrario lo reemplaza con su único hijo, por lógica, derecho.
 */
void* abb_quitar_raiz(abb_t* arbol) {
  nodo_abb_t* raiz = arbol->nodo_raiz;
  void* elemento = raiz->elemento;

  if (arbol->tamanio == 1) {
    free(arbol->nodo_raiz);
    arbol->nodo_raiz = NULL;
  } else if (raiz->izquierda) {
    nodo_abb_t* nueva_raiz = raiz->izquierda;

    if (nueva_raiz->derecha) {
      nodo_abb_t* nodo_actual = nueva_raiz;

      while (nodo_actual->derecha->derecha) {
        nodo_actual = nodo_actual->derecha;
      }

      nueva_raiz = nodo_actual->derecha;
      nodo_actual->derecha = nodo_actual->derecha->izquierda;
      nueva_raiz->izquierda = raiz->izquierda;
    }

    nueva_raiz->derecha = raiz->derecha;

    free(arbol->nodo_raiz);
    arbol->nodo_raiz = nueva_raiz;
  } else if (raiz->derecha) {
    nodo_abb_t* nueva_raiz = raiz->derecha;

    free(arbol->nodo_raiz);
    arbol->nodo_raiz = nueva_raiz;
  }

  return elemento;
}

void* abb_quitar(abb_t* arbol, void *elemento) {
  if (!arbol) return NULL;
  if (arbol->tamanio == 0) return NULL;

  void* elemento_quitado = NULL;

  if (arbol->comparador(arbol->nodo_raiz->elemento, elemento) == 0) {
    elemento_quitado = abb_quitar_raiz(arbol);
  } else {
    elemento_quitado = abb_quitar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
  }

  if (elemento_quitado) {
    arbol->tamanio--;
  }

  return elemento_quitado;
}

/*
 * Pre: Debe recibir un elemento y comparador válido.
 * Post: Buscará el nodo que contiene el elemento solicitado y devolverá el elemento en caso de encontrarlo
 *       caso contrario devuelve NULL
 */
void* abb_buscar_aux(nodo_abb_t* nodo, void* elemento, abb_comparador comparador) {
  if (!nodo) return NULL;

  if ((comparador(nodo->elemento, elemento) > 0) && nodo->izquierda) {
    return abb_buscar_aux(nodo->izquierda, elemento, comparador);
  } else if ((comparador(nodo->elemento, elemento) < 0) && nodo->derecha)  {
    return abb_buscar_aux(nodo->derecha, elemento, comparador);
  } else if (comparador(nodo->elemento, elemento) == 0) {
    return nodo->elemento;
  }

  return NULL;
}

void* abb_buscar(abb_t* arbol, void* elemento) {
  if (!arbol || !arbol->nodo_raiz) return NULL;

  return abb_buscar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t* arbol) {
  if (!arbol) return true;

  return (arbol->tamanio == 0);
}

size_t abb_tamanio(abb_t *arbol) {
  if (!arbol) return 0;

  return arbol->tamanio;
}

/*
 * Pre: Debe recibir un nodo_actual válido.
 * Post: Recorrerá todo los hijos del nodo_actual eliminando cada uno de ellos,
 *       y ejecutando (de existir) la función destructora recibida.
 */
static void destruir_nodos(nodo_abb_t* nodo_actual, void (*destructor)(void *)) {
  if (nodo_actual->izquierda) {
    destruir_nodos(nodo_actual->izquierda, destructor);
  }

  if (nodo_actual->derecha) {
    destruir_nodos(nodo_actual->derecha, destructor);
  }

  if (destructor) {
    destructor(nodo_actual->elemento);
  }

  free(nodo_actual);
}

void abb_destruir(abb_t *arbol) {
  if (!arbol) return;

  if (arbol->nodo_raiz) {
    destruir_nodos(arbol->nodo_raiz, NULL);
  }

  free(arbol);
  arbol = NULL;
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)) {
  if (!arbol) return;

  if (arbol->nodo_raiz) {
    destruir_nodos(arbol->nodo_raiz, destructor);
  }

  free(arbol);
  arbol = NULL;
}

/*
 * Pre: Debe recibir una función, puntero a cantidad y continuar válidos.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera INORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
void recorrer_abb_inorder_aux(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* cantidad, bool* continuar) {
  if (!nodo || !(*continuar)) return;

  if (nodo->izquierda) {
    recorrer_abb_inorder_aux(nodo->izquierda, funcion, aux, cantidad, continuar);
  }

  if (!(*continuar)) return;

  (*continuar) = funcion(nodo->elemento, aux);
  (*cantidad)++;

  if (nodo->derecha) {
    recorrer_abb_inorder_aux(nodo->derecha, funcion, aux, cantidad, continuar);
  }
}

/*
 * Pre: Debe recibir un nodo y función válida.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera INORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
size_t recorrer_abb_inorder(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux) {
  size_t cantidad = 0;
  bool continuar = true;

  recorrer_abb_inorder_aux(nodo, funcion, aux, &cantidad, &continuar);

  return cantidad;
}

/*
 * Pre: Debe recibir una función, puntero a cantidad y continuar válidos.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera POSTORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
void recorrer_abb_postorder_aux(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* cantidad, bool* continuar) {
  if (!nodo || !(*continuar)) return;

  if (nodo->izquierda) {
    recorrer_abb_postorder_aux(nodo->izquierda, funcion, aux, cantidad, continuar);
  }

  if (nodo->derecha) {
    recorrer_abb_postorder_aux(nodo->derecha, funcion, aux, cantidad, continuar);
  }

  if (!(*continuar)) return;

  (*continuar) = funcion(nodo->elemento, aux);
  (*cantidad)++;
}

/*
 * Pre: Debe recibir un nodo y función válida.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera POSTORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
size_t recorrer_abb_postorder(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux) {
  size_t cantidad = 0;
  bool continuar = true;

  recorrer_abb_postorder_aux(nodo, funcion, aux, &cantidad, &continuar);

  return cantidad;
}

/*
 * Pre: Debe recibir una función, puntero a cantidad y continuar válidos.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera PREORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
void recorrer_abb_preorder_aux(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* cantidad, bool* continuar) {
  if (!nodo || !(*continuar)) return;

  (*continuar) = funcion(nodo->elemento, aux);
  (*cantidad)++;

  if (nodo->izquierda) {
    recorrer_abb_preorder_aux(nodo->izquierda, funcion, aux, cantidad, continuar);
  }

  if (nodo->derecha) {
    recorrer_abb_preorder_aux(nodo->derecha, funcion, aux, cantidad, continuar);
  }
}

/*
 * Pre: Debe recibir un nodo y función válida.
 * Post: Recorrerá los elementos descendientes del nodo recibido de manera PREORDER
 *       y ejecutando con ellos la función recibida hasta que esta devuelva false,
 *       o por el contrario, se terminen los hijos.
 */
size_t recorrer_abb_preorder(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux) {
  size_t cantidad = 0;
  bool continuar = true;

  recorrer_abb_preorder_aux(nodo, funcion, aux, &cantidad, &continuar);

  return cantidad;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux) {
  if (!arbol) return 0;
  if (!funcion) return 0;

  nodo_abb_t* nodo = arbol->nodo_raiz;
  if (!nodo) return 0;

  size_t contador = 0;
  if (recorrido == INORDEN) {
    contador = recorrer_abb_inorder(arbol->nodo_raiz, funcion, aux);
  } else if (recorrido == PREORDEN) {
    contador = recorrer_abb_preorder(arbol->nodo_raiz, funcion, aux);
  } else if (recorrido == POSTORDEN) {
    contador = recorrer_abb_postorder(arbol->nodo_raiz, funcion, aux);
  }

  return contador;
}

/*
 * Pre: El puntero auxiliar deberá ser un puntero a un dato de tipo recorrido_t válido.
 * Post: Guardará el elemento recibido dentro de la lista contenida en recorrido_t.
 *       Devuelve true si todavía queda espacio para más elementos, o false en caso contrario.
 */
bool guardar_elemento_abb(void* elemento, void* aux) {
  recorrido_t* recorrido = (recorrido_t*) aux;

  if (recorrido->tamanio_array > (*recorrido->contador)) {
    recorrido->array[(*recorrido->contador)] = elemento;
    (*recorrido->contador)++;

    if (recorrido->tamanio_array == (*recorrido->contador)) return false;
    return true;
  }

  return false;
}

size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array) {
  if (!arbol) return 0;
  if (!array) return 0;

  size_t contador = 0;
  recorrido_t datos = {
    .array = array,
    .tamanio_array = tamanio_array,
    .contador = &contador,
  };

  if (recorrido == INORDEN) {
    contador = recorrer_abb_inorder(arbol->nodo_raiz, &guardar_elemento_abb, &datos);
  } else if (recorrido == PREORDEN) {
    contador = recorrer_abb_preorder(arbol->nodo_raiz, &guardar_elemento_abb, &datos);
  } else if (recorrido == POSTORDEN) {
    contador = recorrer_abb_postorder(arbol->nodo_raiz, &guardar_elemento_abb, &datos);
  }

  return contador;
}
