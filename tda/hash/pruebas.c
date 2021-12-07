#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "pa2mm.h"
#include "src/hash.h"
#include "src/hash_struct.h"

void destructor_dato_free(void* dato) {
    free(dato);
}

void pruebas_con_null() {
    pa2m_nuevo_grupo("PRUEBAS CON VALORES NULL");
    pa2m_afirmar(hash_insertar(NULL, NULL, NULL) == -1, "Insertar un elemento sin un hash, clave o elemento devuelve ERROR.");
    pa2m_afirmar(!hash_obtener(NULL, NULL), "Obtener un elemento sin un hash o clave devuelve NULL.");
    pa2m_afirmar(hash_cantidad(NULL) == 0, "Obtener la cantidad de elementos sin un hash devuelve 0.");
    pa2m_afirmar(!hash_contiene(NULL, NULL) , "Obtener si contiene un elemento sin un hash devuelve FALSE.");
    pa2m_afirmar(!hash_con_cada_clave(NULL, NULL, NULL) , "Iterar cada clave del hash con un HASH, funcion y aux NULL no itera.");
    hash_destruir(NULL);
    pa2m_afirmar(true, "Destruir un hash inexistente no hace nada.");
}

void pruebas_de_creacion() {
    pa2m_nuevo_grupo("PRUEBAS DE CREACIÓN");
    hash_t* hash = hash_crear(&destructor_dato_free, 0);

    pa2m_afirmar(hash != NULL, "Crear un hash con función destructora válida, crea un hash.");
    pa2m_afirmar(hash_cantidad(hash) == 0, "Un hash recién creado inicia con 0 elementos dentro.");
    pa2m_afirmar(hash->tamanio_tabla == 3, "Crear un hash con una capacidad inicial 0, crea el hash con 3.");

    hash_destruir(hash);
    hash = hash_crear(NULL, 1);
    pa2m_afirmar(hash->tamanio_tabla == 3, "Crear un hash con una capacidad inicial 1, crea el hash con 3.");

    hash_destruir(hash);
    hash = hash_crear(NULL, 2);
    pa2m_afirmar(hash->tamanio_tabla == 3, "Crear un hash con una capacidad inicial 2, crea el hash con 3.");

    hash_destruir(hash);
    hash = hash_crear(NULL, 15);
    pa2m_afirmar(hash->tamanio_tabla == 15, "Crear un hash con una capacidad inicial igual o mayor a 3, crea el hash con la capacidad indicada.");

    hash_destruir(hash);
    pa2m_afirmar(true, "Pude destruir el hash creado.");
}

