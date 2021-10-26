#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa2mm.h"

#define TEXTO_PRUEBAS "DANIHV TDA LISTA"
#define CANTIDAD_ELEMENTOS 128

int main() {
  pa2m_nuevo_grupo("Pruebas con una lista vacía");
  lista_t* lista = lista_crear();
  pa2m_afirmar((lista != NULL), "Puedo crear una lista");
  pa2m_afirmar((lista_primero(lista) == NULL), "La lista no tiene inicio");
  pa2m_afirmar((lista_ultimo(lista) == NULL), "La lista no tiene fin");
  pa2m_afirmar(lista_tamanio(lista) == 0, "La lista tiene 0 elementos");
  pa2m_afirmar(lista_vacia(lista), "La lista está vacía");

  pa2m_nuevo_grupo("Insertar elementos al final");
  const char texto_prueba[] = TEXTO_PRUEBAS;
  pa2m_afirmar((lista = lista_insertar(lista, (void*) &texto_prueba)) != NULL, "Puedo añadir un elemento a la lista");
  pa2m_afirmar(!lista_vacia(lista), "Ahora la lista no está vacía");
  pa2m_afirmar((lista_tamanio(lista) == 1), "Ahora la lista tiene un elemento");
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 0), texto_prueba) == 0), "El elemento de la lista coincide con el creado");
  pa2m_afirmar((lista_primero(lista) != NULL), "La lista tiene inicio");
  pa2m_afirmar((lista_ultimo(lista) != NULL), "La lista tiene fin");

  for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
    lista = lista_insertar(lista, (void*) &texto_prueba);
  }

  pa2m_afirmar((lista != NULL), "Puedo insertar múltiples elementos a la lista");
  pa2m_afirmar((lista_tamanio(lista) == (CANTIDAD_ELEMENTOS + 1)), "La cantidad de elementos de la lista coincide con los añadidos");

  pa2m_nuevo_grupo("Quitar elementos del final");
  pa2m_afirmar((strcmp(lista_quitar(lista), texto_prueba) == 0), "Al quitar un nodo, devuelve el elemento correctamente");
  pa2m_afirmar((lista_tamanio(lista) == CANTIDAD_ELEMENTOS), "La lista se achica al eliminar un elemento");

  for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
    lista_quitar(lista);
  }
  pa2m_afirmar((lista != NULL), "Puedo quitar todos los elementos de la lista");
  pa2m_afirmar((lista_tamanio(lista) == 0), "Ahora la lista tiene 0 elementos");
  pa2m_afirmar(lista_vacia(lista), "Ahora la lista está vacía");
  pa2m_afirmar((lista->nodo_inicio == NULL), "La lista no tiene inicio");
  pa2m_afirmar((lista->nodo_fin == NULL), "La lista no tiene fin");

  pa2m_nuevo_grupo("Insertar elementos en posiciones específicas");
  char a = 'a';
  char b = 'b';
  char c = 'c';
  char d = 'd';
  char z = 'z';
  lista = lista_insertar_en_posicion(lista, &c, 0);
  pa2m_afirmar((lista->cantidad == 1), "Puedo agregar un elemento en la posición 0, a una lista vacía.");
  pa2m_afirmar((lista->nodo_inicio != NULL), "Ahora la lista tiene inicio");
  pa2m_afirmar((lista->nodo_fin != NULL), "Ahora la lista tiene fin");
  pa2m_afirmar((lista->nodo_inicio == lista->nodo_fin), "El inicio y el fin coincide");
  pa2m_afirmar((strcmp(lista->nodo_inicio->elemento, &c) == 0), "El elemento dentro del nodo coincide con el guardado");
  lista = lista_insertar_en_posicion(lista, &a, 0);
  pa2m_afirmar((lista->cantidad == 2), "Puedo agregar otro elemento en la posición 0");
  pa2m_afirmar((lista->nodo_inicio != lista->nodo_fin), "Ahora el inicio no coincide con el fin");
  pa2m_afirmar((strcmp(lista->nodo_inicio->elemento, &a) == 0), "El primer elemento es el correcto");
  lista = lista_insertar_en_posicion(lista, &z, 100);
  pa2m_afirmar((lista->cantidad == 3), "Puedo agregar un elemento en una posición mayor a la cantidad");
  pa2m_afirmar((strcmp(lista->nodo_fin->elemento, &z) == 0), "El último elemento coincide con el agregado");
  lista = lista_insertar_en_posicion(lista, &b, 1);
  pa2m_afirmar((lista->cantidad == 4), "Puedo agregar un elemento en una posición existente.")
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 1), &b) == 0), "El elemento en esa posición coincide con el agregado");
  lista = lista_insertar_en_posicion(lista, &d, 3);
  pa2m_afirmar((lista->cantidad == 5), "Puedo agregar otro elemento en una posición existente.")
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 3), &d) == 0), "El elemento en esa posición coincide con el agregado");

  pa2m_nuevo_grupo("Eliminar elementos en posiciones específicas");
  lista_quitar_de_posicion(lista, 0);
  pa2m_afirmar((lista->cantidad == 4), "Puedo eliminar el primer elemento de la lista");
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 0), &b) == 0), "El primer elemento de la lista es correcto");
  pa2m_afirmar((strcmp(lista->nodo_inicio->elemento, &b) == 0), "El nodo inicial de la lista es correcto");
  lista_quitar_de_posicion(lista, 100);
  pa2m_afirmar((lista->cantidad == 3), "Puedo borrar un elemento de la lista en una posicion inexistente");
  pa2m_afirmar((strcmp(lista->nodo_fin->elemento, &d) == 0), "El último elemento de la lista es correcto");
  pa2m_afirmar((strcmp(lista->nodo_fin->elemento, lista_elemento_en_posicion(lista, 2)) == 0), "El nodo final coincide con el último elemento");
  lista_quitar_de_posicion(lista, 1);
  pa2m_afirmar((lista->cantidad == 2), "Puedo sacar un elemento entre otros dos");
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 0), &b) == 0), "El elemento anterior es correcto");
  pa2m_afirmar((strcmp(lista_elemento_en_posicion(lista, 1), &d) == 0), "El elemento siguiente es correcto");

  lista_destruir(lista);

  return pa2m_mostrar_reporte();
}
