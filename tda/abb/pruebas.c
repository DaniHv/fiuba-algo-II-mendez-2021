#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"

int comparador_int(void* num1, void* num2) {
	int valor1 = *((int*) num1);
	int valor2 = *((int*) num2);

	if (valor1 == valor2) return 0;
	return (valor1 < valor2) ? -1 : 1;
}

bool recorrer_mientras_sea_menor(void* elemento, void* aux) {
	int valor = *((int*) elemento);
	int limite = *((int*) aux);
	return valor < limite;
}

void destructor(void *elemento) {
	free(elemento);
}

abb_t* crear_abb_con_elementos(int elementos[], int cantidad_elementos) {
	abb_t* arbol = abb_crear(comparador_int);

	for(size_t i = 0; i < cantidad_elementos ; i++){
		abb_insertar(arbol, &elementos[i]);
	}

	return arbol;
}

void pruebas_null() {
	pa2m_nuevo_grupo("Pruebas con NULL");

	char elemento[] = "x";
	abb_t *arbol = abb_crear(comparador_int);
	pa2m_afirmar(!abb_insertar(NULL, &elemento), "Insertar en árbol NULL devuelve NULL");
	pa2m_afirmar(!abb_tamanio(NULL), "El tamaño de árbol NULL es 0");
	pa2m_afirmar(!abb_quitar(NULL, &elemento), "Quitar elemento de árbol NULL devuelve NULL");
	pa2m_afirmar(abb_vacio(NULL), "Un árbol NULL está vacío");
	pa2m_afirmar(!abb_buscar(NULL, &elemento), "Buscar en árbol NULL devuelve NULL");
	pa2m_afirmar(!abb_recorrer(NULL, INORDEN, NULL, 100), "Recorrer en árbol NULL devuelve 0");
	pa2m_afirmar(!abb_con_cada_elemento(NULL, INORDEN, recorrer_mientras_sea_menor, NULL), "Recorrer con una función un árbol NULL devuelve 0");
	pa2m_afirmar(!abb_con_cada_elemento(arbol, INORDEN, NULL, NULL),  "Recorrer con función NULL un árbol válido devuelve 0");
	abb_destruir(NULL);
	abb_destruir(arbol);
}

void pruebas_creacion() {
	pa2m_nuevo_grupo("Pruebas de creación");
	abb_t *arbol;
	int elemento = 4;
	pa2m_afirmar((arbol = abb_crear(comparador_int)), "Puedo crear un árbol con un comparador válido");
	pa2m_afirmar(abb_vacio(arbol), "El árbol está vacío");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "El árbol tiene 0 elementos");
	pa2m_afirmar(!abb_quitar(arbol, &elemento), "Quitar elemento de árbol vacío devuelve NULL");
	pa2m_afirmar(!abb_buscar(arbol, &elemento), "Buscar elemento de árbol vacío devuelve NULL");
	pa2m_afirmar(!abb_recorrer(arbol, INORDEN, NULL, 100), "Recorrer árbol vacío devuelve 0 elementos recorridos");
	abb_destruir(arbol);
}

void pruebas_insercion() {
	pa2m_nuevo_grupo("Pruebas de inserción");
	int elementos[8] = { 10, 9, 15, 7, 8, 4, 10, 16 };
	abb_t* arbol = crear_abb_con_elementos(elementos, 8);

	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->elemento, &elementos[0]) == 0, "Se insertó la raiz correctamente");
	pa2m_afirmar(!abb_vacio(arbol), "El árbol ya no está vacío")
	pa2m_afirmar(abb_tamanio(arbol) == 8, "Se insertó la cantidad correcta de elementos");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->izquierda->elemento, &elementos[1]) == 0, "El elemento menor está en la posición correcta");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->derecha->elemento, &elementos[2]) == 0, "El elemento mayor está en la posicón correcta");
	
	pa2m_afirmar(abb_tamanio(arbol)==8, "Se insertaron todos los elementos");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->derecha->derecha->elemento, &elementos[7]) == 0, "Puedo insertar sucesivas veces en una dirección");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->izquierda->derecha->elemento, &elementos[6]) == 0, "Puedo insertar elementos iguales y quedan en la posición correcta");
	pa2m_afirmar(arbol->comparador(abb_buscar(arbol, &elementos[5]), &elementos[5]) == 0, "Puedo insertar con varias redirecciones de por medio en la inserción");
	pa2m_afirmar(arbol->comparador(abb_buscar(arbol, &elementos[6]), &elementos[6]) == 0, "Puedo buscar y encontrar un elemento profundo");
	abb_destruir(arbol);
}