void pruebas_de_insersion() {
    pa2m_nuevo_grupo("PRUEBAS DE INSERSIÓN");
    hash_t* hash = hash_crear(NULL, 500);
    pa2m_afirmar(hash->tamanio_tabla == 500, "Puedo crear un hash con una capacidad de tablas inicial de 500.");

    // Prueba inicial básica
    char KEY[] = "AAA";
    char VAL[] = "111";
    pa2m_afirmar(hash_insertar(hash, (const char*) KEY, VAL) != -1, "Puedo insertar un elemento en el hash correctamente.");
    pa2m_afirmar(hash_cantidad(hash) == 1, "El hash contiene ahora un elemento.");
    pa2m_afirmar(strcmp(hash_obtener(hash, (const char*) KEY), VAL) == 0, "Obtener el valor de la clave guardada, devuelve el elemento correcto.");
    strcpy(KEY, "BBB");
    pa2m_afirmar(!hash_obtener(hash, (const char*) KEY), "La clave se guarda por copia y no por referencia.");

    // Prueba insersión NULL
    char KEY_NULL[] = "XXX";
    pa2m_afirmar(hash_insertar(hash, (const char*) KEY_NULL, NULL) != -1, "Puedo insertar un elemento NULL con una clave válida en el hash correctamente.");
    pa2m_afirmar(hash_cantidad(hash) == 2, "El hash contiene sumó a la clave con elemento NULL.");
    pa2m_afirmar(!hash_obtener(hash, (const char*) KEY_NULL), "Obtener el valor de la clave con null guardada, devuelve el elemento null.");

    // Múltiples pruebas de insersión - MODOHARDCORE
    char VAL_COMUN[] = "NOMATTERWHATIPUTHERENOW";

    size_t i = 1;
    bool hubo_error = false;
    while (i <= 100 && !hubo_error) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        hubo_error = (hash_insertar(hash, (const char*) KEY, VAL_COMUN) == -1);

        i++;
    }

    pa2m_afirmar((!hubo_error && hash_cantidad(hash) == 102), "Pude insertar con éxito diversos elementos con distintas claves.");
    
    hubo_error = false;
    for(size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        if (strcmp(hash_obtener(hash, (const char*) KEY), VAL_COMUN) != 0) {
            hubo_error = true;
        }
    }

    pa2m_afirmar(!hubo_error, "Todos los elementos insertados se encuentran dentro del hash.");

    strcpy(VAL_COMUN, "NOWIMDIFFERENT");

    for(size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        hash_insertar(hash, (const char *) KEY, VAL_COMUN);
    }

    pa2m_afirmar(hash_cantidad(hash) == 102, "Inserté nuevamente todos los elementos con diferente valor, y continua la cantidad correcta.");
    
    hubo_error = false;
    for(size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        if (strcmp(hash_obtener(hash, (const char *) KEY), VAL_COMUN) != 0) {
            hubo_error = true;
        }
    }

    pa2m_afirmar(!hubo_error, "Todos los elementos dupliacdos insertados ahora tienen el nuevo valor.");


    // Pruebas de sobreescritura
    char KEY_SOBREESCRITURA[] = "ZZZ";
    char VAL_SOBREESCRITURA[] = "666";
    
    hash_insertar(hash, (const char*) KEY_SOBREESCRITURA, VAL_SOBREESCRITURA);
    pa2m_afirmar(strcmp(hash_obtener(hash, (const char*)KEY_SOBREESCRITURA), VAL_SOBREESCRITURA) == 0, "Insertar un elemento, una vez más se inserta correctamente");

    hash_insertar(hash, (const char*) KEY_SOBREESCRITURA, VAL_SOBREESCRITURA);
    pa2m_afirmar(hash_cantidad(hash) == 103, "Insertar con una clave y valor repetido no agranda el contenido del hash.");
    pa2m_afirmar(strcmp(hash_obtener(hash, (const char*)KEY_SOBREESCRITURA), VAL_SOBREESCRITURA) == 0, "Puedo seguir obteniendo el hash que intenté insertar repetido.");
    strcpy(VAL_SOBREESCRITURA, "999");
    hash_insertar(hash, (const char*) KEY_SOBREESCRITURA, VAL_SOBREESCRITURA);
    pa2m_afirmar(strcmp(hash_obtener(hash, (const char*)KEY_SOBREESCRITURA), VAL_SOBREESCRITURA) == 0, "Insertar con clave repetida actualiza correctamente el valor.");
    pa2m_afirmar(hash_cantidad(hash) == 103, "Insertar con clave repetida mantiene el tamaño del hash.");

    hash_destruir(hash);
    pa2m_afirmar(true, "Pude destruir el hash creado con todos los elementos dentro");
}

void pruebas_de_eliminacion() {
    pa2m_nuevo_grupo("PRUEBAS DE ELIMINACIÓN");
    hash_t* hash = hash_crear(NULL, 150);

    // Eliminación inexistente
    char KEY[] = "XDXDXD";
    pa2m_afirmar((hash_quitar(hash, (const char*) KEY) == -1), "Quitar una clave inexistente devuelve -1");
    pa2m_afirmar((hash_cantidad(hash) == 0), "Quitar una clave inexistente mantiene los cantidad");

    // Eliminación básica
    char VAL[] = "WIIII";
    pa2m_afirmar(!hash_insertar(hash, (const char*) KEY, VAL), "Puedo insertar la misma clave que anteriormente intenté quitar sin existir");
    pa2m_afirmar((hash_cantidad(hash) == 1), "La cantidad de claves se actualizó");
    pa2m_afirmar((strcmp(hash_obtener(hash, (const char*) KEY), VAL) == 0), "El clave de la clave insertada correcto");
    pa2m_afirmar(!hash_quitar(hash, (const char*) KEY), "Pude quitar el clave antes insertado.");
    pa2m_afirmar(hash_quitar(hash, (const char*) KEY) == -1, "Si vuelvo a intentar quitar la clave que ya quité, no hace nada");
    pa2m_afirmar(!hash_obtener(hash, (const char*) KEY), "Ahora la clave no se encuentra dentro del hash");
    pa2m_afirmar((hash_cantidad(hash) == 0), "La cantidad ahora vuelve a ser 0");

    // Eliminación avanzada - MODO HARDCORE
    char VAL_COMUN[] = "WEEEEEEEEEEEEEE";

    for (size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        hash_insertar(hash, (const char*) KEY, VAL_COMUN);
    }

    pa2m_afirmar(hash_cantidad(hash) == 100, "Igual que en inserción, pude insertar gran cantidad de claves al hash");

    for (size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        hash_quitar(hash, (const char*) KEY);
    }

    pa2m_afirmar(hash_cantidad(hash) == 0, "Pude quitar todos las claves antes insertadas");

    size_t i = 1;
    bool encontre_alguno = false;
    while (i <= 100 && !encontre_alguno) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        encontre_alguno = (!!hash_obtener(hash, (const char*) KEY));

        i++;
    }

    pa2m_afirmar(hash_cantidad(hash) == 0, "No pude encontrar ninguno de las claves que eliminé");

    hash_destruir(hash);
    pa2m_afirmar(true, "Pude destruir el hash creado");
}

