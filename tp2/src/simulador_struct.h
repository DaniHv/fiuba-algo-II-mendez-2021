#ifndef __SIMULADOR_STRUCT_H__
#define __SIMULADOR_STRUCT_H__

#include "hospital.h"
#include "lista.h"
#include "cola.h"
#include "heap.h"

struct _simulador_t {
    hospital_t* hospital;
    bool simulacion_finalizada;
    unsigned cantidad_eventos_simulados;
    unsigned puntos;
    unsigned dificultad;
    lista_t* dificultades;
    unsigned entrenadores_atendidos;
    cola_t* entrenadores_en_espera;
    heap_t* pokemones_en_espera;
    pokemon_t* pokemon_en_atencion;
    unsigned pokemones_atendidos;
};

#endif /* __SIMULADOR_STRUCT_H__ */