void pruebas_eliminacion() {
	pa2m_nuevo_grupo("Pruebas de eliminación");
	abb_t *arbol = abb_crear(comparador_int);
	int elemento = 1;
	abb_insertar(arbol, &elemento);
	abb_quitar(arbol, &elemento);
	pa2m_afirmar(abb_vacio(arbol), "Quitar un elemento de un árbol de un elemento lo deja vacío");
	pa2m_afirmar(!abb_tamanio(arbol), "Quitar un elemento de un árbol de un elemento lo deja con tamaño 0");
	pa2m_afirmar(!arbol->nodo_raiz, "Ahora el arbol tiene raíz NULL");

	int elementos[8] = { 11, 9, 15, 7, 8, 4, 10, 16 };
	for(int i = 0; i < 8 ; i++){
		abb_insertar(arbol, &elementos[i]);
	}

	pa2m_afirmar(arbol->comparador(abb_quitar(arbol, &elementos[5]), &elementos[5]) == 0, "Puedo quitar un elemento hoja");
	pa2m_afirmar(!(abb_buscar(arbol, (&elementos[5]))), "El elemento hoja quitado ya no está en el árbol");
	pa2m_afirmar(arbol->comparador(abb_quitar(arbol, &elementos[2]), &elementos[2]) == 0, "Puedo quitar un elemento con un único hijo");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->derecha->elemento, &elementos[7]) == 0, "Eliminar un elemento con un único hijo mantiene la relación de orden");

	pa2m_afirmar(arbol->comparador(abb_quitar(arbol, &elementos[1]), &elementos[1]) == 0, "Puedo quitar un elemento con dos hijos");
	pa2m_afirmar((arbol->comparador(arbol->nodo_raiz->izquierda->elemento, &elementos[4]) == 0) && (arbol->comparador(arbol->nodo_raiz->izquierda->derecha->elemento, &elementos[6]) == 0), "Eliminar un elemento con dos hijos mantiene la relación de orden");
	pa2m_afirmar(abb_tamanio(arbol) == 5, "Tras eliminar varios elementos el tamaño sigue siendo correcto");

	pa2m_afirmar(arbol->comparador(abb_quitar(arbol, &elementos[0]), &elementos[0]) == 0, "Puedo quitar el nodo raíz");
	pa2m_afirmar(arbol->comparador(arbol->nodo_raiz->elemento, &elementos[6]) == 0, "El nuevo nodo raiz es el correcto");
	pa2m_afirmar((arbol->comparador(arbol->nodo_raiz->izquierda->izquierda->elemento, &elementos[3]) == 0) && (arbol->comparador(arbol->nodo_raiz->derecha->elemento, &elementos[7]) == 0), "Eliminar un nodo raíz mantiene la relación de orden");
	pa2m_afirmar(!(abb_buscar(arbol, &elementos[0])), "Buscar el anterior nodo raíz devuelve NULL");
	abb_destruir(arbol);
}

void pruebas_iterador_inorder() {
	int elementos[8] = { 10, 9, 15, 7, 8, 4, 10, 16 };
	abb_t* arbol = crear_abb_con_elementos(elementos, 8);

	int max = 100;
	size_t recorrido_1 = abb_con_cada_elemento(arbol, INORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_1 == abb_tamanio(arbol), "Se recorrió en INORDEN la cantidad correcta de elementos");

	max = 10;
	size_t recorrido_2 = abb_con_cada_elemento(arbol, INORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_2 == 5, "Se recorrió en INORDEN la cantidad correcta de elementos");
	
	max = 14;
	size_t recorrido_3 = abb_con_cada_elemento(arbol, INORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_3 == abb_tamanio(arbol) - 1, "Se recorrió en INORDEN la cantidad correcta de elementos");

	abb_destruir(arbol);
}

void pruebas_iterador_preorder() {
	int elementos[8] = { 10, 9, 15, 7, 8, 4, 10, 16 };
	abb_t* arbol = crear_abb_con_elementos(elementos, 8);

	int max = 100;
	size_t recorrido_1 = abb_con_cada_elemento(arbol, PREORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_1 == abb_tamanio(arbol), "Se recorrió en PREORDEN la cantidad correcta de elementos");

	max = 9;
	size_t recorrido_2 = abb_con_cada_elemento(arbol, PREORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_2 == 1, "Se recorrió en PREORDEN la cantidad correcta de elementos");
	
	max = 14;
	size_t recorrido_3 = abb_con_cada_elemento(arbol, PREORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_3 == abb_tamanio(arbol) - 1, "Se recorrió en PREORDEN la cantidad correcta de elementos");

	abb_destruir(arbol);
}

void pruebas_iterador_postorder() {
	int elementos[8] = { 10, 9, 15, 7, 8, 4, 10, 16 };
	abb_t* arbol = crear_abb_con_elementos(elementos, 8);

	int max = 100;
	size_t recorrido_1 = abb_con_cada_elemento(arbol, POSTORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_1 == abb_tamanio(arbol), "Se recorrió en POSTORDEN la cantidad correcta de elementos");

	max = 10;
	size_t recorrido_2 = abb_con_cada_elemento(arbol, POSTORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_2 == 4, "Se recorrió en POSTORDEN la cantidad correcta de elementos");

	max = 14;
	size_t recorrido_3 = abb_con_cada_elemento(arbol, POSTORDEN, recorrer_mientras_sea_menor, &max);
	pa2m_afirmar(recorrido_3 == 6, "Se recorrió en POSTORDEN la cantidad correcta de elementos");

	abb_destruir(arbol);
}

void pruebas_recorridos() {
	pruebas_iterador_inorder();
  pruebas_iterador_preorder();
  pruebas_iterador_postorder();
}

int main() {
  pruebas_null();
  pruebas_creacion();
  pruebas_insercion();
  pruebas_eliminacion();
	pruebas_recorridos();

  return pa2m_mostrar_reporte();
}