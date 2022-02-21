#ifndef __HOSPITAL_STRUCTS_H__
#define __HOSPITAL_STRUCTS_H__

#include "lista.h"
#include "abb.h"

struct _entrenador_t {
    size_t id;
    char* nombre;
    lista_t* pokemones;
};

struct _hospital_pkm_t {
    lista_t* entrenadores;
    abb_t* pokemones_alfabeticamente;
};

struct _pkm_t {
    entrenador_t* entrenador;
    char* nombre;
    size_t nivel;
};
#endif /* __HOSPITAL_STRUCTS_H__ */