bool iterar_siempre_true(hash_t* hash, const char* clave, void* aux) {
    return true;
}
bool iterar_siempre_false(hash_t* hash, const char* clave, void* aux) {
    return false;
}

typedef struct iterador_hasta_aux {
    size_t actual;
    size_t hasta;
} iterador_hasta_aux_t;

bool iterar_hasta(hash_t* hash, const char* clave, void* aux) {
    if (!aux) return false;

    iterador_hasta_aux_t* iterador_aux = (iterador_hasta_aux_t*) aux;

    iterador_aux->actual++;

    return iterador_aux->actual >= iterador_aux->hasta;
}

void pruebas_de_iteracion() {
    pa2m_nuevo_grupo("PRUEBAS DE ITERACIÓN");
    hash_t* hash = hash_crear(NULL, 150);

    pa2m_afirmar(!hash_con_cada_clave(hash, NULL, NULL), "Iterar un hash sin una función no hace nada.");
    pa2m_afirmar(!hash_con_cada_clave(hash, &iterar_siempre_true, NULL), "Iterar un hash sin elementos devuelve 0.");

    char VAL_COMUN[] = "WEEEEEEEEEEEEEE";

    for (size_t i = 1; i <= 100; i++) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        hash_insertar(hash, (const char*) KEY, VAL_COMUN);
    }

    pa2m_afirmar(hash_con_cada_clave(hash, &iterar_siempre_false, NULL) == 100, "Iterar un hash con una funcion siempre false, recorre todo el hash.");
    pa2m_afirmar(hash_con_cada_clave(hash, &iterar_siempre_true, NULL) == 1, "Iterar un hash con una funcion siempre true, solo llega a recorrer el primero.");

    iterador_hasta_aux_t iterador_aux = {
        .actual = 0,
        .hasta = 1,
    };
    pa2m_afirmar(hash_con_cada_clave(hash, &iterar_hasta, (void*) &iterador_aux) == 1, "Iterar un hash con una función que si detiene, recorre la cantidad correcta si detengo en el primer elemento.");

    iterador_aux.actual = 0;
    iterador_aux.hasta = 50;
    pa2m_afirmar(hash_con_cada_clave(hash, &iterar_hasta, (void*) &iterador_aux) == 50, "Iterar un hash con una función que si detiene, recorre la cantidad correcta si es un elemento del medio.");

    iterador_aux.actual = 0;
    iterador_aux.hasta = 100;
    pa2m_afirmar(hash_con_cada_clave(hash, &iterar_hasta, (void*) &iterador_aux) == 100, "Iterar un hash con una función que si detiene, recorre la cantidad correcta si es el hasta el último elemento.");

    hash_destruir(hash);
    pa2m_afirmar(true, "Pude destruir el hash creado");
}


void pruebas_de_rehash() {
    pa2m_nuevo_grupo("PRUEBAS DE REHASH");
    hash_t* hash = hash_crear(NULL, 3);

    pa2m_afirmar(hash, "Pude crear un hash con el tamaño inicial mínimo.");

    char VAL_COMUN[] = "FIUMBAAAAA";
    bool hubo_error = false;
    size_t i = 1;
    while (i <= 100 && !hubo_error) {
        char KEY[2];
        KEY[0] = i;
        KEY[1] = '\0';

        if (hash_insertar(hash, (const char*) KEY, VAL_COMUN) == -1) {
            hubo_error = true;
        }

        i++;
    }

    pa2m_afirmar(!hubo_error, "Pude agregar gran cantidad de elementos al hash.");
    pa2m_afirmar(hash_cantidad(hash) == 100, "El hash tiene la cantidad correcta de elementos.");
    pa2m_afirmar(hash->tamanio_tabla > 3, "La tabla del hash creció en tamaño.");

    hash_destruir(hash);
    pa2m_afirmar(true, "Pude destruir el hash creado");
}

int main(){
    pruebas_con_null();
    pruebas_de_creacion();
    pruebas_de_insersion();
    pruebas_de_eliminacion();
    pruebas_de_iteracion();
    pruebas_de_rehash();
    return pa2m_mostrar_reporte();
}